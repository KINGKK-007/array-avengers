#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_BOOKINGS 100
 
// Function prototypes
void com_displayMenu();
void com_bookEvent();
void com_viewBookings();
void com_displayQRCode(float amountDue);
void com_printLine();
void com_exitProgram();
void com_goBack();
int com_isValidDate(char *dateStr);
int com_isValidTime(char *timeStr);
int com_isFutureDateTime(char *dateStr, char *timeStr);
int com_getIntInput(char *prompt, int min, int max);
void com_clearInputBuffer();

// Booking structure
typedef struct {
    char eventName[50];
    char date[15];
    char venue[50];
    char time[10];
    int numberOfPeople;
    float feePerPerson;
    float totalBeforeGST;
    float gstAmount;
    float totalAmount;
    char description[255];
    char status[20];
} Booking;

Booking bookings[MAX_BOOKINGS];
int com_bookingCount = 0;

// Event structure
typedef struct {
    char name[50];
    char description[255];
    float feePerPerson;
} Event;

// Array of Community and Cultural Events
Event com_events[] = {
    {"Local Festivals", "Celebrate local traditions and festivities.", 500.0},
    {"Art Exhibitions", "Explore artworks from local artists.", 800.0},
    {"Music Concerts", "Enjoy live performances by bands and artists.", 1000.0},
    {"Food Fairs", "Taste a variety of cuisines and delicacies.", 600.0},
    {"Cultural Parades", "Experience vibrant cultural parades.", 700.0},
    {"Community Workshops", "Participate in skill-building workshops.", 400.0},
    {"Charity Events", "Support causes through community com_events.", 550.0},
};

int main() {
    char choiceStr[10];
    int choice;

    while (1) {
        system("clear"); // Use "cls" if on Windows
        com_printLine();
        printf("\t\t\tCommunity and Cultural Events\n");
        com_printLine();
        printf("[1] Book an Event\n");
        printf("[2] View All Bookings\n");
        printf("[3] Exit\n");
        com_printLine();
        printf("Enter your choice: ");
        fgets(choiceStr, sizeof(choiceStr), stdin);
        sscanf(choiceStr, "%d", &choice);

        switch (choice) {
            case 1:
                com_bookEvent();
                break;
            case 2:
                com_viewBookings();
                break;
            case 3:
                com_exitProgram();
                break;
            default:
                printf("Invalid choice! Press Enter to try again...");
                getchar();
        }
    }
    return 0;
}

void com_displayMenu() {
    com_printLine();
    printf("Select an Event Category:\n");
    com_printLine();
    int numEvents = sizeof(com_events) / sizeof(com_events[0]);
    for (int i = 0; i < numEvents; i++) {
        printf("[%d] %s\n", i + 1, com_events[i].name);
    }
    printf("[%d] Go Back\n", numEvents + 1);
    com_printLine();
    printf("Enter your choice: ");
}

void com_bookEvent() {
    char choiceStr[10];
    int eventChoice;
    Booking newBooking;
    char confirmStr[10];
    char confirm;
    system("clear");
    com_displayMenu();
    fgets(choiceStr, sizeof(choiceStr), stdin);
    sscanf(choiceStr, "%d", &eventChoice);

    int numEvents = sizeof(com_events) / sizeof(com_events[0]);

    if (eventChoice == numEvents + 1) {
        com_goBack();
        return;
    }

    if (eventChoice < 1 || eventChoice > numEvents) {
        printf("Invalid choice! Press Enter to return to menu...");
        getchar();
        return;
    }

    Event selectedEvent = com_events[eventChoice - 1];
    strncpy(newBooking.eventName, selectedEvent.name, sizeof(newBooking.eventName) - 1);
    newBooking.eventName[sizeof(newBooking.eventName) - 1] = '\0';
    strncpy(newBooking.description, selectedEvent.description, sizeof(newBooking.description) - 1);
    newBooking.description[sizeof(newBooking.description) - 1] = '\0';
    newBooking.feePerPerson = selectedEvent.feePerPerson;

    system("clear");
    com_printLine();
    printf("Event Details:\n");
    com_printLine();
    printf("Event: %s\n", selectedEvent.name);
    printf("Description: %s\n", selectedEvent.description);
    printf("Fee per Person: ₹%.2f\n", selectedEvent.feePerPerson);
    com_printLine();

    // Ask for date, venue, time with validation
    while (1) {
        printf("Enter Date (DD/MM/YYYY): ");
        fgets(newBooking.date, sizeof(newBooking.date), stdin);
        strtok(newBooking.date, "\n"); // Remove newline character
        if (!com_isValidDate(newBooking.date)) {
            printf("Invalid date format. Please try again.\n");
            continue;
        }
        // Check if date is in the future
        if (!com_isFutureDateTime(newBooking.date, "00:00")) {
            printf("Date must be in the future. Please enter a future date.\n");
            continue;
        }
        break;
    }

    printf("Enter Venue: ");
    fgets(newBooking.venue, sizeof(newBooking.venue), stdin);
    strtok(newBooking.venue, "\n");

    while (1) {
        printf("Enter Time (HH:MM): ");
        fgets(newBooking.time, sizeof(newBooking.time), stdin);
        strtok(newBooking.time, "\n");
        if (!com_isValidTime(newBooking.time)) {
            printf("Invalid time format. Please try again.\n");
            continue;
        }
        // Check if time is in the future on the same date
        if (!com_isFutureDateTime(newBooking.date, newBooking.time)) {
            printf("Time must be in the future. Please enter a future time.\n");
            continue;
        }
        break;
    }

    // Ask for the number of people with validation
    newBooking.numberOfPeople = com_getIntInput("Enter Number of People Attending (50-1500): ", 50, 1500);

    // Calculate costs
    newBooking.totalBeforeGST = newBooking.feePerPerson * newBooking.numberOfPeople;
    newBooking.gstAmount = newBooking.totalBeforeGST * 0.18; // 18% GST
    newBooking.totalAmount = newBooking.totalBeforeGST + newBooking.gstAmount;

    strncpy(newBooking.status, "Waiting for Approval", sizeof(newBooking.status) - 1);
    newBooking.status[sizeof(newBooking.status) - 1] = '\0';

    com_printLine();
    // Show cost breakdown
    printf("Cost Breakdown:\n");
    com_printLine();
    printf("Number of People: %d\n", newBooking.numberOfPeople);
    printf("Fee per Person: ₹%.2f\n", newBooking.feePerPerson);
    printf("Total before GST: ₹%.2f\n", newBooking.totalBeforeGST);
    printf("GST @18%%: ₹%.2f\n", newBooking.gstAmount);
    printf("Total Amount Payable: ₹%.2f\n", newBooking.totalAmount);
    com_printLine();

    printf("Confirm Booking and Proceed to Payment? (Y/N): ");
    fgets(confirmStr, sizeof(confirmStr), stdin);
    confirm = confirmStr[0];

    if (confirm == 'Y' || confirm == 'y') {
        com_displayQRCode(newBooking.totalAmount);
        printf("Please pay ₹%.2f\n", newBooking.totalAmount);
        printf("Payment made? (Y/N): ");
        fgets(confirmStr, sizeof(confirmStr), stdin);
        confirm = confirmStr[0];

        if (confirm == 'Y' || confirm == 'y') {
            // Option to view invoice
            printf("Would you like to view your invoice? (Y/N): ");
            fgets(confirmStr, sizeof(confirmStr), stdin);
            confirm = confirmStr[0];

            if (confirm == 'Y' || confirm == 'y') {
                system("clear");
                com_printLine();
                printf("\t\t\t\tInvoice\n");
                com_printLine();
                printf("Event: %s\n", newBooking.eventName);
                printf("Date: %s\n", newBooking.date);
                printf("Venue: %s\n", newBooking.venue);
                printf("Time: %s\n", newBooking.time);
                com_printLine();
                printf("Number of People: %d\n", newBooking.numberOfPeople);
                printf("Fee per Person: ₹%.2f\n", newBooking.feePerPerson);
                printf("Total before GST: ₹%.2f\n", newBooking.totalBeforeGST);
                printf("GST @18%%: ₹%.2f\n", newBooking.gstAmount);
                printf("Total Amount Paid: ₹%.2f\n", newBooking.totalAmount);
                com_printLine();
                printf("Thank you for your payment!\n");
                com_printLine();
                printf("Press Enter to continue...");
                getchar();
            }

            strncpy(newBooking.status, "Approved", sizeof(newBooking.status) - 1);
            newBooking.status[sizeof(newBooking.status) - 1] = '\0';

            if (com_bookingCount < MAX_BOOKINGS) {
                bookings[com_bookingCount++] = newBooking;
                system("clear");
                com_printLine();
                printf("\t\t\tBooking Confirmed!\n");
                com_printLine();
                printf("Event: %s\n", newBooking.eventName);
                printf("Date: %s\n", newBooking.date);
                printf("Venue: %s\n", newBooking.venue);
                printf("Time: %s\n", newBooking.time);
                printf("Status: %s\n", newBooking.status);
                com_printLine();
                printf("Thank you for booking with us!\n");
                com_printLine();
                printf("[1] Book Another Event\n");
                printf("[2] View All Bookings\n");
                printf("[3] Exit\n");
                com_printLine();
                printf("Enter your choice: ");
                fgets(confirmStr, sizeof(confirmStr), stdin);
                int postChoice = atoi(confirmStr);

                switch (postChoice) {
                    case 1:
                        com_bookEvent();
                        break;
                    case 2:
                        com_viewBookings();
                        break;
                    case 3:
                        com_exitProgram();
                        break;
                    default:
                        printf("Invalid choice! Press Enter to return to main menu...");
                        getchar();
                }
            } else {
                printf("Booking limit reached. Cannot accept more bookings.\n");
                printf("Press Enter to return to menu...");
                getchar();
            }
        } else {
            printf("Payment not completed. Booking canceled.\n");
            printf("Press Enter to return to menu...");
            getchar();
        }
    } else {
        printf("Booking canceled.\n");
        printf("Press Enter to return to menu...");
        getchar();
    }
}

void com_viewBookings() {
    system("clear");
    com_printLine();
    printf("\t\t\tAll Bookings\n");
    com_printLine();
    if (com_bookingCount == 0) {
        printf("No bookings found.\n");
    } else {
        for (int i = 0; i < com_bookingCount; i++) {
            printf("Booking %d:\n", i + 1);
            printf("\tEvent: %s\n", bookings[i].eventName);
            printf("\tDate: %s\n", bookings[i].date);
            printf("\tVenue: %s\n", bookings[i].venue);
            printf("\tTime: %s\n", bookings[i].time);
            printf("\tNumber of People: %d\n", bookings[i].numberOfPeople);
            printf("\tTotal Amount Paid: ₹%.2f\n", bookings[i].totalAmount);
            printf("\tStatus: %s\n", bookings[i].status);
            printf("\tDescription: %s\n", bookings[i].description);
            com_printLine();
        }
    }
    printf("Press Enter to return to menu...");
    getchar();
}

// Function to display a QR code with solid edges and random interior
// void com_displayQRCode(float amountDue) {
//     system("clear");
//     com_printLine();
//     printf("\t\t\tScan QR Code to Pay\n");
//     com_printLine();

//     srand(time(0)); // Seed the random number generator for randomness
//     int size = 21; // QR code size (21x21 for standard)

//     for (int i = 0; i < size; i++) {
//         for (int j = 0; j < size; j++) {
//             // Fill the edges
//             if (i == 0 || i == size - 1 || j == 0 || j == size -1) {
//                 printf("██"); // Edge blocks
//             } else {
//                 // Randomly decide whether to print a block or a space
//                 if (rand() % 2 == 0) {
//                     printf("  "); // Double spaces for white space
//                 } else {
//                     printf("██"); // Use block character for black square
//                 }
//             }
//         }
//         printf("\n");
//     }

//     com_printLine();
// }
void com_displayQRCode(float amountDue) {
    system("clear");
    com_printLine();
    printf("\t\t\tScan QR Code to Pay\n");
    com_printLine();

    // Display the amount due
    printf("\n\t\t\tAmount Due: %.2f\n", amountDue);
    com_printLine();

    srand(time(0)); // Seed the random number generator for randomness
    int size = 21; // QR code size (21x21 for standard)

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            // Fill the edges
            if (i == 0 || i == size - 1 || j == 0 || j == size - 1) {
                printf("██"); // Edge blocks
            } else {
                // Randomly decide whether to print a block or a space
                if (rand() % 2 == 0) {
                    printf("  "); // Double spaces for white space
                } else {
                    printf("██"); // Use block character for black square
                }
            }
        }
        printf("\n");
    }

    com_printLine();
}


void com_printLine() {
    printf("\n==============================================================================\n\n");
}

void com_exitProgram() {
    printf("\nExiting...\n");
    exit(0);
}

void com_goBack() {
    printf("\nGoing back...\n");
    printf("Press Enter to continue...");
    getchar();
}

// Function to validate date format and values
int com_isValidDate(char *dateStr) {
    int day, month, year;
    if (sscanf(dateStr, "%d/%d/%d", &day, &month, &year) != 3)
        return 0;
    if (year < 1900 || year > 9999)
        return 0;
    if (month < 1 || month > 12)
        return 0;
    if (day < 1 || day > 31)
        return 0;
    // Check for months with fewer days
    if (month == 2) {
        // Leap year check
        int isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (day > (isLeap ? 29 : 28))
            return 0;
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30)
            return 0;
    }
    return 1;
}

// Function to validate time format and values
int com_isValidTime(char *timeStr) {
    int hour, minute;
    if (sscanf(timeStr, "%d:%d", &hour, &minute) != 2)
        return 0;
    if (hour < 0 || hour > 23)
        return 0;
    if (minute < 0 || minute > 59)
        return 0;
    return 1;
}

// Function to check if the date and time are in the future
int com_isFutureDateTime(char *dateStr, char *timeStr) {
    struct tm eventTime = {0};
    time_t currentTime;
    time(&currentTime);

    int day, month, year, hour = 0, minute = 0;
    sscanf(dateStr, "%d/%d/%d", &day, &month, &year);
    if (strcmp(timeStr, "00:00") != 0) {
        sscanf(timeStr, "%d:%d", &hour, &minute);
    }

    eventTime.tm_year = year - 1900;
    eventTime.tm_mon = month - 1;
    eventTime.tm_mday = day;
    eventTime.tm_hour = hour;
    eventTime.tm_min = minute;
    eventTime.tm_sec = 0;
    eventTime.tm_isdst = -1; // Auto-detect daylight saving time

    time_t eventTimestamp = mktime(&eventTime);
    if (eventTimestamp == -1)
        return 0;

    return difftime(eventTimestamp, currentTime) > 0;
}

// Function to get integer input within a range with validation
int com_getIntInput(char *prompt, int min, int max) {
    int value;
    char inputStr[20];
    while (1) {
        printf("%s", prompt);
        fgets(inputStr, sizeof(inputStr), stdin);
        if (sscanf(inputStr, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        if (value < min || value > max) {
            printf("Please enter a number between %d and %d.\n", min, max);
            continue;
        }
        return value;
    }
}

void com_clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
