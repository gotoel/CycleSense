package web

import (
	"cycleSenseCentral/src/sensors"
	"fmt"
	"net/http"
)

func RunWebserver() {
	http.Handle("/", http.FileServer(http.Dir("./src/web/gauges")))
	http.HandleFunc("/rpm", rpm)
	http.ListenAndServe(":3000", nil)
}

func rpm(w http.ResponseWriter, r *http.Request) {
	fmt.Fprint(w, sensors.Manager.Bike.RPM)
}
