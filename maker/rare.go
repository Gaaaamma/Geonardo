package maker

import (
	"fmt"
	"math/rand"
	"time"

	"github.com/go-vgo/robotgo"
	"github.com/tarm/serial"
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
	MoneyWidth     int
	MoneyHeight    int

	RareX int
	RareY int

	FirstConfirmX int
	FirstConfirmY int

	SecondConfirmX int
	SecondConfirmY int

	MysticItemX int
	MysticItemY int

	MagnifierX int
	MagnifierY int
)

func SpecialToRare(leonardo *serial.Port) {
	data := make([]byte, 128)
	for j := 0; j < ItemCountsY; j++ {
		for i := 0; i < ItemCountsX; i++ {
			// Step1: Move to the item
			MoveToItem(leonardo, i, j, 200)

			// Step2: Drag item to mystic cube
			LeonardoEcho(leonardo, command_singleClick, data)
			time.Sleep(time.Second)
			LeonardoEcho(leonardo, command_toMisticItem, data)
			time.Sleep(time.Second)
			LeonardoEcho(leonardo, command_singleClick, data)
			time.Sleep(time.Second)

			// Step3: Rare detection and works
			for !RareDetection() {
				// Special item
				// Step4: Move to First confirm button and click
				sleep := rand.Intn(30) + 10
				LeonardoEcho(leonardo, command_toMisticConfirmA, data)
				time.Sleep(time.Duration(sleep) * time.Millisecond)
				LeonardoEcho(leonardo, command_singleClick, data)
				time.Sleep(time.Duration(sleep) * time.Millisecond)

				// Step5: Move to Second confirm button and click
				LeonardoEcho(leonardo, command_toMisticConfirmB, data)
				time.Sleep(time.Duration(sleep) * time.Millisecond)
				LeonardoEcho(leonardo, command_singleClick, data)
				time.Sleep(900 * time.Millisecond)
			}
			fmt.Printf("[Rare] it is rare now\n")
		}
	}
}

func RareDetection() bool {
	bit := robotgo.CaptureScreen(WinMoneyStartX, WinMoneyStartY, MoneyWidth, MoneyHeight)
	rgba := robotgo.ToRGBA(bit)
	robotgo.FreeBitmap(bit)
	for i := -1; i <= 1; i++ {
		r, g, b, _ := rgba.At(RareX, RareY+i).RGBA()
		r8 := (uint8)(r >> 8)
		g8 := (uint8)(g >> 8)
		b8 := (uint8)(b >> 8)
		if isBlack(r8, g8, b8) {
			fmt.Println("[Rare] This is a rare item")
			return true
		}
	}
	fmt.Println("[Special] This is a special item")
	return false
}

// Use magnifier to check all items
func Magnifiering(leonardo *serial.Port) {
	for j := 0; j < ItemCountsY; j++ {
		for i := 0; i < ItemCountsX; i++ {
			Magnifier(leonardo, i, j)
		}
	}
}

func Magnifier(leonardo *serial.Port, x, y int) {
	data := make([]byte, 128)

	// Move cursor to magnifier and click
	LeonardoEcho(leonardo, command_toMagnifier, data)
	time.Sleep(200 * time.Millisecond)
	LeonardoEcho(leonardo, command_singleClick, data)
	time.Sleep(200 * time.Millisecond)

	// Move cursor to item (x,y)
	MoveToItem(leonardo, x, y, 200)
	time.Sleep(100 * time.Millisecond)

	// Use and confirm
	LeonardoEcho(leonardo, command_singleClick, data)
	time.Sleep(200 * time.Millisecond)
	LeonardoEcho(leonardo, command_keyEnter, data)
	time.Sleep(1000 * time.Millisecond)
}

func MysticLocating() {
	// Money locating
	for {
		if err := moneyLocating(); err != nil {
			fmt.Println(err)
			continue
		}
		break
	}

	// Mystic cube locating
	fmt.Println("[Mystic] Step1: move cursor to mystic item")
	fmt.Println("[Mystic] Step2: press 'y' to catch position")
	if robotgo.AddEvent("y") {
		x, y := robotgo.GetMousePos()
		MysticItemX = GetLeonardoX(x)
		MysticItemY = GetLeonardoY(y)
	}

	fmt.Println("[Mystic] Step3: move cursor to first confirm button")
	fmt.Println("[Mystic] Step4: press 'y' to catch position")
	if robotgo.AddEvent("y") {
		x, y := robotgo.GetMousePos()
		FirstConfirmX = GetLeonardoX(x)
		FirstConfirmY = GetLeonardoY(y)
	}

	fmt.Println("[Mystic] Step5: press first confirm to get second confirm button")
	fmt.Println("[Mystic] Step6: move cursor to second confirm button")
	fmt.Println("[Mystic] Step7: press 'y' to catch position")
	if robotgo.AddEvent("y") {
		x, y := robotgo.GetMousePos()
		SecondConfirmX = GetLeonardoX(x)
		SecondConfirmY = GetLeonardoY(y)
	}

	fmt.Println("[Potential] Step8: move cursor to magnifier")
	fmt.Println("[Potential] Step9: press 'y' to catch position")
	if robotgo.AddEvent("y") {
		x, y := robotgo.GetMousePos()
		MagnifierX = GetLeonardoX(x)
		MagnifierY = GetLeonardoY(y)
	}
}

func moneyLocating() error {
	fmt.Println("[Money] Step1: put a rare item in the mystic cube")
	fmt.Println("[Money] Step2: move cursor to left-top of the money field of mystic cube")
	fmt.Println("[Money] Step3: press 'y' to catch start position")
	if robotgo.AddEvent("y") {
		GoMoneyStartX, GoMoneyStartY = robotgo.GetMousePos()
		WinMoneyStartX = GetWindowsX(GoMoneyStartX)
		WinMoneyStartY = GetWindowsY(GoMoneyStartY)
		fmt.Printf("[Start location] Go(%d, %d) => Win(%d, %d)]\n", GoMoneyStartX, GoMoneyStartY, WinMoneyStartX, WinMoneyStartY)
	}

	fmt.Println("[Money] Step4: move cursor to right-bottom of the money field of mystic cube")
	fmt.Println("[Money] Step5: press 'y' to catch end position")
	if robotgo.AddEvent("y") {
		GoMoneyEndX, GoMoneyEndY = robotgo.GetMousePos()
		WinMoneyEndX = GetWindowsX(GoMoneyEndX)
		WinMoneyEndY = GetWindowsY(GoMoneyEndY)
		fmt.Printf("[End location] Go(%d, %d) => Win(%d, %d)]\n", GoMoneyEndX, GoMoneyEndY, WinMoneyEndX, WinMoneyEndY)
	}

	MoneyWidth = WinMoneyEndX - WinMoneyStartX
	MoneyHeight = WinMoneyEndY - WinMoneyStartY
	GetImage(WinMoneyStartX, WinMoneyStartY, MoneyWidth, MoneyHeight, "MoneyPosition")
	return rareMoneyFind()
}

func rareMoneyFind() error {
	bit := robotgo.CaptureScreen(WinMoneyStartX, WinMoneyStartY, MoneyWidth, MoneyHeight)
	rgba := robotgo.ToRGBA(bit)
	robotgo.FreeBitmap(bit)

	fmt.Println("[Money] start finding position of '1' in 1xxxxx ")
	for w := 0; w < MoneyWidth; w++ {
		r, g, b, _ := rgba.At(w, MoneyHeight/2).RGBA()
		r8 := (uint8)(r >> 8)
		g8 := (uint8)(g >> 8)
		b8 := (uint8)(b >> 8)
		if isBlack(r8, g8, b8) {
			// Find black here -> check if it is number '1'
			ur, ug, ub, _ := rgba.At(w, MoneyHeight/2-1).RGBA()
			dr, dg, db, _ := rgba.At(w, MoneyHeight/2+1).RGBA()

			ur8 := (uint8)(ur >> 8)
			ug8 := (uint8)(ug >> 8)
			ub8 := (uint8)(ub >> 8)
			dr8 := (uint8)(dr >> 8)
			dg8 := (uint8)(dg >> 8)
			db8 := (uint8)(db >> 8)
			if isBlack(ur8, ug8, ub8) && isBlack(dr8, dg8, db8) {
				RareX = w
				RareY = MoneyHeight / 2

				fmt.Printf("[Success] find rare first '1' at Rec[%d, %d]\n", RareX, RareY)
				return nil
			}
		}
	}
	return fmt.Errorf("[Fail] didn't find rare first '1'")
}
