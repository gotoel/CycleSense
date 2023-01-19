package main

import (
	"github.com/WesleiRamos/goxinput"
)

type SenseController struct {
	controller *goxinput.VirtualController
}

func NewSenseController() (senseController SenseController) {
	return
}

func (senseController *SenseController) Initialize() {
	senseController.controller = goxinput.NewController()
	if !senseController.controller.IsVBusExists() {
		panic("VBus driver is not installed")
	}

	// Plugin controller
	if error := senseController.controller.PlugIn(); error != nil {
		panic(error)
	}
}

func (senseController *SenseController) SetAxis(rpm float32) {
	const MAX_RPM = 300 // this needs to be moved
	senseController.controller.SetAxis(goxinput.AXIS_LX, rpm/MAX_RPM)
}

func (senseController *SenseController) Unplug() {
	senseController.controller.Unplug()
}
