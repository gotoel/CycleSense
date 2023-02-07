package chuck

const Name = "Chuck"

type Data struct {
	Info          Info          `json:"info"`
	Accelerometer Accelerometer `json:"accelerometer"`
	Sticks        Sticks        `json:"sticks"`
	Buttons       Buttons       `json:"buttons"`
	Triggers      Triggers      `json:"triggers"`
	Dpad          Dpad          `json:"dpad"`
}

type Info struct {
	ControllerType uint32 `json:"controller_type,omitempty"`
}

type Accelerometer struct {
	AcclX uint32 `json:"accl_x,omitempty"`
	AcclY uint32 `json:"accl_y,omitempty"`
	AcclZ uint32 `json:"accl_z,omitempty"`
}

type Sticks struct {
	AxisLeftX  uint32 `json:"axis_left_x,omitempty"`
	AxisLeftY  uint32 `json:"axis_left_y,omitempty"`
	AxisRightX uint32 `json:"axis_right_x,omitempty"`
	AxisRightY uint32 `json:"axis_right_y,omitempty"`
}

type Buttons struct {
	ButtonC bool `json:"button_c,omitempty"`
	ButtonZ bool `json:"button_z,omitempty"`

	ButtonA bool `json:"button_a,omitempty"`
	ButtonB bool `json:"button_b,omitempty"`
	ButtonX bool `json:"button_x,omitempty"`
	ButtonY bool `json:"button_y,omitempty"`

	ButtonMinus bool `json:"button_minus,omitempty"`
	ButtonHome  bool `json:"button_home,omitempty"`
	ButtonPlus  bool `json:"button_plus,omitempty"`
}

type Triggers struct {
	TriggerLeft   uint32 `json:"trigger_left"`
	TriggerRight  uint32 `json:"trigger_right"`
	TriggerZLeft  bool   `json:"trigger_z_left"`
	TriggerZRight bool   `json:"trigger_z_right"`
}

type Dpad struct {
	PadUp    bool `json:"pad_up"`
	PadDown  bool `json:"pad_down"`
	PadRight bool `json:"pad_right"`
	PadLeft  bool `json:"pad_left"`
}
