# Unsafe code
- 一种可以绕过 Go 类型安全和内存安全检查的 Go 代码
- 任何 Go 指针都可以转化为 `unsafe.Pointer` 指针
  - unsafe.Pointer 类型的指针可以覆盖掉 Go 的系统类型。这毫无疑问很快，但是如果不小心或者不正确使用的话就会很危险，它给了开发者更多选择去掌控数据

# Unsafe 包
```go
package unsafe
type ArbitraryType int
type Pointer *ArbitraryType
func Sizeof(x ArbitraryType) uintptr
func Offsetof(x ArbitraryType) uintptr
func Alignof(x ArbitraryType) uintptr
```
> OK，其它的unsafe包的go代码去哪里了？答案很简单：当你import到你程序里的时候，Go编译器实现了这个unsafe库。
- 当在程序中导入`unsafe`包时,实际上是在引入编译器对该包的实现