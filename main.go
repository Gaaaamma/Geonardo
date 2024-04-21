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
			daily.MapLocating()
			for {
				if daily.PlayerDetection() {
					// Alert when there is player in the map
					daily.Notice()
				} else {
					time.Sleep(SUPPORT_PERIOD * time.Second)
				}
			}
		} else if command == "music" {
			daily.Notice()
		}
	}
}
