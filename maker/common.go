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

func ReadFileRGBA(path string) {
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

	// Now that you have the RGBA image, you can display its values
	ShowRGBA(rgba, rgba.Bounds().Dx(), rgba.Bounds().Dy())
}

func ShowRGBA(rgba *image.RGBA, width, height int) {
	for h := 0; h < height; h++ {
		for i := 0; i < width; i++ {
			fmt.Printf("[%d,%d] = %+v\n", h, i, rgba.At(i, h))
		}
	}
}

func GetImage(x, y, width, height int, name string) {
	bit := robotgo.CaptureScreen(x, y, width, height)
	defer robotgo.FreeBitmap(bit)
	robotgo.SaveBitmap(bit, fmt.Sprintf("%s.png", name))
	// ShowRGBA(robotgo.ToRGBA(bit), width, height)
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
