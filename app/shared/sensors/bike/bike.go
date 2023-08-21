package bike

const Name = "Bike"

type Data struct {
	RPM float32 `json:"rpm" msgpack:"rpm"`
}

func (data *Data) Reset() {
	data.RPM = 0
}
