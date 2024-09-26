#include <iostream>
#include <string>
#include <cstdlib> // For system()
#include "NTPClient.hpp"

// Function to clear the screen
void clearScreen() {
#ifdef _WIN32
    system("cls");  // Windows
#else
    system("clear");  // Unix/Linux
#endif
}

int main() {
    NTPClient client;

    while (true) {
        clearScreen();  // Clear the screen before displaying the menu
        client.displayMenu();  // Display the menu

        int choice;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        // Define a color for the message
        int messageColor = 36;  // Example: Cyan color

        // Check if the user wants to exit
        if (choice == 0) {
            client.printWithTypewriterEffect("Exiting the program...", "", messageColor);
            break;  // Exit the loop
        }

        // Notify the user of their choice
        client.printWithTypewriterEffect("You selected", std::to_string(choice), messageColor);

        // Handle the choice
        switch (choice) {
            case 1:
                client.printWithTypewriterEffect("Date", client.getDate(), 32);  // Green for date
                break;
            case 2:
                client.printWithTypewriterEffect("Time in seconds", std::to_string(client.getTimeInSeconds()), 33);  // Yellow for seconds
                break;
            case 3:
                client.printWithTypewriterEffect("Hours", std::to_string(client.getHours()), 34);  // Blue for hours
                break;
            case 4:
                client.printWithTypewriterEffect("Minutes", std::to_string(client.getMinutes()), 35);  // Magenta for minutes
                break;
            case 5:
                client.printWithTypewriterEffect("Seconds", std::to_string(client.getSeconds()), 36);  // Cyan for seconds
                break;
            case 6:
                client.printWithTypewriterEffect("Day", client.getDayOfWeek(), 37);  // White for day
                break;
            case 7:
                client.printWithTypewriterEffect("Month", client.getMonth(), 31);  // Red for month
                break;
            case 8:
                client.displayReadableTime();  // Display all time information
                break;
            default:
                client.printWithTypewriterEffect("Invalid option", "Please try again. or enter 0 to exit ", 31);  // Red for error
                break;
        }

        // Optional: Wait for user input before clearing the screen again
        std::cout << "Press Enter to continue...";
        std::cin.ignore(); // Ignore the newline character from previous input
        std::cin.get(); // Wait for user to press Enter
    }

    // Optional cleanup before exit
    std::cout << "\nThank you for using the NTP Client. Goodbye!\n";
    return 0;
}
