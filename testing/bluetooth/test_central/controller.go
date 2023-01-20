package main

import (
	"github.com/WesleiRamos/goxinput"
	"github.com/micmonay/keybd_event"
)

type SenseController struct {
	controller *goxinput.VirtualController
	kb         keybd_event.KeyBonding
}

func NewSenseController() (senseController SenseController) {
	return
}

func (senseController *SenseController) Initialize() {
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
	if error := senseController.controller.PlugIn(); error != nil {
		panic(error)
	}
}

func (senseController *SenseController) SetAxis(rpm float32) {
	const MAX_RPM = 300 // this needs to be moved
	if rpm > MAX_RPM {
		rpm = MAX_RPM
	}

	senseController.controller.SetAxis(goxinput.AXIS_LX, rpm/MAX_RPM)
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
