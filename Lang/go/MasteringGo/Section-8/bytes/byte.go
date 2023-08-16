package main

import (
	"fmt"
	"bytes"
	"io"
	"os"
)

func main() {
	var buffer bytes.Buffer
	buffer.Write([]byte("This is"))
	fmt.Fprintf(&buffer, " a string!\n")
	buffer.WriteTo(os.Stdout)
	buffer.WriteTo(os.Stdout)

	buffer.Reset()
	buffer.Write([]byte("Mastering GO!"))
	r := bytes.NewReader([]byte(buffer.String()))
	fmt.Println(buffer.String())
	for {
		b := make([]byte, 3)
		n, err := r.Read(b)
		if err == io.EOF {
			break
		}

		if err != nil {
			fmt.Println(err)
			continue
		}

		fmt.Printf("Readn %d bytes is %s\n", n, b)
	}
}