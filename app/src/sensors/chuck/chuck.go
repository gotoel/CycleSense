package chuck

const Name = "Chuck"

type Data struct {
	AcclX   uint32 `json:"accl_x"`
	AcclY   uint32 `json:"accl_y"`
	AcclZ   uint32 `json:"accl_z"`
	AxisX   uint32 `json:"axis_x"`
	AxisY   uint32 `json:"axis_y"`
	ButtonC bool   `json:"button_c"`
	ButtonZ bool   `json:"button_z"`
}

func ProcessInputs(data Data) {
	//senseAxisX := (float32(data.AxisX) * (2) / 255) - 1
	//senseAxisY := (float32(data.AxisY) * (2) / 255) - 1

	//println(fmt.Sprintf("(%0.2f, %0.2f)", senseAxisX, senseAxisY))

	//input.Input.SetAxis(senseAxisX, senseAxisY)
}
