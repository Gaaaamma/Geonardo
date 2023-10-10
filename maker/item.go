package maker

import (
	"fmt"

	"github.com/go-vgo/robotgo"
)

var (
	LeftTopX int
	LeftTopY int

	RightDistance int // 1434 under screen mode
	DownDistance  int // 2552 under screen mode

	ItemCountsX int
	ItemCountsY int
)

func ItemLocating(defaultDistance bool) {
	fmt.Println("[Item] Note: you must put your items as a rectangle")
	fmt.Println("[Item] Step1: move cursor to the center of left-top item")
	fmt.Println("[Item] Step2: press 'y' to catch position")
	if robotgo.AddEvent("y") {
		x, y := robotgo.GetMousePos()
		LeftTopX = GetLeonardoX(x)
		LeftTopY = GetLeonardoY(y)
	}

	if defaultDistance {
		RightDistance = 1434
		DownDistance = 2552
		fmt.Printf("[Item] Step3~6: skip distance setting with default(%d,%d)\n", RightDistance, DownDistance)

	} else {
		fmt.Println("[Item] Step3: move cursor to the center of the next right item")
		fmt.Println("[Item] Step4: press 'y' to catch position")
		if robotgo.AddEvent("y") {
			x, _ := robotgo.GetMousePos()
			x = GetLeonardoX(x)
			RightDistance = x - LeftTopX
		}

		fmt.Println("[Item] Step5: move cursor to the center of the down item of left-top item")
		fmt.Println("[Item] Step6: press 'y' to catch position")
		if robotgo.AddEvent("y") {
			_, y := robotgo.GetMousePos()
			y = GetLeonardoY(y)
			DownDistance = y - LeftTopY
		}
	}

	fmt.Print("[Item] Step7: input the number of items in x coordination = ")
	fmt.Scan(&ItemCountsX)
	fmt.Print("[Item] Step8: input the number of items in y coordination = ")
	fmt.Scan(&ItemCountsY)
}

func ShowItemInfo() {
	fmt.Printf("LeftTopX = %d\n", LeftTopX)
	fmt.Printf("LeftTopY = %d\n\n", LeftTopY)
	fmt.Printf("RightDistance = %d\n", RightDistance)
	fmt.Printf("DownDistance = %d\n\n", DownDistance)
	fmt.Printf("ItemCountsX = %d\n", ItemCountsX)
	fmt.Printf("ItemCountsY = %d\n\n", ItemCountsY)
}
