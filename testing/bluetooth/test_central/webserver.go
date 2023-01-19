package main

import (
	"encoding/json"
	"net/http"
)

func runWebserver() {
	http.Handle("/", http.FileServer(http.Dir("./web")))
	http.HandleFunc("/rpm", rpm)
	http.ListenAndServe(":3000", nil)
}

func rpm(w http.ResponseWriter, r *http.Request) {
	json.NewEncoder(w).Encode(Bike)
}
