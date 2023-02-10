package input

import (
	"github.com/WesleiRamos/goxinput"
	"github.com/micmonay/keybd_event"
	"shared/constants"
	"shared/sensors/manager"
	"shared/types"
	"sync"
)

var once sync.Once

type SenseController struct {
	sensorEvents <-chan types.Event

	controller *goxinput.VirtualController
	kb         keybd_event.KeyBonding
}

var (
	Input *SenseController
)

func NewSenseController(ch <-chan types.Event) *SenseController {
	once.Do(func() {
		Input = &SenseController{
			sensorEvents: ch,
		}
	})

	return Input
}

func (senseController *SenseController) Initialize() {
	defer func() {
		if err := recover(); err != nil {
			senseController.controller.Unplug(true)
		}
	}()

	var err error
	senseController.kb, err = keybd_event.NewKeyBonding()
	if err != nil {
		panic(err)
	}

	// Set hotkey combo to press on RPMs
	senseController.kb.SetKeys(keybd_event.VK_A, keybd_event.VK_B)
	senseController.kb.SetKeys(keybd_event.VK_Z)
	senseController.kb.HasSHIFT(true)
	senseController.kb.HasALT(true)

	senseController.controller = goxinput.NewController()
	if !senseController.controller.IsVBusExists() {
		panic("VBus driver is not installed")
	}

	// Plugin controller
	if err = senseController.controller.PlugIn(); err != nil {
		panic(err)
	}

	go senseController.processEvents()
}

func (senseController *SenseController) processEvents() {
	go func() {
		for event := range senseController.sensorEvents {
			if event.Name == constants.EventReset {
				manager.Manager.ResetSensorData()
			}

			if manager.Manager.Bike.RPM > 40 {
				senseAxisXLeft := (float32(manager.Manager.Chuck.Sticks.AxisLeftX) * (2) / 255) - 1
				senseAxisYLeft := (float32(manager.Manager.Chuck.Sticks.AxisLeftY) * (2) / 255) - 1

				senseAxisXRight := (float32(manager.Manager.Chuck.Sticks.AxisRightX) * (2) / 255) - 1
				senseAxisYRight := (float32(manager.Manager.Chuck.Sticks.AxisRightY) * (2) / 255) - 1

				//println(fmt.Sprintf("(%0.2f, %0.2f)", senseAxisX, senseAxisY))

				senseController.controller.SetAxis(goxinput.AXIS_LX, senseAxisXLeft)
				senseController.controller.SetAxis(goxinput.AXIS_LY, senseAxisYLeft)

				senseController.controller.SetAxis(goxinput.AXIS_RX, senseAxisXRight)
				senseController.controller.SetAxis(goxinput.AXIS_RY, senseAxisYRight)

				senseController.controller.SetBtn(goxinput.BUTTON_A,
					manager.Manager.Chuck.Buttons.ButtonA || manager.Manager.Chuck.Buttons.ButtonC)

				senseController.controller.SetBtn(goxinput.BUTTON_B,
					manager.Manager.Chuck.Buttons.ButtonB || manager.Manager.Chuck.Buttons.ButtonZ)

				senseController.controller.SetBtn(goxinput.BUTTON_X, manager.Manager.Chuck.Buttons.ButtonX)
				senseController.controller.SetBtn(goxinput.BUTTON_Y, manager.Manager.Chuck.Buttons.ButtonY)

				if manager.Manager.Chuck.Triggers.TriggerLeft {
					senseController.controller.SetTrigger(goxinput.BUTTON_LT, 1)
				} else {
					senseController.controller.SetTrigger(goxinput.BUTTON_LT, 0)
				}

				if manager.Manager.Chuck.Triggers.TriggerRight {
					senseController.controller.SetTrigger(goxinput.BUTTON_RT, 1)
				} else {
					senseController.controller.SetTrigger(goxinput.BUTTON_RT, 0)
				}

				senseController.controller.SetBtn(goxinput.BUTTON_LB, manager.Manager.Chuck.Triggers.TriggerZLeft)
				senseController.controller.SetBtn(goxinput.BUTTON_RB, manager.Manager.Chuck.Triggers.TriggerZRight)

				senseController.controller.SetBtn(goxinput.BUTTON_BACK, manager.Manager.Chuck.Buttons.ButtonMinus)
				senseController.controller.SetBtn(goxinput.BUTTON_START, manager.Manager.Chuck.Buttons.ButtonPlus)

				if manager.Manager.Chuck.Dpad.PadUp {
					senseController.controller.SetDpad(goxinput.DPAD_UP)
				} else if manager.Manager.Chuck.Dpad.PadDown {
					senseController.controller.SetDpad(goxinput.DPAD_DOWN)
				} else if manager.Manager.Chuck.Dpad.PadRight {
					senseController.controller.SetDpad(goxinput.DPAD_RIGHT)
				} else if manager.Manager.Chuck.Dpad.PadLeft {
					senseController.controller.SetDpad(goxinput.DPAD_LEFT)
				} else {
					senseController.controller.SetDpad(goxinput.DPAD_OFF)
				}
			}
		}
	}()
}

func (senseController *SenseController) SetAxisRPM(rpm float32) {
	const MAX_RPM = 300 // this needs to be moved
	if rpm > MAX_RPM {
		rpm = MAX_RPM
	}

	senseController.controller.SetAxis(goxinput.AXIS_LX, rpm/MAX_RPM)
}

func (senseController *SenseController) SetAxis(x float32, y float32) {
	// axis range is -1 to 1

	set := senseController.controller.SetAxis(goxinput.AXIS_LX, x)
	set = senseController.controller.SetAxis(goxinput.AXIS_LY, y)
	_ = set
}

func (senseController *SenseController) PressHotkey() {
	senseController.kb.Press()
}

func (senseController *SenseController) ReleaseHotkey() {
	senseController.kb.Release()
}

func (senseController *SenseController) Unplug() {
	senseController.controller.Unplug()
}
