package input

import (
	"cycleSenseCentral/src/sensors/manager"
	"cycleSenseCentral/src/sensors/sensor_event"
	"github.com/WesleiRamos/goxinput"
	"github.com/micmonay/keybd_event"
	"sync"
)

var once sync.Once

type SenseController struct {
	sensorEvents <-chan sensor_event.SensorEvent

	controller *goxinput.VirtualController
	kb         keybd_event.KeyBonding
}

var (
	Input *SenseController
)

func NewSenseController(ch <-chan sensor_event.SensorEvent) *SenseController {
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
			_ = event
			if manager.Manager.Bike.RPM > 40 {
				senseAxisX := (float32(manager.Manager.Chuck.AxisX) * (2) / 255) - 1
				senseAxisY := (float32(manager.Manager.Chuck.AxisY) * (2) / 255) - 1

				//println(fmt.Sprintf("(%0.2f, %0.2f)", senseAxisX, senseAxisY))

				senseController.SetAxis(senseAxisX, senseAxisY)
			} else {
				senseController.SetAxis(0, 0)
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

	senseController.controller.SetAxis(goxinput.AXIS_LX, x)
	senseController.controller.SetAxis(goxinput.AXIS_LY, y)
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
