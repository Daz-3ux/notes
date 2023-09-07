package main

import (
	"os"
	"log"
	"net/http"
)

func setupLogger() {
	logFileLocation, _ := os.OpenFile("./test.log", os.O_CREATE|os.O_APPEND|os.O_RDWR, 0744)
	log.SetOutput(logFileLocation)
}

func simpleHttpGet(url string) {
	resp, err := http.Get(url)
	if err != nil {
		log.Printf("Error fetching url %s: %s", url, err.Error())
	}else {
		log.Printf("Status Code for %s : %s", url, resp.Status)
		resp.Body.Close()
	}
}

func main() {
	setupLogger()
	simpleHttpGet("www.google.com")
	simpleHttpGet("http://www.google.com")
}