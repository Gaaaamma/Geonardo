package maker

import (
	"fmt"

	"github.com/go-vgo/robotgo"
)

var (
	GoMoneyStartX int
	GoMoneyStartY int
	GoMoneyEndX   int
	GoMoneyEndY   int

	WinMoneyStartX int
	WinMoneyStartY int
	WinMoneyEndX   int
	WinMoneyEndY   int
	Width          int
	Height         int

	RareX int
	RareY int
)

func RareDetection() bool {
	bit := robotgo.CaptureScreen(WinMoneyStartX, WinMoneyStartY, Width, Height)
	rgba := robotgo.ToRGBA(bit)
	robotgo.FreeBitmap(bit)
	for i := -1; i <= 1; i++ {
		r, g, b, _ := rgba.At(RareX, RareY+i).RGBA()
		if r == 0 && g == 0 && b == 0 {
			fmt.Println("[Rare] This is a rare item")
			return true
		}
	}
	fmt.Println("[Special] This is a special item")
	return false
}

func MoneyLocating() error {
	fmt.Println("[Money] Step1: put a rare item in the mysterious block")
	fmt.Println("[Money] Step2: move cursor to left-top of the money field of mysterious block")
	fmt.Println("[Money] Step3: press 'y' to catch start position")
	if robotgo.AddEvent("y") {
		GoMoneyStartX, GoMoneyStartY = robotgo.GetMousePos()
		WinMoneyStartX = GetWindowsX(GoMoneyStartX)
		WinMoneyStartY = GetWindowsY(GoMoneyStartY)
		fmt.Printf("[Start location] Go(%d, %d) => Win(%d, %d)]\n", GoMoneyStartX, GoMoneyStartY, WinMoneyStartX, WinMoneyStartY)
	}

	fmt.Println("[Money] Step4: move cursor to right-bottom of the money field of mysterious block")
	fmt.Println("[Money] Step5: press 'y' to catch end position")
	if robotgo.AddEvent("y") {
		GoMoneyEndX, GoMoneyEndY = robotgo.GetMousePos()
		WinMoneyEndX = GetWindowsX(GoMoneyEndX)
		WinMoneyEndY = GetWindowsY(GoMoneyEndY)
		fmt.Printf("[End location] Go(%d, %d) => Win(%d, %d)]\n", GoMoneyEndX, GoMoneyEndY, WinMoneyEndX, WinMoneyEndY)
	}

	Width = WinMoneyEndX - WinMoneyStartX
	Height = WinMoneyEndY - WinMoneyStartY
	GetImage(WinMoneyStartX, WinMoneyStartY, Width, Height, "MoneyPosition")
	return rareMoneyFind()
}

func rareMoneyFind() error {
	bit := robotgo.CaptureScreen(WinMoneyStartX, WinMoneyStartY, Width, Height)
	rgba := robotgo.ToRGBA(bit)
	robotgo.FreeBitmap(bit)

	fmt.Println("[Money] start finding position of '1' in 1xxxxx ")
	for w := 0; w < Width; w++ {
		r, g, b, _ := rgba.At(w, Height/2).RGBA()
		if r == 0 && g == 0 && b == 0 {
			// Find black here -> check if it is number '1'
			ur, ug, ub, _ := rgba.At(w, Height/2-1).RGBA()
			dr, dg, db, _ := rgba.At(w, Height/2+1).RGBA()
			if ur == 0 && ug == 0 && ub == 0 && dr == 0 && dg == 0 && db == 0 {
				RareX = w
				RareY = Height / 2

				fmt.Printf("[Success] find rare first '1' at Rec[%d, %d]", RareX, RareY)
				return nil
			}
		}
	}
	return fmt.Errorf("[Fail] didn't find rare first '1'")
}
