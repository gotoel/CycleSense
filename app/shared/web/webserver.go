package web

import (
	"encoding/json"
	"net/http"
	"shared/comms"
	"shared/sensors/manager"
)

func RunWebserver() {
	http.Handle("/", http.FileServer(http.Dir("./shared/web/dashboard")))
	http.Handle("/dashboard", http.FileServer(http.Dir("./shared/web/dashboard")))
	http.Handle("/dashboard/",
		http.StripPrefix("/dashboard/", http.FileServer(http.Dir("./shared/web/dashboard"))))

	http.HandleFunc("/stats", stats)
	http.ListenAndServe(":3000", nil)
}

func stats(w http.ResponseWriter, r *http.Request) {
	result := make(map[string]interface{})
	result["bike"] = manager.Manager.Bike
	result["chuck"] = manager.Manager.Chuck
	result["connected"] = comms.Handler.IsConnected()
	json.NewEncoder(w).Encode(result)
}
