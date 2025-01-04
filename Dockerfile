# Use an official Python runtime as the base image
FROM python:3.9-slim

# Install Go and other required tools
RUN apt-get update && apt-get install -y \
    curl \
    build-essential \
    && curl -Lo go1.19.linux-amd64.tar.gz https://golang.org/dl/go1.19.linux-amd64.tar.gz \
    && tar -C /usr/local -xvzf go1.19.linux-amd64.tar.gz \
    && rm go1.19.linux-amd64.tar.gz

# Set Go environment variables
ENV PATH=$PATH:/usr/local/go/bin
ENV GOPATH=/go

# Set working directory
WORKDIR /app

# Copy the Go and Python code to the Docker container
COPY . .

# Install Python dependencies
RUN pip install -r requirements.txt

# Build the Go binary
RUN go build -o raja.exe raja.go

# Run the Python script
CMD python d.py
