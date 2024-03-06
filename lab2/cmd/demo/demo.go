package main

import "fmt"

func foo() {
	strs := []string{"foo", "bar", "baz"}
	for k, str := range strs {
		fmt.Printf("Range-loop in another function: %d, %s\n", k, str)
	}
}

func main() {
	var n int

	if _, err := fmt.Scan(&n); err != nil {
		panic(err)
	}

	func() {
		for i := 0; i < 10; i++ {
			fmt.Printf("A loop in an anonymous function with pre-known iterations: %d\n", i)
		}
	}()

	for i := 0; i < n; i++ {
		for j := 0; j < n; j++ {
			fmt.Printf("Nested loop with user defined iterations: %d, %d\n", i, j)
		}
	}

	foo()
}
