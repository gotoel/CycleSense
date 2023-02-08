package chuck

const Name = "Chuck"

type Data struct {
	Info          Info          `json:"info" msgpack:"info"`
	Accelerometer Accelerometer `json:"accelerometer" msgpack:"accelerometer"`
	Sticks        Sticks        `json:"sticks" msgpack:"sticks"`
	Buttons       Buttons       `json:"buttons" msgpack:"buttons"`
	Triggers      Triggers      `json:"triggers" msgpack:"triggers"`
	Dpad          Dpad          `json:"dpad" msgpack:"dpad"`
}

type Info struct {
	ControllerType uint32 `json:"controller_type" msgpack:"controller_type,omitempty"`
}

type Accelerometer struct {
	AcclX uint32 `json:"accl_x" msgpack:"accl_x,omitempty"`
	AcclY uint32 `json:"accl_y" msgpack:"accl_y,omitempty"`
	AcclZ uint32 `json:"accl_z" msgpack:"accl_z,omitempty"`
}

type Sticks struct {
	AxisLeftX  uint32 `json:"axis_left_x" msgpack:"axis_left_x,omitempty"`
	AxisLeftY  uint32 `json:"axis_left_y" msgpack:"axis_left_y,omitempty"`
	AxisRightX uint32 `json:"axis_right_x" msgpack:"axis_right_x,omitempty"`
	AxisRightY uint32 `json:"axis_right_y" msgpack:"axis_right_y,omitempty"`
}

type Buttons struct {
	ButtonC bool `json:"button_c" msgpack:"button_c,omitempty"`
	ButtonZ bool `json:"button_z" msgpack:"button_z,omitempty"`

	ButtonA bool `json:"button_a" msgpack:"button_a,omitempty"`
	ButtonB bool `json:"button_b" msgpack:"button_b,omitempty"`
	ButtonX bool `json:"button_x" msgpack:"button_x,omitempty"`
	ButtonY bool `json:"button_y" msgpack:"button_y,omitempty"`

	ButtonMinus bool `json:"button_minus" msgpack:"button_minus,omitempty"`
	ButtonHome  bool `json:"button_home" msgpack:"button_home,omitempty"`
	ButtonPlus  bool `json:"button_plus" msgpack:"button_plus,omitempty"`
}

type Triggers struct {
	TriggerLeft   uint32 `json:"trigger_left" msgpack:"trigger_left"`
	TriggerRight  uint32 `json:"trigger_right" msgpack:"trigger_right"`
	TriggerZLeft  bool   `json:"trigger_z_left" msgpack:"trigger_z_left"`
	TriggerZRight bool   `json:"trigger_z_right" msgpack:"trigger_z_right"`
}

type Dpad struct {
	PadUp    bool `json:"pad_up" msgpack:"pad_up"`
	PadDown  bool `json:"pad_down" msgpack:"pad_down"`
	PadRight bool `json:"pad_right" msgpack:"pad_right"`
	PadLeft  bool `json:"pad_left" msgpack:"pad_left"`
}
