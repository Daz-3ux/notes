package main

import (
	"fmt"
	"strings"
	"bytes"
)

func basename1(s string) string {
	for i := len(s) - 1; i >= 0; i-- {
		if s[i] == '/' {
			s = s[i+1:]
			break
		}
	}
	for i := len(s) - 1; i >= 0; i-- {
		if s[i] == '.' {
			s = s[:i]
			break
		}
	}
	return s
}

func basename2(s string) string {
	slash := strings.LastIndex(s, "/") // -1 if "/" not found
	s = s[slash+1:]
	if dot := strings.LastIndex(s, "."); dot >= 0 {
		s = s[:dot]
	}
	return s
}

func comma1(s string) string {
	n := len(s)
	if n <= 3 {
		return s
	}
	return comma1(s[:n-3]) + "," + s[n-3:]
}

func comma2(s string) string {
	n := len(s)
	if n <= 3 {
		return s
	}

	cycle := n / 3
	reminder := n % 3

	var buf bytes.Buffer
	buf.WriteString(s[:reminder])
	for i := 0; i < cycle; i++ {
		buf.WriteString(",")
		start := reminder + i*3
		end := start + 3
		buf.WriteString(s[start:end])
	}
	return buf.String()
}

func comma3(s string) string {
	var buf bytes.Buffer
	var symbol byte
	if s[0] == '-' || s[0] == '+' {
		symbol = s[0]
		s = s[1:]
	}

	buf.WriteByte(symbol)

	ss := strings.Split(s, ".")
	integer := ss[0]
	n := len(integer)
	cycle := n / 3
	reminder := n % 3
	buf.WriteString(s[:reminder])
	for i := 0; i < cycle; i++ {
		buf.WriteString(",")
		start := reminder + i*3
		end := start + 3
		buf.WriteString(s[start:end])
	}

	if len(ss) == 2 {
		buf.WriteString(".")
		buf.WriteString(ss[1])
	}

	return buf.String()
}

func same(a, b string) bool {
	if len(a) != len(b) {
		return false
	}
	seen := make(map[rune]int)

	for _, v := range a {
		seen[v]++
	}

	for _, v := range b {
		seen[v]--
	}

	for _, v := range seen {
		if v != 0 {
			return false
		}
	}

	return true
}

func main() {
	s1 := basename1("test/testt/testt.t")
	fmt.Println(s1)
	s2 := basename2("test/testt/testt.t")
	fmt.Println(s2)
	s3 := comma1("01234567890")
	fmt.Println(s3)
	s4 := comma2("01234567890")
	fmt.Println(s4)
	s5 := comma3("01234567890.987654321")
	fmt.Println(s5)
	s6 := same("asdfghjkl", "lkjhgfdsa")
	fmt.Println(s6)
	s7 := same("asdfghjkl", "asdfghhhh")
	fmt.Println(s7)
}