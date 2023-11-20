package remote

import (
	"fmt"
	"time"

	"github.com/fatih/color"
	"github.com/go-vgo/robotgo"
)

const (
	FRENZY_KEY = "q"
	BURN_KEY   = "e"
	FRENZY_CD  = 420
	BURN_CD    = 1800
	CENTER_X   = 720
	CENTER_Y   = 466
)

var (
	BrowserX int
	BrowserY int
)

func FrenzyLocating() {
	fmt.Println("[Frenzy] Step1: move cursor to the browser")
	fmt.Println("[Frenzy] Step2: press 'y' to catch the position")
	if robotgo.AddEvent("y") {
		BrowserX, BrowserY = robotgo.GetMousePos()
	}
}

func FrenzyWorking() {
	time.Sleep(time.Second)
	frenzyClock := time.NewTicker(FRENZY_CD * time.Second)
	burnClock := time.NewTicker(BURN_CD * time.Second)

	// Work one time and wait
	goToGame()
	time.Sleep(time.Second)
	robotgo.KeyTap(FRENZY_KEY)
	color.Green("[Frenzy] play the skill frenzy: %v", time.Now())

	time.Sleep(time.Second)
	robotgo.KeyTap(BURN_KEY)
	color.Green("[Frenzy] play the skill burn: %v", time.Now())

	for {
		select {
		case t := <-frenzyClock.C:
			// Use the skill frenzy
			goToGame()
			time.Sleep(time.Second)
			robotgo.KeyTap(FRENZY_KEY)
			color.Green("[Frenzy] play the skill frenzy: %v", t)

		case t := <-burnClock.C:
			// Use the skill burnClock
			goToGame()
			time.Sleep(time.Second)
			robotgo.KeyTap(BURN_KEY)
			color.Green("[Frenzy] play the skill burn: %v", t)
		}
	}
}

func goToGame() {
	robotgo.Move(BrowserX, BrowserY)
	robotgo.Click()
	robotgo.Move(CENTER_X, CENTER_Y)
	robotgo.Click()
}
