package geometry

import (
	"math"
)

type Point struct{ X, Y float64 }

// traditional function
func Distance(p, q Point) float64 {
	return math.Hypot(q.X-p.X, q.Y-p.Y)
}

// same thing, but as a method of the Ponit type
// p 是方法的接收器
func (p Point) Distance(q Point) float64 {
	return math.Hypot(q.X-p.X, q.Y-p.Y)
}

/*
p := Point{1, 2}
q := Point{3, 4}
// 调用 包级别的函数geometry.Distance
fmt.Println(Distance(p, q)) // "5", function call
// 调用 Point类下声明的Point.Distance方法
fmt.Println(p.Distance(q))  // "5", method call
*/