package main

import (
	"Geonardo/maker"
	"fmt"
	"log"

	"github.com/fatih/color"
)

func main() {
	for i := 0; i < 43; i++ {
		path := fmt.Sprintf("./testimg/%d.png", i)
		fmt.Printf("\n%s\n", path)

		rgba := maker.ReadFileRGBA(path)
		if err := maker.FindRoot(rgba); err != nil {
			log.Fatal(err)
		}
		if maker.PotentialDetection(rgba) {
			color.Green("[Potential] meet 9 percent requirement")
		} else {
			color.Yellow("[Potential] not over 9 percent")
		}
	}
}
