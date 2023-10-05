package maker

import (
	"fmt"
	"image"

	"github.com/go-vgo/robotgo"
)

const (
	WIN_SCREEN_X = 1919
	WIN_SCREEN_Y = 1079

	GO_SCREEN_X = 1279
	GO_SCREEN_Y = 719

	LEONARDO_SHIFT = 32767
	LEONARDO_X     = 65534 // -32767 ~ 0 ~ 32767
	LEONARDO_Y     = 65534 // -32767 ~ 0 ~ 32767
)

// func QuitListener(wg *sync.WaitGroup) {
// 	defer wg.Done()
// 	quit := robotgo.AddEvent("q")
// 	if quit {
// 		fmt.Println("[Quit] Get command of quit")
// 		return
// 	}
// }

func GetWindowsX(x int) int {
	return x * WIN_SCREEN_X / GO_SCREEN_X
}

func GetWindowsY(y int) int {
	return y * WIN_SCREEN_Y / GO_SCREEN_Y
}

func GetLeonardoX(x int) int {
	// x : ? = GO_SCREEN_X : LEONARDO_X
	result := x * LEONARDO_X / GO_SCREEN_X

	// (0,0) at center => shift
	result -= LEONARDO_SHIFT
	return result
}

func GetLeonardoY(y int) int {
	// input : ? = GO_SCREEN_Y : LEONARDO_Y
	result := y * LEONARDO_Y / GO_SCREEN_Y

	// (0,0) at center => shift
	result -= LEONARDO_SHIFT
	return result
}

func ShowRGBA(rgba *image.RGBA, width, height int) {
	for h := 0; h < height; h++ {
		for i := 0; i < width; i++ {
			fmt.Printf("[%d,%d] = %+v]\n", h, i, rgba.At(i, h))
		}
	}
}

func GetImage(x, y, width, height int, name string) {
	bit := robotgo.CaptureScreen(x, y, width, height)
	defer robotgo.FreeBitmap(bit)
	robotgo.SaveBitmap(bit, fmt.Sprintf("%s.png", name))
	// ShowRGBA(robotgo.ToRGBA(bit), width, height)
}
