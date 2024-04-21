package daily

import (
	"Geonardo/maker"
	"fmt"

	"github.com/go-vgo/robotgo"
)

var (
	GoMapStartX, GoMapStartY, GoMapEndX, GoMapEndY     int
	WinMapStartX, WinMapStartY, WinMapEndX, WinMapEndY int
	MapWidth, MapHeight                                int
)

func MapLocating() {
	fmt.Println("[Map] Step1: move cursor to left-top of the map")
	fmt.Println("[Map] Step2: press 'y' to catch start position")
	if robotgo.AddEvent("y") {
		GoMapStartX, GoMapStartY = robotgo.GetMousePos()
		WinMapStartX = maker.GetWindowsX(GoMapStartX)
		WinMapStartY = maker.GetWindowsY(GoMapStartY)
		fmt.Printf("[Start location] Go(%d, %d) => Win(%d, %d)]\n", GoMapStartX, GoMapStartY, WinMapStartX, WinMapStartY)
	}

	fmt.Println("[Map] Step3: move cursor to right-bottom of the map")
	fmt.Println("[Map] Step4: press 'y' to catch end position")
	if robotgo.AddEvent("y") {
		GoMapEndX, GoMapEndY = robotgo.GetMousePos()
		WinMapEndX = maker.GetWindowsX(GoMapEndX)
		WinMapEndY = maker.GetWindowsY(GoMapEndY)
		fmt.Printf("[End location] Go(%d, %d) => Win(%d, %d)]\n", GoMapEndX, GoMapEndY, WinMapEndX, WinMapEndY)
	}
	MapWidth = WinMapEndX - WinMapStartX
	MapHeight = WinMapEndY - WinMapStartY
}

func PlayerDetection() bool {
	maker.GetImage(WinMapStartX, WinMapStartY, MapWidth, MapHeight, "MapPosition")
	bit := robotgo.CaptureScreen(WinMapStartY, WinMapStartY, MapWidth, MapHeight)
	rgba := robotgo.ToRGBA(bit)
	robotgo.FreeBitmap(bit)

	for w := 0; w < MapWidth; w++ {
		for h := 0; h < MapHeight; h++ {
			r, g, b, _ := rgba.At(w, h).RGBA()
			r8 := (uint8)(r >> 8)
			g8 := (uint8)(g >> 8)
			b8 := (uint8)(b >> 8)
			if isPlayer(r8, g8, b8) {
				return true
			}
		}
	}
	return false
}

func isPlayer(r, g, b uint8) bool {
	return (r == 255 && g == 0 && b == 0)
}
