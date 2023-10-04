package main

import (
	"fmt"
	"log"

	"github.com/fatih/color"
	"github.com/tarm/serial"
)

const DEVICE = "/dev/cu.usbmodem1301"
const BAUD = 9600
const COMMAND_MAXLENGH = 16

func main() {
	config := &serial.Config{Name: DEVICE, Baud: BAUD}
	leonardo, err := serial.OpenPort(config)
	if err != nil {
		log.Fatal(err)
	}

	data := make([]byte, 128)
	for {
		// Get command from user
		command := ""
		_, err := fmt.Scanf("%s\n", &command)
		if err != nil {
			log.Fatal(err)
		} else if len(command) > COMMAND_MAXLENGH {
			color.Red("[Error] command is longer than %d bytes\n", COMMAND_MAXLENGH)
			continue
		}
		color.Cyan("[User] command: %s\n", command)

		// Send data to leonardo
		_, err = leonardo.Write([]byte(command))
		if err != nil {
			log.Fatal(err)
		}

		// Get echo from leonardo
		n, err := leonardo.Read(data)
		if err != nil {
			log.Fatal(err)
		}
		color.Green("[Leonardo] Get %d bytes: %s\n", n, data)
		clear(data)
	}
}
