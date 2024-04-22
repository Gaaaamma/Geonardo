package daily

import (
	"Geonardo/maker"
	"fmt"
	"log"
	"os"
	"time"

	"github.com/faiface/beep"
	"github.com/faiface/beep/mp3"
	"github.com/faiface/beep/speaker"
	"github.com/go-vgo/robotgo"
)

var (
	GoMapStartX, GoMapStartY, GoMapEndX, GoMapEndY     int
	WinMapStartX, WinMapStartY, WinMapEndX, WinMapEndY int
	MapWidth, MapHeight                                int
	File                                               *os.File
	streamer                                           beep.StreamSeekCloser
	format                                             beep.Format
	WheelX, WheelY                                     int
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
	bit := robotgo.CaptureScreen(WinMapStartX, WinMapStartY, MapWidth, MapHeight)
	rgba := robotgo.ToRGBA(bit)
	robotgo.FreeBitmap(bit)

	for h := 0; h < MapHeight; h++ {
		for w := 0; w < MapWidth; w++ {
			r, g, b, _ := rgba.At(w, h).RGBA()
			r8 := (uint8)(r >> 8)
			g8 := (uint8)(g >> 8)
			b8 := (uint8)(b >> 8)
			// fmt.Printf("[%d,%d] = (%d,%d,%d)\n", h, w, r8, g8, b8)
			if isPlayer(r8, g8, b8) {
				fmt.Println(r8, g8, b8)
				fmt.Println("Find player")
				return true
			}
		}
	}
	return false
}

func WheelDetection() bool {
	maker.GetImage(WinMapStartX, WinMapStartY, MapWidth, MapHeight, "MapPosition")
	bit := robotgo.CaptureScreen(WinMapStartX, WinMapStartY, MapWidth, MapHeight)
	rgba := robotgo.ToRGBA(bit)
	robotgo.FreeBitmap(bit)

	for h := 0; h < MapHeight; h++ {
		for w := 0; w < MapWidth; w++ {
			r, g, b, _ := rgba.At(w, h).RGBA()
			r8 := (uint8)(r >> 8)
			g8 := (uint8)(g >> 8)
			b8 := (uint8)(b >> 8)
			// fmt.Printf("[%d,%d] = (%d,%d,%d)\n", h, w, r8, g8, b8)
			if isWheel(r8, g8, b8) {
				fmt.Println(r8, g8, b8)
				WheelX = w
				WheelY = h
				fmt.Printf("Find wheel at (%v, %v)\n", WheelX, WheelY)
				return true
			}
		}
	}
	WheelX, WheelY = -1, -1
	return false
}

func MusicInit() {
	// Open the MP3 file
	File, _ = os.Open("test.mp3")

	// Decode the MP3 file
	streamer, format, _ = mp3.Decode(File)

	// Initialize the speaker
	speaker.Init(format.SampleRate, format.SampleRate.N(time.Second/10))
}

func Notice() {
	fmt.Println("start!")
	done := make(chan bool)

	// Play the audio stream
	speaker.Play(beep.Seq(streamer, beep.Callback(func() {
		done <- true
	})))

	// Wait for playback to finish
	<-done
	fmt.Println("done")

	// Rewind the file to the beginning
	_, err := File.Seek(0, 0)
	if err != nil {
		log.Fatal(err)
	}
}
func isPlayer(r, g, b uint8) bool {
	return (r == 255 && g == 0 && b == 0)
}
func isWheel(r, g, b uint8) bool {
	return (r == 221 && g == 102 && b == 255)
}
