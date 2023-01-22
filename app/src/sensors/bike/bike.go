package bike

import "cycleSenseCentral/src/input"

type Data struct {
	RPM float32
}

func ProcessInputs(data Data) {
	//const DEAD_ZONE = 30
	//if Bike.RPM > DEAD_ZONE {
	//	senseController.PressHotkey()
	//} else {
	//	senseController.ReleaseHotkey()
	//}

	input.Input.SetAxisRPM(data.RPM)
}
