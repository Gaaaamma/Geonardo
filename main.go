package main

import (
	"Geonardo/daily"
	"fmt"
	"log"
	"time"

	"github.com/fatih/color"
)

const COMMAND_MAXLENGH = 16
const SUPPORT_PERIOD = 5
const PLAYER_EXIST_TIMES = 4

func main() {
	// Init
	daily.MusicInit()

	for {
		command := ""
		fmt.Print("[Geonardo] input your command: ")
		_, err := fmt.Scan(&command)
		if err != nil {
			log.Fatal(err)
		} else if len(command) > COMMAND_MAXLENGH {
			color.Red("[Error] command is longer than %d bytes\n", COMMAND_MAXLENGH)
			continue
		} else if len(command) == 0 {
			color.Red("[Error] empty command")
			continue
		}

		if command == "support" { // Test cursor movement among items is correct or not
			playerTime := 0
			daily.MapLocating()
			for {
				if daily.WheelDetection() {
					daily.Notice()
				} else if daily.PlayerDetection() {
					playerTime += 1
					fmt.Printf("Player exists for %d times\n", playerTime)
					if playerTime >= PLAYER_EXIST_TIMES {
						// Alert when there is player in the map over PLAYER_EXIST_TIMES
						daily.Notice()
					} else {
						time.Sleep(SUPPORT_PERIOD * time.Second)
					}
				} else {
					playerTime = 0
					time.Sleep(SUPPORT_PERIOD * time.Second)
				}
			}
		} else if command == "music" {
			daily.Notice()
		}
	}
}
