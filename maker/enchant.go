package maker

import (
	"fmt"
	"image"
	"log"
	"time"

	"github.com/fatih/color"
	"github.com/go-vgo/robotgo"
	"github.com/tarm/serial"
)

const (
	ENCHANT_SHORTCUT    = "-"
	STAR_WORD_HEIGHT    = 12
	STAR_NUM_DISTANCE_X = -8
	STAR_NUM_DISTANCE_Y = 2
)

var (
	InventoryX int
	InventoryY int

	EnchantItemX  int
	EnchantItemY  int
	EnchantStartX int
	EnchantStartY int
	EnchantWidth  int
	EnchantHeight int

	EnchantReelX int
	EnchantReelY int

	EnchantUseX     int
	EnchantUseY     int
	EnchantConfirmX int
	EnchantConfirmY int
	EnchantDoneX    int
	EnchantDoneY    int

	StarRootX int
	StarRootY int
	Star      [8][]int
	Star0     [8][]int
	Star1     [8][]int
	Star2     [8][]int
	StarSlice [3][8][]int
)

func init() {
	Star = [8][]int{
		{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, // 1
		{0, 1, 8, 9},                   // 2
		{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, // 3
		{0, 1, 8, 9},                   // 4
		{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, // 5
		{4, 5},                         // 6
		{0, 1, 4, 5},                   // 7
		{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, // 8
	}
	Star0 = [8][]int{
		{1, 2, 3, 4}, // 1
		{0, 1, 4, 5}, // 2
		{0, 1, 4, 5}, // 3
		{0, 1, 4, 5}, // 4
		{0, 1, 4, 5}, // 5
		{0, 1, 4, 5}, // 6
		{0, 1, 4, 5}, // 7
		{1, 2, 3, 4}, // 8
	}
	Star1 = [8][]int{
		{2, 3},       // 1
		{1, 2, 3},    // 2
		{2, 3},       // 3
		{2, 3},       // 4
		{2, 3},       // 5
		{2, 3},       // 6
		{2, 3},       // 7
		{1, 2, 3, 4}, // 8
	}
	Star2 = [8][]int{
		{1, 2, 3, 4},       // 1
		{0, 1, 4, 5},       // 2
		{0, 1, 4, 5},       // 3
		{3, 4},             // 4
		{2, 3},             // 5
		{1, 2},             // 6
		{0, 1},             // 7
		{0, 1, 2, 3, 4, 5}, // 8
	}
	StarSlice = [3][8][]int{
		Star0,
		Star1,
		Star2,
	}
}

func EnchantWorking(leonardo *serial.Port, ignore []int, starTarget int) {
	// Open enchant UI
	data := make([]byte, 128)
	LeonardoEcho(leonardo, command_openEnchantUI, data)
	time.Sleep(time.Second)

	for j := 0; j < ItemCountsY; j++ {
		for i := 0; i < ItemCountsX; i++ {
			// Ignore item of specific index
			index := i + j*ItemCountsX
			if len(ignore) != 0 && index == ignore[0] {
				// Remove first item in ignore and continue to next round
				ignore = ignore[1:]
				continue
			}

			// Start working
			// Invoke inventory first
			LeonardoEcho(leonardo, command_toInventory, data)
			time.Sleep(100 * time.Millisecond)
			LeonardoEcho(leonardo, command_singleClick, data)
			time.Sleep(100 * time.Millisecond)

			// Move to item and put item into enchant UI
			MoveToItem(leonardo, i, j, 200)
			time.Sleep(100 * time.Millisecond)
			LeonardoEcho(leonardo, command_singleClick, data)
			time.Sleep(100 * time.Millisecond)
			LeonardoEcho(leonardo, command_toEnchantItem, data)
			time.Sleep(100 * time.Millisecond)
			LeonardoEcho(leonardo, command_singleClick, data)
			time.Sleep(100 * time.Millisecond)

			// Invoke the reel
			LeonardoEcho(leonardo, command_toEnchantReel, data)
			time.Sleep(100 * time.Millisecond)
			LeonardoEcho(leonardo, command_singleClick, data)
			time.Sleep(100 * time.Millisecond)

			// Start checking star count and do reel and star level up
			for !StarDetection(starTarget) {
				// Not yet meet the star requirement
				LeonardoEcho(leonardo, command_toEnchantUse, data)
				time.Sleep(100 * time.Millisecond)
				LeonardoEcho(leonardo, command_doubleClick, data)
				time.Sleep(100 * time.Millisecond)

				LeonardoEcho(leonardo, command_toEnchantConfirm, data)
				time.Sleep(100 * time.Millisecond)
				LeonardoEcho(leonardo, command_doubleClick, data)
				time.Sleep(1000 * time.Millisecond)

				LeonardoEcho(leonardo, command_toEnchantDone, data)
				time.Sleep(100 * time.Millisecond)
				LeonardoEcho(leonardo, command_doubleClick, data)
				time.Sleep(500 * time.Millisecond)
			}
			color.Green("[Enchant] success - star counts %d\n", starTarget)
		}
	}
}

func StarDetection(val int) bool {
	if val >= len(StarSlice) {
		log.Fatal("[Enchant] StarDetection - val must not over len(StarSlice)\n")
	}
	bit := robotgo.CaptureScreen(EnchantStartX, EnchantStartY, EnchantWidth, EnchantHeight)
	rgba := robotgo.ToRGBA(bit)
	robotgo.FreeBitmap(bit)

	for j, row := range StarSlice[val] {
		for _, index := range row {
			r, g, b, _ := rgba.At(StarRootX+index, StarRootY+j).RGBA()
			r8 := (uint8)(r >> 8)
			g8 := (uint8)(g >> 8)
			b8 := (uint8)(b >> 8)
			if !isWhite(r8, g8, b8) {
				return false
			}
		}
	}
	return true
}

func EnchantLocating() {
	fmt.Println("[Enchant] Step1: move cursor to the inventory")
	fmt.Println("[Enchant] Step2: press 'y' to catch position")
	if robotgo.AddEvent("y") {
		x, y := robotgo.GetMousePos()
		InventoryX = GetLeonardoX(x)
		InventoryY = GetLeonardoY(y)
	}

	fmt.Println("[Enchant] Step3: open enchant UI")
	fmt.Println("[Enchant] Step4: move cursor to enchant item")
	fmt.Println("[Enchant] Step5: press 'y' to catch position")
	if robotgo.AddEvent("y") {
		x, y := robotgo.GetMousePos()
		EnchantItemX = GetLeonardoX(x)
		EnchantItemY = GetLeonardoY(y)
	}

	fmt.Println("[Enchant] Step6: move cursor to reel to be used")
	fmt.Println("[Enchant] Step7: press 'y' to catch position")
	if robotgo.AddEvent("y") {
		x, y := robotgo.GetMousePos()
		EnchantReelX = GetLeonardoX(x)
		EnchantReelY = GetLeonardoY(y)
	}

	fmt.Println("[Enchant] Step8: move cursor to enchant use button")
	fmt.Println("[Enchant] Step9: press 'y' to catch position")
	if robotgo.AddEvent("y") {
		x, y := robotgo.GetMousePos()
		EnchantUseX = GetLeonardoX(x)
		EnchantUseY = GetLeonardoY(y)
	}

	fmt.Println("[Enchant] Step10: move cursor to enchant confirm button")
	fmt.Println("[Enchant] Step11: press 'y' to catch position")
	if robotgo.AddEvent("y") {
		x, y := robotgo.GetMousePos()
		EnchantConfirmX = GetLeonardoX(x)
		EnchantConfirmY = GetLeonardoY(y)
	}

	fmt.Println("[Enchant] Step12: move cursor to enchant done button")
	fmt.Println("[Enchant] Step13: press 'y' to catch position")
	if robotgo.AddEvent("y") {
		x, y := robotgo.GetMousePos()
		EnchantDoneX = GetLeonardoX(x)
		EnchantDoneY = GetLeonardoY(y)
	}

	fmt.Println("[Enchant] Note: don't forget to check the checkbox of star catch")

	for {
		fmt.Println("[Enchant] Step14: put an item with star enchant")
		fmt.Println("[Enchant] Step15: move cursor to left-top of the star enchant")
		fmt.Println("[Enchant] Step16: press 'y' to catch position")
		if robotgo.AddEvent("y") {
			x, y := robotgo.GetMousePos()
			EnchantStartX = GetWindowsX(x)
			EnchantStartY = GetWindowsY(y)
		}

		fmt.Println("[Enchant] Step15: move cursor to right-bottom of the star enchant")
		fmt.Println("[Enchant] Step16: press 'y' to catch position")
		if robotgo.AddEvent("y") {
			x, y := robotgo.GetMousePos()
			x = GetWindowsX(x)
			y = GetWindowsY(y)
			EnchantWidth = x - EnchantStartX
			EnchantHeight = y - EnchantStartY
		}

		// Find starRootX and starRootY
		if err := findStarRoot(EnchantStartX, EnchantStartY, EnchantWidth, EnchantHeight); err != nil {
			color.Red("%s", err)
			continue
		}
		break
	}
}

func findStarRoot(startX, startY, width, height int) error {
	bit := robotgo.CaptureScreen(startX, startY, width, height)
	rgba := robotgo.ToRGBA(bit)
	robotgo.FreeBitmap(bit)

	for y := 0; y < rgba.Bounds().Dy(); y++ {
		for x := 0; x < rgba.Bounds().Dx(); x++ {
			r, g, b, _ := rgba.At(x, y).RGBA()
			r8 := (uint8)(r >> 8)
			g8 := (uint8)(g >> 8)
			b8 := (uint8)(b >> 8)
			if x+STAR_NUM_DISTANCE_X >= 0 && y+STAR_WORD_HEIGHT < rgba.Bounds().Dy() && isWhite(r8, g8, b8) && starFind(rgba, x, y) { // Find "white" point and starFind success
				StarRootX = x + STAR_NUM_DISTANCE_X
				StarRootY = y + STAR_NUM_DISTANCE_Y
				return nil
			}
		}
	}
	return fmt.Errorf("[Enchant] can't find starRootX && starRootY")
}

func starFind(rgba *image.RGBA, x, y int) bool {
	if x+Star[0][len(Star[0])-1] < rgba.Bounds().Dx() && y+STAR_WORD_HEIGHT < rgba.Bounds().Dy() {
		for j, row := range Star {
			for _, index := range row {
				r, g, b, _ := rgba.At(x+index, y+j).RGBA()
				r8 := (uint8)(r >> 8)
				g8 := (uint8)(g >> 8)
				b8 := (uint8)(b >> 8)
				if !isWhite(r8, g8, b8) {
					return false
				}
			}
		}
		return true
	}
	return false
}
