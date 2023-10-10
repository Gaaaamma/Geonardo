package maker

import (
	"fmt"
	"image"
	"log"

	"github.com/fatih/color"
	"github.com/go-vgo/robotgo"
	"github.com/tarm/serial"
)

// STR
// INT
// DEX
// LUX
// 全

const (
	EN_HEIGHT           = 8
	ZN_HEIGHT           = 12
	EN_DISTANCE         = 6
	RARE_FIRST_DISTANCE = 4
	POTENTIAL_TARGET    = 9
)

var (
	// Potential setting
	ConsumeX          int
	ConsumeY          int
	PotentialCubeX    int
	PotentialCubeY    int
	PotentialReuseX   int
	PotentialReuseY   int
	PotentialConfirmX int
	PotentialConfirmY int

	PotentialStartX int
	PotentialStartY int
	PotentialWidth  int
	PotentialHeight int

	RootX int
	RootY int

	YelloTopLineX []int
	STR           [8][]int
	INT           [8][]int
	DEX           [8][]int
	LUK           [8][]int
	ALL           [8][]int
	THREE         [8][]int
	SIX           [8][]int
)

func init() {
	YelloTopLineX = []int{0, 2, 6, 13}
	STR = [8][]int{
		{1, 2, 3, 4, 6, 7, 8, 9, 10, 12, 13, 14, 15},
		{0, 4, 6, 8, 10, 13, 16},
		{0, 8, 13, 16},
		{1, 2, 8, 13, 14, 15},
		{3, 8, 13, 15},
		{4, 8, 13, 16},
		{0, 4, 8, 13, 16},
		{0, 1, 2, 3, 7, 8, 9, 12, 13, 14, 16, 17},
	}
	INT = [8][]int{
		{0, 1, 2, 3, 4, 6, 7, 9, 10, 11, 12, 13, 14, 15, 16},
		{2, 7, 10, 12, 14, 16},
		{2, 7, 8, 10, 14},
		{2, 7, 8, 10, 14},
		{2, 7, 9, 10, 14},
		{2, 7, 9, 10, 14},
		{2, 7, 10, 14},
		{0, 1, 2, 3, 4, 6, 7, 8, 10, 13, 14, 15},
	}
	DEX = [8][]int{
		{0, 1, 2, 3, 6, 7, 8, 9, 10, 12, 13, 15, 16},
		{1, 4, 7, 10, 13, 15},
		{1, 4, 7, 9, 13, 15},
		{1, 4, 7, 8, 9, 14},
		{1, 4, 7, 9, 14},
		{1, 4, 7, 13, 15},
		{1, 4, 7, 10, 13, 15},
		{0, 1, 2, 3, 6, 7, 8, 9, 10, 12, 13, 15, 16},
	}
	LUK = [8][]int{
		{0, 1, 2, 6, 7, 10, 11, 12, 13, 14, 16, 17},
		{1, 7, 10, 13, 16},
		{1, 7, 10, 13, 15},
		{1, 7, 10, 13, 14},
		{1, 7, 10, 13, 15},
		{1, 7, 10, 13, 16},
		{1, 5, 7, 10, 13, 16},
		{0, 1, 2, 3, 4, 5, 8, 9, 12, 13, 14, 16, 17},
	}
	ALL = [8][]int{
		{3, 7, 61, 62, 63, 67, 70},
		{2, 8, 60, 64, 66, 68, 70},
		{0, 1, 9, 10, 64, 66, 68, 69},
		{2, 3, 4, 5, 6, 7, 8, 62, 63, 66, 68, 70},
		{5, 64, 67, 68, 69, 71},
		{5, 64, 68, 69, 71},
		{1, 2, 3, 4, 5, 6, 7, 8, 9, 60, 64, 67, 69, 71},
		{5, 61, 62, 63, 67, 70},
	}
	THREE = [8][]int{
		{43, 44, 45, 49, 52},
		{42, 46, 48, 50, 52},
		{46, 48, 50, 51},
		{44, 45, 48, 50, 52},
		{46, 49, 50, 51, 53},
		{46, 50, 51, 53},
		{42, 46, 49, 51, 53},
		{43, 44, 45, 49, 52},
	}
	SIX = [8][]int{
		{44, 45, 49, 52},
		{43, 46, 48, 50, 52},
		{42, 48, 50, 51},
		{42, 44, 45, 48, 50, 52},
		{42, 43, 46, 49, 50, 51, 53},
		{42, 46, 50, 51, 53},
		{42, 46, 49, 51, 53},
		{43, 44, 45, 49, 52},
	}
}

// Potential main working function
func PotentialWorking(leonardo *serial.Port) {

}

// Detect potential result meet requirements or not
func PotentialDetection() bool {
	bit := robotgo.CaptureScreen(PotentialStartX, PotentialStartY, PotentialWidth, PotentialHeight)
	rgba := robotgo.ToRGBA(bit)
	robotgo.FreeBitmap(bit)

	potential := make(map[string]int)
	attrs := []string{"STR", "INT", "DEX", "LUK", "ALL"}

	for i := 0; i < 3; i++ {
		rootX := RootX
		rootY := RootY + i*(EN_HEIGHT+EN_DISTANCE)

		// Check the attribute of each row
		for _, attr := range attrs {
			value, err := attributeDetection(rootX, rootY, rgba, attr)
			if err != nil {
				log.Fatal(err)
			}
			potential[attr] += value
		}
	}

	// Check if this item meet the target
	attr := ""
	maxPotential := 0
	for k, v := range potential {
		if k != "ALL" && v > maxPotential {
			attr = k
			maxPotential = v
		}
	}
	maxPotential += potential["ALL"]

	if maxPotential >= POTENTIAL_TARGET {
		color.Green("[Potential] %s: %+v\n", attr, potential)
		return true
	} else {
		color.Yellow("[Potential] %+v\n", potential)
		return false
	}
}

// Potential locating flow called by user
func PotentialLocating() {
	for {
		potentialLocating()
		if err := findRoot(); err != nil {
			color.Red("%s", err)
			continue
		}
		break
	}
}

// Used to check the word position
func WordCheck(word [8][]int) {
	maxLength := 0
	for i := 0; i < len(word); i++ {
		maxLength = max(maxLength, word[i][len(word[i])-1])
	}
	fmt.Println("")
	for i := 0; i < len(word); i++ {
		counter := 0
		for j := 0; j <= maxLength; j++ {
			if counter >= len(word[i]) || j != word[i][counter] {
				fmt.Print(" ")
			} else {
				fmt.Print("=")
				counter += 1
			}
		}
		fmt.Println()
	}
	fmt.Println("")
}

// Set potential cube position
func potentialLocating() {
	fmt.Println("[Potential] Step1: move cursor to consume column")
	fmt.Println("[Potential] Step2: press 'y' to catch position")
	if robotgo.AddEvent("y") {
		x, y := robotgo.GetMousePos()
		ConsumeX = GetLeonardoX(x)
		ConsumeY = GetLeonardoY(y)
	}

	fmt.Println("[Potential] Step3: move cursor to potential cube")
	fmt.Println("[Potential] Step4: press 'y' to catch position")
	if robotgo.AddEvent("y") {
		x, y := robotgo.GetMousePos()
		PotentialCubeX = GetLeonardoX(x)
		PotentialCubeY = GetLeonardoY(y)
	}

	fmt.Println("[Potential] Step5: use cube to get first potential result")
	fmt.Println("[Potential] Step6: move cursor to potential reuse button")
	fmt.Println("[Potential] Step7: press 'y' to catch position")
	if robotgo.AddEvent("y") {
		x, y := robotgo.GetMousePos()
		PotentialReuseX = GetLeonardoX(x)
		PotentialReuseY = GetLeonardoY(y)
	}

	fmt.Println("[Potential] Step8: move cursor to potential confirm button")
	fmt.Println("[Potential] Step9: press 'y' to catch position")
	if robotgo.AddEvent("y") {
		x, y := robotgo.GetMousePos()
		PotentialConfirmX = GetLeonardoX(x)
		PotentialConfirmY = GetLeonardoY(y)
	}

	fmt.Println("[Potential] Step10: move cursor to left-top of the result")
	fmt.Println("[Potential] Step11: press 'y' to catch position")
	if robotgo.AddEvent("y") {
		x, y := robotgo.GetMousePos()
		PotentialStartX = GetWindowsX(x)
		PotentialStartY = GetWindowsY(y)
	}

	fmt.Println("[Potential] Step12: move cursor to right-bottom of the result")
	fmt.Println("[Potential] Step13: press 'y' to catch position")
	if robotgo.AddEvent("y") {
		x, y := robotgo.GetMousePos()
		x = GetWindowsX(x)
		y = GetWindowsY(y)
		PotentialWidth = x - PotentialStartX
		PotentialHeight = y - PotentialStartY
	}

	fmt.Println("[Potential] Step14: confirm position of image 'potential.png' is correct")
	GetImage(PotentialStartX, PotentialStartY, PotentialWidth, PotentialHeight, "potential")
}

// Find the reference position of attribute words (STR/INT/DEX/LUK)
func findRoot() error {
	bit := robotgo.CaptureScreen(PotentialStartX, PotentialStartY, PotentialWidth, PotentialHeight)
	rgba := robotgo.ToRGBA(bit)
	robotgo.FreeBitmap(bit)

	if !findRootX(rgba) {
		return fmt.Errorf("[Potential] find RootX failed")
	}
	if !findRootY(rgba) {
		return fmt.Errorf("[Potential] find RootY failed")
	}
	color.Green("[Potential] find (RootX, RootY) = (%d, %d)", RootX, RootY)
	return nil
}

// Use white point to find the position of RootX.
// X must not equals to 0 which represents user cut a bad graph
func findRootX(rgba *image.RGBA) bool {
	for x := 0; x < rgba.Bounds().Dx(); x++ {
		for y := 0; y < rgba.Bounds().Dy(); y++ {
			r, g, b, _ := rgba.At(x, y).RGBA()
			r8 := (uint8)(r >> 8)
			g8 := (uint8)(g >> 8)
			b8 := (uint8)(b >> 8)

			if isWhite(r8, g8, b8) { // Find "white" point
				// Further check x must not equals to 0 (represent user cut a bad graph to be identified)
				RootX = x
				return x != 0
			}
		}
	}
	return false
}

// Use top line of yellow words ("稀有") to find the position of RootY.
// User must cut the whole word ("稀有") or the function will fail
func findRootY(rgba *image.RGBA) bool {
	for y := 0; y < rgba.Bounds().Dy(); y++ {
		for x := 0; x < rgba.Bounds().Dx(); x++ {
			r, g, b, _ := rgba.At(x, y).RGBA()
			r8 := (uint8)(r >> 8)
			g8 := (uint8)(g >> 8)
			b8 := (uint8)(b >> 8)

			if isYellow(r8, g8, b8) { // Find "yellow" point
				// Need to further check it is the top line of "稀有"
				if x+YelloTopLineX[len(YelloTopLineX)-1] >= rgba.Bounds().Dx() || y+1 >= rgba.Bounds().Dy() {
					return false
				}
				for i := 0; i < len(YelloTopLineX); i++ {
					r, g, b, _ = rgba.At(x+YelloTopLineX[i], y).RGBA()
					r8 := (uint8)(r >> 8)
					g8 := (uint8)(g >> 8)
					b8 := (uint8)(b >> 8)
					if !isYellow(r8, g8, b8) {
						return false
					}
				}
				r, g, b, _ = rgba.At(x+YelloTopLineX[len(YelloTopLineX)-1], y+1).RGBA()
				r8 := (uint8)(r >> 8)
				g8 := (uint8)(g >> 8)
				b8 := (uint8)(b >> 8)
				RootY = y + ZN_HEIGHT + RARE_FIRST_DISTANCE
				return isYellow(r8, g8, b8)
			}
		}
	}
	return false
}

func attributeDetection(rootX, rootY int, rgba *image.RGBA, attr string) (int, error) {
	// Attribute selection
	attribute := [8][]int{}
	switch attr {
	case "STR":
		attribute = STR
	case "INT":
		attribute = INT
	case "DEX":
		attribute = DEX
	case "LUK":
		attribute = LUK
	case "ALL":
		attribute = ALL
	default:
		return 0, fmt.Errorf("[Error] unknown attribute: %s", attr)
	}

	// Check attribute first
	for h := 0; h < len(attribute); h++ {
		for _, w := range attribute[h] {
			r, g, b, _ := rgba.At(rootX+w, rootY+h).RGBA()
			r8 := (uint8)(r >> 8)
			g8 := (uint8)(g >> 8)
			b8 := (uint8)(b >> 8)
			if !isWhite(r8, g8, b8) {
				return 0, nil
			}
		}
	}

	// Check percentage second
	if attr == "ALL" {
		return 3, nil
	} else {
		percent, err := percentDetection(rootX, rootY, rgba, 6)
		if percent != 0 || err != nil {
			return percent, err
		}
		percent, err = percentDetection(rootX, rootY, rgba, 3)
		if percent != 0 || err != nil {
			return percent, err
		}
	}
	return 0, nil
}

func percentDetection(rootX, rootY int, rgba *image.RGBA, percent int) (int, error) {
	// Percent selection
	verification := [8][]int{}
	switch percent {
	case 3:
		verification = THREE
	case 6:
		verification = SIX
	default:
		return 0, fmt.Errorf("[Error] unknown percent: %d", percent)
	}

	for h := 0; h < len(verification); h++ {
		for _, w := range verification[h] {
			r, g, b, _ := rgba.At(rootX+w, rootY+h).RGBA()
			r8 := (uint8)(r >> 8)
			g8 := (uint8)(g >> 8)
			b8 := (uint8)(b >> 8)
			if !isWhite(r8, g8, b8) {
				return 0, nil
			}
		}
	}
	return percent, nil
}
