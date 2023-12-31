package main

import (
	"Geonardo/maker"
	"Geonardo/remote"
	"flag"
	"fmt"
	"log"
	"time"

	"github.com/fatih/color"
	"github.com/tarm/serial"
)

const REMOTE_MODE = true
const COMMAND_MAXLENGH = 16

func main() {
	// Remote mode
	if REMOTE_MODE {
		remote.FrenzyLocating()
		remote.FrenzyWorking()
		return
	}

	// Parsing argument to change port and baud rate
	device := flag.String("p", "COM10", "Serial port name of the device")
	baud := flag.Int("b", 9600, "Baud rate of the device")
	flag.Parse()

	config := &serial.Config{Name: *device, Baud: *baud}
	leonardo, err := serial.OpenPort(config)
	if err != nil {
		log.Fatal(err)
	}

	// Locating
	maker.ItemLocating(true)
	maker.MysticLocating()
	maker.PotentialLocating()
	maker.EnchantLocating()

	// Command initialization
	maker.CommandInit()

	// Get command from user and work
	data := make([]byte, 128)
	for {
		// Get command from user
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

		if command == "cursor" { // Test cursor movement among items is correct or not
			for y := 0; y < maker.ItemCountsY; y++ {
				for x := 0; x < maker.ItemCountsX; x++ {
					maker.MoveToItem(leonardo, x, y, 200)
				}
			}

		} else if command == "reset" { // Reset config of Item, Mystic or Potential
			fmt.Print("[Geonardo] reset config (ItemT, ItemF, Mystic, Potential): ")
			if _, err := fmt.Scan(&command); err != nil {
				log.Fatal(err)
			}
			switch command {
			case "ItemT":
				maker.ItemLocating(true)
			case "ItemF":
				maker.ItemLocating(false)
			case "Mystic":
				maker.MysticLocating()
			case "Potential":
				maker.PotentialLocating()
			default:
				continue
			}
			// Need to update the command
			maker.CommandInit()

		} else if command == "work" { // A complete working procedure
			ignore := maker.SpecialToRare(leonardo, true)
			time.Sleep(3 * time.Second)
			maker.PotentialWorking(leonardo, ignore, []string{"STR", "INT", "DEX", "LUK", "ALL"}, maker.POTENTIAL_TARGET)
			time.Sleep(3 * time.Second)
			maker.EnchantWorking(leonardo, ignore, 2)

		} else if command == "create" {
			maker.Creating(leonardo, 900)

		} else if command == "weaponPHY" {
			maker.SpecialToRare(leonardo, false)
			time.Sleep(3 * time.Second)
			maker.PotentialWorking(leonardo, []int{}, []string{"PHY"}, maker.WEAPON_TARGET)

		} else if command == "weaponMAG" {
			maker.SpecialToRare(leonardo, false)
			time.Sleep(3 * time.Second)
			maker.PotentialWorking(leonardo, []int{}, []string{"MAG"}, maker.WEAPON_TARGET)

		} else if command == "1" { // Move cursor to left-top item
			color.Cyan("[User] command: move cursor to left-top item\n")
			command = fmt.Sprintf("m%d,%d\n", maker.LeftTopX, maker.LeftTopY)
			maker.LeonardoEcho(leonardo, command, data)

		} else if command == "2" { // Move cursor to right item
			color.Cyan("[User] command: move cursor to right item\n")
			command = fmt.Sprintf("r%d,%d\n", maker.RightDistance, 0)
			maker.LeonardoEcho(leonardo, command, data)

		} else if command == "3" { // Move cursor to down item
			color.Cyan("[User] command: move cursor to down item\n")
			command = fmt.Sprintf("r%d,%d\n", 0, maker.DownDistance)
			maker.LeonardoEcho(leonardo, command, data)

		} else if command == "4" { // SpecialToRare
			maker.SpecialToRare(leonardo, true)

		} else if command == "5" { // PotentialWorking with no ignore
			maker.PotentialWorking(leonardo, []int{}, []string{"STR", "INT", "DEX", "LUK", "ALL"}, maker.POTENTIAL_TARGET)

		} else if command == "0" { // Command for experiment
			// Test for StarDetection - if we can check the star counts of an item
			color.Green("[Enchant] StarDetection: %d\n", maker.StarDetection())
			// maker.EnchantWorking(leonardo, []int{}, 2)
		}
	}
}
