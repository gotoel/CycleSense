package web

import (
	"cycleSenseCentral/src/sensors/manager"
	"fmt"
	"net/http"
)

func RunWebserver() {
	http.Handle("/", http.FileServer(http.Dir("./src/web/gauges")))
	http.HandleFunc("/rpm", rpm)
	http.ListenAndServe(":3000", nil)
}

func rpm(w http.ResponseWriter, r *http.Request) {
	fmt.Fprint(w, manager.Manager.Bike.RPM)
}
