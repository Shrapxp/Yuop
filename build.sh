#!/bin/bash

# Set Go version
export PATH=$PATH:/usr/local/go/bin
export GOROOT=/usr/local/go
export GOPATH=$HOME/go

# Install Go if not installed
if ! which go > /dev/null; then
    echo "Go not found, installing Go..."
    curl -Lo go1.19.linux-amd64.tar.gz https://golang.org/dl/go1.19.linux-amd64.tar.gz
    tar -C /usr/local -xvzf go1.19.linux-amd64.tar.gz
fi

# Go binary compilation
echo "Building Go binary..."
go build -o raja.exe raja.go
