package main

import (
	"Geonardo/maker"
	"flag"
	"fmt"
	"log"

	"github.com/fatih/color"
	"github.com/tarm/serial"
)

const COMMAND_MAXLENGH = 16

func main() {
	// Parsing argument to change port and baud rate
	device := flag.String("p", "COM10", "Serial port name of the device")
	baud := flag.Int("b", 9600, "Baud rate of the device")
	flag.Parse()

	config := &serial.Config{Name: *device, Baud: *baud}
	leonardo, err := serial.OpenPort(config)
	if err != nil {
		log.Fatal(err)
	}

	data := make([]byte, 128)
	// Money locating
	for {
		if err := maker.MoneyLocating(); err != nil {
			fmt.Println(err)
			continue
		}
		break
	}

	// Locating
	maker.MysteryLocating()
	maker.ItemLocating()
	maker.ShowItemInfo()

	for {
		// Get command from user
		command := ""
		fmt.Print("[Working] input your command: ")
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

		if command == "1" {
			// Move cursor to left-top item
			color.Cyan("[User] command: move cursor to left-top item\n")
			command = fmt.Sprintf("m%d,%d\n", maker.LeftTopX, maker.LeftTopY)
			maker.LeonardoEcho(leonardo, command, data)

		} else if command == "2" {
			// Move cursor to right item
			color.Cyan("[User] command: move cursor to right item\n")
			command = fmt.Sprintf("r%d,%d\n", maker.RightDistance, 0)
			maker.LeonardoEcho(leonardo, command, data)

		} else if command == "3" {
			// Move cursor to down item
			color.Cyan("[User] command: move cursor to down item\n")
			command = fmt.Sprintf("r%d,%d\n", 0, maker.DownDistance)
			maker.LeonardoEcho(leonardo, command, data)
		} else if command == "4" {
			// Experiment functionality
			maker.SpecialToRare(leonardo)
		}
	}
}
