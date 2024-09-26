#include "NTPClient.hpp"
#include <chrono>
#include <thread>
#include <iomanip>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sstream>

#define NTP_TIMESTAMP_DELTA 2208988800ull

struct ntp_packet { 
    uint8_t li_vn_mode;      // Eight bits. li, vn, and mode.
    uint8_t stratum;         // Eight bits. Stratum level of the local clock.
    uint8_t poll;            // Eight bits. Maximum interval between successive messages.
    uint8_t precision;       // Eight bits. Precision of the local clock.
    uint32_t rootDelay;      // 32 bits. Total round trip delay time.
    uint32_t rootDispersion; // 32 bits. Max error allowed from primary clock source.
    uint32_t refId;          // 32 bits. Reference clock identifier.
    uint32_t refTm_s;        // 32 bits. Reference time-stamp seconds.
    uint32_t refTm_f;        // 32 bits. Reference time-stamp fraction of a second.
    uint32_t origTm_s;       // 32 bits. Originate time-stamp seconds.
    uint32_t origTm_f;       // 32 bits. Originate time-stamp fraction of a second.
    uint32_t rxTm_s;         // 32 bits. Received time-stamp seconds.
    uint32_t rxTm_f;         // 32 bits. Received time-stamp fraction of a second.
    uint32_t txTm_s;         // 32 bits. Transmit time-stamp seconds.
    uint32_t txTm_f;         // 32 bits. Transmit time-stamp fraction of a second.
};

NTPClient::NTPClient() {
    syncNTP();  // Initialize and sync time with the NTP server
}

void NTPClient::syncNTP() {
    for (const std::string &ip : serverIPs) {
        currentTime = getNTPTime(ip);
        if (currentTime != -1) {
            break;  // Successfully synced time
        }
    }
}

std::time_t NTPClient::getNTPTime(const std::string &serverIP) {
    int sockfd;
    struct sockaddr_in server_addr;
    struct ntp_packet packet = {0};
    
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) {
        std::cerr << "Error creating socket\n";
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(123);  // NTP uses port 123

    if (inet_pton(AF_INET, serverIP.c_str(), &server_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address: " << serverIP << "\n";
        return -1;
    }

    packet.li_vn_mode = 0x1B;  // LI = 0, VN = 3, Mode = 3 (client)
    
    if (sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Error sending packet\n";
        return -1;
    }

    socklen_t addr_len = sizeof(server_addr);
    if (recvfrom(sockfd, &packet, sizeof(packet), 0, (struct sockaddr*)&server_addr, &addr_len) < 0) {
        std::cerr << "Error receiving packet\n";
        return -1;
    }

    close(sockfd);

    packet.txTm_s = ntohl(packet.txTm_s);  // Convert to host byte order
    time_t txTm = (time_t)(packet.txTm_s - NTP_TIMESTAMP_DELTA);  // NTP time is seconds since 1900, UNIX since 1970

    return txTm;
}

std::string NTPClient::getDate() {
    std::tm *localTime = std::localtime(&currentTime);
    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << localTime->tm_mday << "/"
       << std::setw(2) << std::setfill('0') << (localTime->tm_mon + 1) << "/"
       << (localTime->tm_year + 1900);
    return ss.str();
}

long NTPClient::getTimeInSeconds() {
    return static_cast<long>(currentTime);
}

int NTPClient::getHours() {
    std::tm *localTime = std::localtime(&currentTime);
    return localTime->tm_hour;
}

int NTPClient::getMinutes() {
    std::tm *localTime = std::localtime(&currentTime);
    return localTime->tm_min;
}

std::string NTPClient::getDayOfWeek() {
    std::tm *localTime = std::localtime(&currentTime);
    const char* days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    return days[localTime->tm_wday];
}

std::string NTPClient::getMonth() {
    std::tm *localTime = std::localtime(&currentTime);
    const char* months[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    return months[localTime->tm_mon];
}

// display readable time 
int NTPClient::getSeconds() {
    std::tm *localTime = std::localtime(&currentTime);
    return localTime->tm_sec;
}

void NTPClient::displayReadableTime() {
    std::cout << "Current Time Information:\n";
    
    printWithTypewriterEffect("Date", getDate(), 36);  // Cyan color
    printWithTypewriterEffect("Time in seconds", std::to_string(getTimeInSeconds()), 32);  // Green color
    printWithTypewriterEffect("Hours", std::to_string(getHours()), 33);  // Yellow color
    printWithTypewriterEffect("Minutes", std::to_string(getMinutes()), 34);  // Blue color
    printWithTypewriterEffect("Seconds", std::to_string(getSeconds()), 35);  // Magenta color
    printWithTypewriterEffect("Day of the Week", getDayOfWeek(), 37);  // White color
    printWithTypewriterEffect("Month", getMonth(), 31);  // Red color
}


void NTPClient::displayMenu() {
    const std::string menu[] = {
        "1: Get Date (dd/mm/yyyy)",
        "2: Get Time in seconds",
        "3: Get Hours",
        "4: Get Minutes",
        "5: Get Seconds",
        "6: Get Day (text format)",
        "7: Get Month",
        "8: Display All Time Information"
    };

    int colors[] = {36, 32, 33, 34, 35, 37, 31, 38};  // Different colors for each option

    for (size_t i = 0; i < sizeof(menu) / sizeof(menu[0]); ++i) {
        typewriterPrintResult(menu[i], colors[i % (sizeof(colors) / sizeof(colors[0]))]);
        std::cout << std::endl;  // New line after each menu item
    }
}

// New method for typewriter output of results
void NTPClient::typewriterPrintResult(const std::string &text, int colorCode) {
    
    
    
    std::cout << "\033[" << colorCode << "m";  // Set text color
    for (char c : text) {
        std::cout << c;  // Print one character
        std::cout.flush();  // Ensure the character is printed immediately
        std::this_thread::sleep_for(std::chrono::milliseconds(30));  // Adjust speed as needed
    }
    std::cout << "\033[0m";  // Reset text color
}

void NTPClient::typewriterPrint(const std::string &text, int colorCode) {
    std::cout << "\033[" << colorCode << "m";  // Set text color
    for (char c : text) {
        std::cout << c;
        std::this_thread::sleep_for(std::chrono::milliseconds(30));  // Simulate typewriter effect
    }
    std::cout << "\033[0m";  // Reset text color
}

void NTPClient::printWithTypewriterEffect(const std::string &label, const std::string &value, int colorCode) {
     
      // Combine label and value with a newline at the end
    std::string finalString = label + ": " + value + "\n";  // Add a newline character

    std::cout << "\033[" << colorCode << "m";  // Set text color
    for (char c : finalString) {
        std::cout << c;  // Print one character
        std::cout.flush();  // Ensure the character is printed immediately
        std::this_thread::sleep_for(std::chrono::milliseconds(30));  // Adjust speed as needed
    }
    std::cout << "\033[0m";  // Reset text color
   
   
 }
