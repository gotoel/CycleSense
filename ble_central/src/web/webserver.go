package web

import (
	"cycleSenseCentral/src/sensors"
	"encoding/json"
	"net/http"
)

func RunWebserver() {
	http.Handle("/", http.FileServer(http.Dir("./src/web/gauges")))
	http.HandleFunc("/rpm", rpm)
	http.ListenAndServe(":3000", nil)
}

func rpm(w http.ResponseWriter, r *http.Request) {
	json.NewEncoder(w).Encode(sensors.Manager.Bike.RPM)
}
