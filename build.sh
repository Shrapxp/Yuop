#!/bin/bash

# Install Go (if it's not already installed)
echo "Checking for Go installation..."
if ! command -v go &>/dev/null; then
    echo "Go not found. Installing Go..."
    curl -Lo go1.19.linux-amd64.tar.gz https://golang.org/dl/go1.19.linux-amd64.tar.gz
    sudo tar -C /usr/local -xvzf go1.19.linux-amd64.tar.gz
    export PATH=$PATH:/usr/local/go/bin
fi

# Ensure Go version is correct
echo "Go version:"
go version

# Install Go dependencies (optional, if you have any Go modules)
echo "Checking for Go modules..."
if [ -f "go.mod" ]; then
    echo "Installing Go dependencies from go.mod..."
    go mod tidy
fi

# Install Python dependencies (if requirements.txt exists)
echo "Checking for Python requirements..."
if [ -f "requirements.txt" ]; then
    echo "Installing Python dependencies..."
    pip install -r requirements.txt
fi

# Build Go binary (raja.go)
echo "Building Go binary..."
go build -o raja.exe raja.go

# Run Python script (d.py)
echo "Running Python script..."
python3 d.py
