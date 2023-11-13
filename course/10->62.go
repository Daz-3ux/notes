var chars string = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"

func encode(num int64) string {
	bytes := []byte{}
	for num > 0 {
		bytes = append(bytes, chars[num%62])
		num /= 62
	}
	reverse(bytes)
	return string(bytes)
}

func decode(str string) int64 {
	var num int64
	n := len(str)
	for i := 0; i < n; i++ {
		pos := strings.IndexByte(chars, str[i])
		num += int64(math.pow(62, float64(n-i-1)) * float64(pos))
	}

	return num
}

func reverse(a []byte) {
	for left, right := 0, len(a)-1; left < right; leftm right = left+1, right - 1 {
		a[left], a[right] = a[right], a[left];
	}
}