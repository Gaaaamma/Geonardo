package maker

import (
	"fmt"
	"strings"
	"time"

	"github.com/tarm/serial"
)

var (
	command_toFirstItem      string
	command_toMisticItem     string
	command_toMisticConfirmA string
	command_toMisticConfirmB string
	command_toMysticCancel   string

	command_toConsume          string
	command_toPotentialCube    string
	command_toPotentialReuse   string
	command_toPotentialConfirm string
	command_toMagnifier        string
	command_toSort             string
	command_sortToStack        string
	command_toCreate           string
	command_toCreateDone       string

	command_singleClick string
	command_doubleClick string

	command_toRightItem string
	command_toDownItem  string
	// command_toLeftItem  string
	// command_toUpItem    string

	command_keyEnter string

	command_openEnchantUI    string
	command_toInventory      string
	command_toEnchantItem    string
	command_toEnchantReel    string
	command_toEnchantUse     string
	command_toEnchantConfirm string
	command_toEnchantDone    string
	command_toStarCatch      string
)

// Initialize the command (Must called after updating global settings)
func CommandInit() {
	command_toFirstItem = fmt.Sprintf("m%d,%d\n", LeftTopX, LeftTopY)
	command_toMisticItem = fmt.Sprintf("m%d,%d\n", MysticItemX, MysticItemY)
	command_toMisticConfirmA = fmt.Sprintf("m%d,%d\n", FirstConfirmX, FirstConfirmY)
	command_toMisticConfirmB = fmt.Sprintf("m%d,%d\n", SecondConfirmX, SecondConfirmY)
	command_toMysticCancel = fmt.Sprintf("m%d,%d\n", MysticCancelX, MysticCancelY)

	command_toConsume = fmt.Sprintf("m%d,%d\n", ConsumeX, ConsumeY)
	command_toPotentialCube = fmt.Sprintf("m%d,%d\n", PotentialCubeX, PotentialCubeY)
	command_toPotentialReuse = fmt.Sprintf("m%d,%d\n", PotentialReuseX, PotentialReuseY)
	command_toPotentialConfirm = fmt.Sprintf("m%d,%d\n", PotentialConfirmX, PotentialConfirmY)
	command_toMagnifier = fmt.Sprintf("m%d,%d\n", MagnifierX, MagnifierY)
	command_toSort = fmt.Sprintf("m%d,%d\n", SortX, SortY)
	command_sortToStack = fmt.Sprintf("r%d,%d\n", SORT_STACK_X, SORT_STACK_Y)
	command_toCreate = fmt.Sprintf("m%d,%d\n", CreateX, CreateY)
	command_toCreateDone = fmt.Sprintf("m%d,%d\n", CreateDoneX, CreateDoneY)

	command_singleClick = "s"
	command_doubleClick = "d"

	command_toRightItem = fmt.Sprintf("r%d,%d\n", RightDistance, 0)
	command_toDownItem = fmt.Sprintf("r%d,%d\n", 0, DownDistance)
	// command_toLeftItem = fmt.Sprintf("r%d,%d\n", -1*RightDistance, 0)
	// command_toUpItem = fmt.Sprintf("r%d,%d\n", 0, -1*DownDistance)

	command_keyEnter = "e"

	command_openEnchantUI = fmt.Sprintf("k%s", ENCHANT_SHORTCUT)
	command_toInventory = fmt.Sprintf("m%d,%d\n", InventoryX, InventoryY)
	command_toEnchantItem = fmt.Sprintf("m%d,%d\n", EnchantItemX, EnchantItemY)
	command_toEnchantReel = fmt.Sprintf("m%d,%d\n", EnchantReelX, EnchantReelY)
	command_toEnchantUse = fmt.Sprintf("m%d,%d\n", EnchantUseX, EnchantUseY)
	command_toEnchantConfirm = fmt.Sprintf("m%d,%d\n", EnchantConfirmX, EnchantConfirmY)
	command_toEnchantDone = fmt.Sprintf("m%d,%d\n", EnchantDoneX, EnchantDoneY)
	command_toStarCatch = fmt.Sprintf("m%d,%d\n", StarCatchX, StarCatchY)
}

// Move the cursor to item relative to LeftTop item
func MoveToItem(leonardo *serial.Port, x, y int, latency time.Duration) {
	// Step0. move mouse to leftTop item
	data := make([]byte, 128)
	LeonardoEcho(leonardo, command_toFirstItem, data)
	time.Sleep(time.Second)

	// Step1: Horizontal movement
	for i := 0; i < x; i++ {
		LeonardoEcho(leonardo, command_toRightItem, data)
		time.Sleep(latency * time.Millisecond)
	}

	// Step2: Vertical movement
	for i := 0; i < y; i++ {
		LeonardoEcho(leonardo, command_toDownItem, data)
		time.Sleep(latency * time.Millisecond)
	}
}

// Close mystic cube UI
func CloseMysticCube(leonardo *serial.Port) {
	commands := []string{
		command_toMysticCancel,
		command_singleClick,
	}

	data := make([]byte, 128)
	for _, command := range commands {
		LeonardoEcho(leonardo, command, data)
		time.Sleep(1000 * time.Millisecond)
	}
}

// Go to consume and double click potential cube
func InvokePotentialCube(leonardo *serial.Port, latency time.Duration) {
	commands := []string{
		command_toConsume,
		command_singleClick,
		command_toPotentialCube,
		command_doubleClick,
	}

	data := make([]byte, 128)
	for _, command := range commands {
		LeonardoEcho(leonardo, command, data)
		time.Sleep(latency * time.Millisecond)
	}
}

// Stack consume items
func StackConsumeItems(leonardo *serial.Port) {
	commands := []string{
		command_toConsume,
		command_singleClick,
		command_toSort,
		command_singleClick,
		command_sortToStack,
		command_singleClick,
	}

	data := make([]byte, 128)
	for _, command := range commands {
		LeonardoEcho(leonardo, command, data)
		time.Sleep(200 * time.Millisecond)
	}
}

// Create specific item in createDelay period
func CreateItem(leonardo *serial.Port, createDelay time.Duration) {
	commands := []string{
		command_toCreate,
		command_singleClick,
		command_keyEnter,
		command_toCreateDone, // Need to wait a moment for item done
		command_singleClick,
	}
	data := make([]byte, 128)
	for _, command := range commands {
		if strings.Compare(command, command_toCreateDone) == 0 {
			time.Sleep(10 * time.Second)
		}
		LeonardoEcho(leonardo, command, data)
		time.Sleep(createDelay * time.Millisecond)
	}
}
