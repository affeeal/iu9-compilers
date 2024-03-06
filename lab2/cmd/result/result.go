package main

import "fmt"

var counter_cnk56nse8dehqpc44r5g int = 0

func foo() {
	strs := []string{"foo", "bar", "baz"}
	for k, str := range strs {
		fmt.Printf("Range-loop in another function: %d, %s\n", k, str)
		counter_cnk56nse8dehqpc44r5g++
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
			counter_cnk56nse8dehqpc44r5g++
		}
	}()

	for i := 0; i < n; i++ {
		for j := 0; j < n; j++ {
			fmt.Printf("Nested loop with user defined iterations: %d, %d\n", i, j)
			counter_cnk56nse8dehqpc44r5g++
		}
		counter_cnk56nse8dehqpc44r5g++
	}

	foo()

	counter := 5
	for {
		fmt.Printf("A loop with no init, cond and post statements: %d\n", counter)

		counter--
		if counter < 0 {
			break
		}
		counter_cnk56nse8dehqpc44r5g++
	}
	fmt.Printf("Total loop iterations count: %d\n", counter_cnk56nse8dehqpc44r5g)
}
