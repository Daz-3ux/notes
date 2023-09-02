package main

import (
	"log"
	"net/http"
)

func main() {
	http.HandleFunc("/ping", pong)
	http.HandleFunc("/hello", hw)
	log.Println("Starting http Server ...")
	log.Fatal(http.ListenAndServe(":20002", nil))
}

func pong(w http.ResponseWriter, r *http.Request) {
	w.Write([]byte("pong"))
}

func hw(w http.ResponseWriter, r *http.Request) {
	w.Write([]byte("Hello World"))
}
