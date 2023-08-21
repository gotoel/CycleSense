package input

import (
	"github.com/WesleiRamos/goxinput"
	"github.com/micmonay/keybd_event"
	"shared/constants"
	"shared/sensors/chuck"
	"shared/sensors/manager"
	"shared/types"
	"sync"
)

var once sync.Once

type SenseController struct {
	sensorEvents <-chan types.Event

	controller *goxinput.VirtualController
	kb         keybd_event.KeyBonding
	keyMap     map[int]bool
}

var (
	Input *SenseController
)

func NewSenseController(ch <-chan types.Event) *SenseController {
	once.Do(func() {
		Input = &SenseController{
			sensorEvents: ch,
			keyMap:       map[int]bool{},
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
			var chuckData *chuck.Data

			if event.Name == constants.EventReset {
				manager.Manager.ResetSensorData()
			}

			if manager.Manager.Bike.RPM < constants.MinimumRpm {
				chuckData = &chuck.Data{}
			} else {
				chuckData = manager.Manager.Chuck
			}

			senseAxisXLeft := (float32(chuckData.Sticks.AxisLeftX) * (2) / 255) - 1
			senseAxisYLeft := (float32(chuckData.Sticks.AxisLeftY) * (2) / 255) - 1

			senseAxisXRight := (float32(chuckData.Sticks.AxisRightX) * (2) / 255) - 1
			senseAxisYRight := (float32(chuckData.Sticks.AxisRightY) * (2) / 255) - 1

			//println(fmt.Sprintf("(%0.2f, %0.2f)", senseAxisX, senseAxisY))

			senseController.controller.SetAxis(goxinput.AXIS_LX, senseAxisXLeft)
			senseController.controller.SetAxis(goxinput.AXIS_LY, senseAxisYLeft)

			senseController.controller.SetAxis(goxinput.AXIS_RX, senseAxisXRight)
			senseController.controller.SetAxis(goxinput.AXIS_RY, senseAxisYRight)

			senseController.controller.SetBtn(goxinput.BUTTON_A,
				chuckData.Buttons.ButtonA || chuckData.Buttons.ButtonC)

			senseController.controller.SetBtn(goxinput.BUTTON_B,
				chuckData.Buttons.ButtonB || chuckData.Buttons.ButtonZ)

			senseController.controller.SetBtn(goxinput.BUTTON_X, chuckData.Buttons.ButtonX)
			senseController.controller.SetBtn(goxinput.BUTTON_Y, chuckData.Buttons.ButtonY)

			if chuckData.Triggers.TriggerLeft {
				senseController.controller.SetTrigger(goxinput.BUTTON_LT, 1)
			} else {
				senseController.controller.SetTrigger(goxinput.BUTTON_LT, 0)
			}

			if chuckData.Triggers.TriggerRight {
				senseController.controller.SetTrigger(goxinput.BUTTON_RT, 1)
			} else {
				senseController.controller.SetTrigger(goxinput.BUTTON_RT, 0)
			}

			senseController.controller.SetBtn(goxinput.BUTTON_LB, chuckData.Triggers.TriggerZLeft)
			senseController.controller.SetBtn(goxinput.BUTTON_RB, chuckData.Triggers.TriggerZRight)

			senseController.controller.SetBtn(goxinput.BUTTON_BACK, chuckData.Buttons.ButtonMinus)
			senseController.controller.SetBtn(goxinput.BUTTON_START, chuckData.Buttons.ButtonPlus)
			if chuckData.Buttons.ButtonHome {
				senseController.PressKey(keybd_event.VK_ENTER)
			} else {
				senseController.ReleaseKey(keybd_event.VK_ENTER)
			}

			if chuckData.Dpad.PadUp {
				senseController.controller.SetDpad(goxinput.DPAD_UP)
			} else if chuckData.Dpad.PadDown {
				senseController.controller.SetDpad(goxinput.DPAD_DOWN)
			} else if chuckData.Dpad.PadRight {
				senseController.controller.SetDpad(goxinput.DPAD_RIGHT)
			} else if chuckData.Dpad.PadLeft {
				senseController.controller.SetDpad(goxinput.DPAD_LEFT)
			} else {
				senseController.controller.SetDpad(goxinput.DPAD_OFF)
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

func (senseController *SenseController) PressKey(key int) {
	if toggled, ok := senseController.keyMap[key]; ok {
		if !toggled {
			senseController.keyMap[key] = true
			senseController.processKeys()
		}
	} else {
		senseController.keyMap[key] = true
		senseController.processKeys()
	}
}

func (senseController *SenseController) ReleaseKey(key int) {
	if _, ok := senseController.keyMap[key]; ok {
		delete(senseController.keyMap, key)
		senseController.processKeys()
	}
}

func (senseController *SenseController) processKeys() {
	senseController.kb.Clear()
	for key, toggled := range senseController.keyMap {
		if toggled {
			senseController.kb.AddKey(key)
		}
	}

	if len(senseController.keyMap) > 0 {
		senseController.kb.Press()
	}
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
