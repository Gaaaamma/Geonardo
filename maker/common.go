package maker

import (
	"fmt"
	"image"

	"github.com/go-vgo/robotgo"
)

const (
	WIN_SCREEN_X = 1920
	WIN_SCREEN_Y = 1080

	GO_SCREEN_X = 1280
	GO_SCREEN_Y = 720

	LEONARDO_SHIFT = 32767
	LEONARDO_SIZE  = 65535 // -32767 ~ 0 ~ 32767
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

func GetLeonardoPosition(input int) int {
	//TODO
	return -1
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
