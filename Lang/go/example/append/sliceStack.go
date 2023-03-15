package Stack

func push(stack []int, v int) []int {
	stack = append(stack, v)

	return stack
}

func pop(stack []int) int {
	top := stack[len(stack)-1]
	stack = stack[:len(stack)-1]

	return top
}

