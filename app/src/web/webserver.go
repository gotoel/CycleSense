package web

import (
	"cycleSenseCentral/src/sensors/manager"
	"encoding/json"
	"net/http"
)

func RunWebserver() {
	http.Handle("/", http.FileServer(http.Dir("./src/web/dashboard")))
	http.HandleFunc("/stats", stats)
	http.ListenAndServe(":3000", nil)
}

func stats(w http.ResponseWriter, r *http.Request) {
	result := make(map[string]interface{})
	result["bike"] = manager.Manager.Bike
	result["chuck"] = manager.Manager.Chuck
	json.NewEncoder(w).Encode(result)
}
