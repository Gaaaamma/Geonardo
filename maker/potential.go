package maker

import (
	"fmt"
	"image"

	"github.com/fatih/color"
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
)

var (
	RootX         int
	RootY         int
	YelloTopLineX []int
)

func init() {
	YelloTopLineX = []int{0, 2, 6, 13}
}

// Find the reference position of ability words (STR/INT/DEX/LUK)
func FindRoot(rgba *image.RGBA) error {
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
