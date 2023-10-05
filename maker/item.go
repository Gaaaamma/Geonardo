package maker

import (
	"fmt"

	"github.com/go-vgo/robotgo"
)

var (
	FirstConfirmX int
	FirstConfirmY int

	SecondConfirmX int
	SecondConfirmY int

	MysteryItemX int
	MysteryItemY int

	LeftTopX int
	LeftTopY int

	RightDistance int
	DownDistance  int

	ItemCountsX int
	ItemCountsY int
)

func MysteryLocating() {
	fmt.Println("[Mystery] Step1: move cursor to mystery item")
	fmt.Println("[Mystery] Step2: press 'y' to catch position")
	if robotgo.AddEvent("y") {
		x, y := robotgo.GetMousePos()
		MysteryItemX = GetLeonardoX(x)
		MysteryItemY = GetLeonardoY(y)
	}

	fmt.Println("[Mystery] Step3: move cursor to first confirm button")
	fmt.Println("[Mystery] Step4: press 'y' to catch position")
	if robotgo.AddEvent("y") {
		x, y := robotgo.GetMousePos()
		FirstConfirmX = GetLeonardoX(x)
		FirstConfirmY = GetLeonardoY(y)
	}

	fmt.Println("[Mystery] Step5: press first confirm to get second confirm button")
	fmt.Println("[Mystery] Step6: move cursor to second confirm button")
	fmt.Println("[Mystery] Step7: press 'y' to catch position")
	if robotgo.AddEvent("y") {
		x, y := robotgo.GetMousePos()
		SecondConfirmX = GetLeonardoX(x)
		SecondConfirmY = GetLeonardoY(y)
	}
}

func ItemLocating() {
	fmt.Println("[Item] Note: you must put your items as a rectangle")
	fmt.Println("[Item] Step1: move cursor to the center of left-top item")
	fmt.Println("[Item] Step2: press 'y' to catch position")
	if robotgo.AddEvent("y") {
		x, y := robotgo.GetMousePos()
		LeftTopX = GetLeonardoX(x)
		LeftTopY = GetLeonardoY(y)
	}

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

	fmt.Print("[Item] Step7: input the number of items in x coordination = ")
	fmt.Scan(&ItemCountsX)
	fmt.Print("[Item] Step8: input the number of items in x coordination = ")
	fmt.Scan(&ItemCountsY)
}

func ShowItemInfo() {
	fmt.Printf("MysteryItemX = %d\n", MysteryItemX)
	fmt.Printf("MysteryItemY = %d\n\n", MysteryItemY)
	fmt.Printf("FirstConfirmX = %d\n", FirstConfirmX)
	fmt.Printf("FirstConfirmY = %d\n\n", FirstConfirmY)
	fmt.Printf("SecondConfirmX = %d\n", SecondConfirmX)
	fmt.Printf("SecondConfirmX = %d\n\n", SecondConfirmY)
	fmt.Printf("LeftTopX = %d\n", LeftTopX)
	fmt.Printf("LeftTopY = %d\n\n", LeftTopY)
	fmt.Printf("RightDistance = %d\n", RightDistance)
	fmt.Printf("DownDistance = %d\n\n", DownDistance)
	fmt.Printf("ItemCountsX = %d\n", ItemCountsX)
	fmt.Printf("ItemCountsY = %d\n\n", ItemCountsY)
}
