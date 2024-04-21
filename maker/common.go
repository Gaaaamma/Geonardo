package maker

import (
	"fmt"
	"image"
	"log"
	"os"

	"github.com/fatih/color"
	"github.com/go-vgo/robotgo"
	"github.com/tarm/serial"
)

const (
	WIN_SCREEN_X = 1919
	WIN_SCREEN_Y = 1079

	// On Windows host
	GO_SCREEN_X = 1279
	GO_SCREEN_Y = 719

	// One MacOSX host
	// GO_SCREEN_X = 1439
	// GO_SCREEN_Y = 931

	LEONARDO_SHIFT = 32767
	LEONARDO_X     = 65534 // -32767 ~ 0 ~ 32767
	LEONARDO_Y     = 65534 // -32767 ~ 0 ~ 32767

	YELLO_R = 255
	YELLO_G = 255
	YELLO_B = 17
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

func ReadFileRGBA(path string) *image.RGBA {
	// Decode the PNG data. If reading from file, create a reader
	reader, err := os.Open(path)
	if err != nil {
		log.Fatal(err)
	}
	defer reader.Close()

	img, _, err := image.Decode(reader)
	if err != nil {
		log.Fatal(err)
	}

	// Create an RGBA image
	rgba := image.NewRGBA(img.Bounds())

	// Convert the decoded image to RGBA
	for y := rgba.Bounds().Min.Y; y < rgba.Bounds().Max.Y; y++ {
		for x := rgba.Bounds().Min.X; x < rgba.Bounds().Max.X; x++ {
			rgba.Set(x, y, img.At(x, y))
		}
	}
	return rgba
}

func ShowRGBA(rgba *image.RGBA, width, height int) {
	for y := 0; y < height; y++ {
		for x := 0; x < width; x++ {
			r, g, b, _ := rgba.At(x, y).RGBA()
			r8 := (uint8)(r >> 8)
			g8 := (uint8)(g >> 8)
			b8 := (uint8)(b >> 8)
			fmt.Printf("[%d,%d] = (%d,%d,%d)\n", x, y, r8, g8, b8)
		}
	}
}

func GetImage(x, y, width, height int, name string) {
	bit := robotgo.CaptureScreen(x, y, width, height)
	defer robotgo.FreeBitmap(bit)
	robotgo.SaveBitmap(bit, fmt.Sprintf("%s.png", name))
	ShowRGBA(robotgo.ToRGBA(bit), width, height)
}

func LeonardoEcho(leonardo *serial.Port, command string, data []byte) {
	_, err := leonardo.Write([]byte(command))
	if err != nil {
		log.Fatal(err)
	}

	// Get echo from leonardo
	n, err := leonardo.Read(data)
	if err != nil {
		log.Fatal(err)
	}
	color.Green("[Leonardo] Send %d bytes: %s\n", n, data)
	clear(data)
}

func isYellow(r, g, b uint8) bool {
	return (r == YELLO_R && g == YELLO_G && b == YELLO_B)
}

func isWhite(r, g, b uint8) bool {
	return (r == 255 && g == 255 && b == 255)
}

func isBlack(r, g, b uint8) bool {
	return (r == 0 && g == 0 && b == 0)
}
