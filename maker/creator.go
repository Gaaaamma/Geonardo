package maker

import (
	"fmt"
	"time"

	"github.com/fatih/color"
	"github.com/go-vgo/robotgo"
	"github.com/tarm/serial"
)

var (
	CreateX     int
	CreateY     int
	CreateDoneX int
	CreateDoneY int
)

func creatorLocating() {
	fmt.Println("[Creator] Step1: move cursor to create button")
	fmt.Println("[Creator] Step2: press 'y' to catch position")
	if robotgo.AddEvent("y") {
		x, y := robotgo.GetMousePos()
		CreateX = GetLeonardoX(x)
		CreateX = GetLeonardoY(y)
	}

	fmt.Println("[Creator] Step3: create this item to get create done button")
	fmt.Println("[Creator] Step4: move cursor to create done button")
	fmt.Println("[Creator] Step5: press 'y' to catch position")
	if robotgo.AddEvent("y") {
		x, y := robotgo.GetMousePos()
		CreateDoneX = GetLeonardoX(x)
		CreateDoneY = GetLeonardoY(y)
	}
}

func Creating(leonardo *serial.Port, period time.Duration) {
	fmt.Println("[Creator] Step1: move cursor to create button")
	fmt.Println("[Creator] Step2: press 'y' to catch position")
	if robotgo.AddEvent("y") {
		x, y := robotgo.GetMousePos()
		CreateX = GetLeonardoX(x)
		CreateX = GetLeonardoY(y)
	}

	fmt.Println("[Creator] Step3: create this item to get create done button")
	fmt.Println("[Creator] Step4: move cursor to create done button")
	fmt.Println("[Creator] Step5: press 'y' to catch position")
	if robotgo.AddEvent("y") {
		x, y := robotgo.GetMousePos()
		CreateDoneX = GetLeonardoX(x)
		CreateDoneY = GetLeonardoY(y)
	}

	fmt.Println("[Creator] Step6: make sure you are at correct item to be created")
	fmt.Println("[Creator] Step7: input the number of items to be created")
	counts := 0
	fmt.Scan(&counts)
	if counts > 0 && counts < 100 {
		for i := 0; i < counts; i++ {
			CreateItem(leonardo, 500)
			time.Sleep(period*time.Second + 10*time.Second)
		}
	} else {
		color.Red("[Creator] invalid number of items: %d", counts)
	}
}
