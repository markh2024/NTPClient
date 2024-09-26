#ifndef NTPCLIENT_H
#define NTPCLIENT_H

#include <iostream>
#include <string>
#include <ctime>

class NTPClient {
public:
    NTPClient();  // Constructor to initialize the NTP connection

    // Getters to retrieve various pieces of date and time information
    std::string getDate();        // Format: dd/mm/yyyy
    long getTimeInSeconds();      // Time in seconds
    int getHours();               // Current hour
    int getMinutes();             // Current minute
    std::string getDayOfWeek();   // Day of the week in text format
    std::string getMonth();       // Month in text format

    void displayMenu();           // Method to display the menu in typewriter style
    void typewriterPrint(const std::string &text, int colorCode); // Helper for typewriter printing with colors
    void typewriterPrintResult(const std::string &text, int colorCode);
    void displayReadableTime() ; // displays all in readable format 
    int getSeconds() ; // for converting the seconds to readable format 
    void  printWithTypewriterEffect(const std::string &label, const std::string &value, int colorCode)  ;  // dipsplay readable answers improved


private:
    void syncNTP();               // Method to contact the NTP server and sync time
    std::time_t currentTime;      // Store the current time synced with the NTP server
    std::string serverIPs[4] = {  // Array of NTP server IP addresses
        "93.93.131.118", 
        "185.53.93.157", 
        "158.43.128.33", 
        "134.0.16.1"
    };

    std::time_t getNTPTime(const std::string &serverIP);  // Helper function to get time from NTP server
};

#endif
