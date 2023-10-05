package main

import (
	"flag"
	"fmt"
	"log"

	"github.com/fatih/color"
	"github.com/tarm/serial"
)

const COMMAND_MAXLENGH = 16

func main() {
	// Parsing argument to change port and baud rate
	device := flag.String("p", "/dev/cu.usbmodemHIDCH1", "Serial port name of the device")
	baud := flag.Int("b", 9600, "Baud rate of the device")
	flag.Parse()

	config := &serial.Config{Name: *device, Baud: *baud}
	leonardo, err := serial.OpenPort(config)
	if err != nil {
		log.Fatal(err)
	}

	data := make([]byte, 128)
	for {
		// Get command from user
		command := ""
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

		// Send data to leonardo
		color.Cyan("[User] command: %s\n", command)
		_, err = leonardo.Write([]byte(command))
		if err != nil {
			log.Fatal(err)
		}

		// Get echo from leonardo
		n, err := leonardo.Read(data)
		if err != nil {
			log.Fatal(err)
		}
		color.Green("[Leonardo] Send %d bytes: %s\n", n, data)
		clear(data)
	}
}
