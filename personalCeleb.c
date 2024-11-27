#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_BOOKINGS 100
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define MAGENTA "\033[35m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define CYAN "\033[1;36m"

// Function prototypes
void personal_main();
void personal_cat_display();
void personal_bookEvent();
void personal_viewBookings();
void personal_displayQRCode(float amountDue);
void personal_printLine();
void personal_exitProgram();
void personal_goBack();
int personal_isValidDate(char *dateStr);
int personal_isValidTime(char *timeStr);
int personal_isFutureDateTime(char *dateStr, char *timeStr);
int personal_getIntInput(char *prompt, int min, int max);
void personal_clearInputBuffer();

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

typedef struct BookingNode {
    Booking data;
    struct BookingNode *next;
} BookingNode;

BookingNode *bookingList = NULL;

Booking bookings[MAX_BOOKINGS];
int personal_bookingCount = 0;

// Event structure
typedef struct {
    char name[50];
    char description[255];
    float feePerPerson;
} Event;

// Array of Personal Celebrations
Event personal_events[] = {
    {"Birthday Parties", "Celebrate your special day with us.", 500.0},
    {"Weddings", "Make your wedding memorable.", 2000.0},
    {"Anniversaries", "Celebrate years of togetherness.", 1500.0},
    {"Family Reunions", "Reconnect with your loved ones.", 800.0},
    {"Graduation Parties", "Honor your academic achievements.", 1000.0},
    {"Baby Showers", "Celebrate the upcoming joy.", 700.0},
    {"Housewarming Parties", "Warm up your new home with friends.", 600.0},
};

int personal_getTerminalWidth() {
    FILE *fp = popen("tput cols", "r");
    if (!fp)
        return 80; // Default width if the command fails
    int width;
    fscanf(fp, "%d", &width);
    pclose(fp);
    return width;
}

void personal_printLine() {
    printf("\033[1;36m%s\033[0m\n", "=======================================================");
}

void personal_displayCenteredText(const char* text, int width, const char* color) {
    int padding = (width - (int)strlen(text)) / 2;
    for (int i = 0; i < padding; i++) printf(" ");
    printf("%s%s%s\n", color, text, RESET);
}

void personal_displayBanner(int width) {
    for (int i = 0; i < width; i++) printf("\033[1;36m=\033[0m");
    printf("\n");
}

void personal_main() {
    while (1) {
        int choice;
        int width = personal_getTerminalWidth(); // Get the current terminal width dynamically

        // Clear the screen
        system("clear");

        // Display the header
        personal_displayBanner(width);
        personal_displayCenteredText("\xF0\x9F\x92\xAB PERSONAL CELEBRATIONS \xF0\x9F\x92\xAB", width, CYAN); // 💫
        personal_displayBanner(width);

        // Menu options
        printf("\n");
        personal_displayCenteredText("\x31\xE2\x83\xA3 Book an Event\n", width, YELLOW BOLD);   // 1️⃣
        personal_displayCenteredText("\x32\xE2\x83\xA3 View All Bookings\n", width, BLUE); // 2️⃣
        personal_displayCenteredText("\x33\xE2\x83\xA3 Exit\n", width, RED);           // 3️⃣
        printf("\n");


        // Prompt for choice
        printf("%sEnter your choice: %s", BOLD, GREEN);
        scanf("%d", &choice);
        getchar(); // Clear newline character from input buffer

        // Handle user choice
        switch (choice) {
            case 1:
                system("clear");
                personal_displayCenteredText("\xF0\x9F\x8E\x89 Booking an event... \xF0\x9F\x8E\x89", width, GREEN); // 🎉
                personal_bookEvent();
                sleep(2);
                break;

            case 2:
                system("clear");
                personal_displayCenteredText("\xF0\x9F\x93\x8C Viewing all bookings... \xF0\x9F\x93\x8C", width, GREEN); // 📌
                personal_viewBookings();
                sleep(2);
                break;

            case 3:
                system("clear");
                personal_displayCenteredText("\xF0\x9F\x9A\xAA Exiting the program. Thank you! \xF0\x9F\x9A\xAA", width, BLUE); // 🚪
                sleep(2);
                exit(0);

            default:
                system("clear");
                personal_displayCenteredText("\xF0\x9F\x98\xB1 Invalid choice! Please try again. \xF0\x9F\x98\xB1", width, RED); // 😱
                sleep(2);
        }
    }
}

void personal_cat_display() {
    int width = personal_getTerminalWidth(); // Dynamically determine terminal width

    // Clear the screen for a fresh UI
    system("clear");

    // Display header
    printf("\n");
    personal_displayCenteredText("\xF0\x9F\x92\x96 Personal Event Categories", width, MAGENTA BOLD); // 💖
    printf("\n");
    for (int i = 0; i < width; i++) // Print top border
        printf("%s=%s", CYAN, RESET);
    printf("\n");

    // Display event categories
    int numEvents = sizeof(personal_events) / sizeof(personal_events[0]);
    for (int i = 0; i < numEvents; i++) {
        char eventLine[200];
        snprintf(eventLine, sizeof(eventLine), "[%d] %s", i + 1, personal_events[i].name);
        personal_displayCenteredText(eventLine, width, GREEN); // Display each event in green
    }

    // Display "Go Back" option
    char goBackLine[200];
    snprintf(goBackLine, sizeof(goBackLine), "[%d] Go Back", numEvents + 1);
    personal_displayCenteredText(goBackLine, width, YELLOW BOLD); // "Go Back" in yellow bold

    // Print bottom separator
    printf("\n");
    for (int i = 0; i < width; i++) // Print bottom border
        printf("%s=%s", CYAN, RESET);
    printf("\n");

    // Prompt for user input
    personal_displayCenteredText("Enter your choice:", width, BLUE);
    printf("%s> %s", BOLD, RESET);
}


void personal_bookEvent() {
    char choiceStr[10];
    int eventChoice;
    Booking newBooking;
    char confirmStr[10];
    char confirm;

    system("clear");

    // Display event categories
    personal_cat_display();

    // Get user's event choice
    printf("\n\033[1;33mEnter your choice: \033[0m");
    fgets(choiceStr, sizeof(choiceStr), stdin);
    sscanf(choiceStr, "%d", &eventChoice);

    int numEvents = sizeof(personal_events) / sizeof(personal_events[0]);

    if (eventChoice == numEvents + 1) {
        personal_goBack();
        return;
    }

    if (eventChoice < 1 || eventChoice > numEvents) {
        printf("\033[1;31mInvalid choice!\033[0m Press Enter to return to menu...");
        getchar();
        return;
    }

    Event selectedEvent = personal_events[eventChoice - 1];
    strncpy(newBooking.eventName, selectedEvent.name, sizeof(newBooking.eventName) - 1);
    newBooking.eventName[sizeof(newBooking.eventName) - 1] = '\0';
    strncpy(newBooking.description, selectedEvent.description, sizeof(newBooking.description) - 1);
    newBooking.description[sizeof(newBooking.description) - 1] = '\0';
    newBooking.feePerPerson = selectedEvent.feePerPerson;

    system("clear");

    // Display selected event details
    personal_printLine();
    printf("\033[1;36mEvent Details\033[0m\n");
    personal_printLine();
    printf("\033[1;32mEvent:\033[0m %s\n", selectedEvent.name);
    printf("\033[1;32mDescription:\033[0m %s\n", selectedEvent.description);
    printf("\033[1;32mFee per Person:\033[0m ₹%.2f\n", selectedEvent.feePerPerson);
    personal_printLine();

    // Input Date
    while (1) {
        printf("\033[1;33mEnter Date (DD/MM/YYYY): \033[0m");
        fgets(newBooking.date, sizeof(newBooking.date), stdin);
        strtok(newBooking.date, "\n"); // Remove newline character
        if (!personal_isValidDate(newBooking.date)) {
            printf("\033[1;31mInvalid date format. Please try again.\033[0m\n");
            continue;
        }
        if (!personal_isFutureDateTime(newBooking.date, "00:00")) {
            printf("\033[1;31mDate must be in the future. Please enter a future date.\033[0m\n");
            continue;
        }
        break;
    }

    // Input Venue
    printf("\033[1;33mEnter Venue: \033[0m");
    fgets(newBooking.venue, sizeof(newBooking.venue), stdin);
    strtok(newBooking.venue, "\n");

    // Input Time
    while (1) {
        printf("\033[1;33mEnter Time (HH:MM): \033[0m");
        fgets(newBooking.time, sizeof(newBooking.time), stdin);
        strtok(newBooking.time, "\n");
        if (!personal_isValidTime(newBooking.time)) {
            printf("\033[1;31mInvalid time format. Please try again.\033[0m\n");
            continue;
        }
        if (!personal_isFutureDateTime(newBooking.date, newBooking.time)) {
            printf("\033[1;31mTime must be in the future. Please enter a future time.\033[0m\n");
            continue;
        }
        break;
    }

    // Input Number of People
    newBooking.numberOfPeople = personal_getIntInput("\033[1;33mEnter Number of People Attending (50-1500): \033[0m", 50, 1500);

    // Calculate costs
    newBooking.totalBeforeGST = newBooking.feePerPerson * newBooking.numberOfPeople;
    newBooking.gstAmount = newBooking.totalBeforeGST * 0.18;
    newBooking.totalAmount = newBooking.totalBeforeGST + newBooking.gstAmount;
    strncpy(newBooking.status, "Waiting for Approval", sizeof(newBooking.status) - 1);
    newBooking.status[sizeof(newBooking.status) - 1] = '\0';

    // Display cost breakdown
    personal_printLine();
    printf("\033[1;36mCost Breakdown\033[0m\n");
    personal_printLine();
    printf("\033[1;32mNumber of People:\033[0m %d\n", newBooking.numberOfPeople);
    printf("\033[1;32mFee per Person:\033[0m ₹%.2f\n", newBooking.feePerPerson);
    printf("\033[1;32mTotal before GST:\033[0m ₹%.2f\n", newBooking.totalBeforeGST);
    printf("\033[1;32mGST @18%%:\033[0m ₹%.2f\n", newBooking.gstAmount);
    printf("\033[1;32mTotal Amount Payable:\033[0m ₹%.2f\n", newBooking.totalAmount);
    personal_printLine();

    // Confirm booking
    printf("\033[1;33mConfirm Booking and Proceed to Payment? (Y/N): \033[0m");
    fgets(confirmStr, sizeof(confirmStr), stdin);
    confirm = confirmStr[0];

    if (confirm == 'Y' || confirm == 'y') {
        personal_displayQRCode(newBooking.totalAmount);
        printf("\033[1;36mPlease pay ₹%.2f\033[0m\n", newBooking.totalAmount);
        printf("\033[1;33mPayment made? (Y/N): \033[0m");
        fgets(confirmStr, sizeof(confirmStr), stdin);
        confirm = confirmStr[0];

        if (confirm == 'Y' || confirm == 'y') {
            printf("\033[1;33mWould you like to view your invoice? (Y/N): \033[0m");
            fgets(confirmStr, sizeof(confirmStr), stdin);
            confirm = confirmStr[0];

            if (confirm == 'Y' || confirm == 'y') {
                system("clear");
                personal_printLine();
                printf("\033[1;36m\t\t\tInvoice\033[0m\n");
                personal_printLine();
                printf("\033[1;32mEvent:\033[0m %s\n", newBooking.eventName);
                printf("\033[1;32mDate:\033[0m %s\n", newBooking.date);
                printf("\033[1;32mVenue:\033[0m %s\n", newBooking.venue);
                printf("\033[1;32mTime:\033[0m %s\n", newBooking.time);
                personal_printLine();
                printf("\033[1;32mNumber of People:\033[0m %d\n", newBooking.numberOfPeople);
                printf("\033[1;32mFee per Person:\033[0m ₹%.2f\n", newBooking.feePerPerson);
                printf("\033[1;32mTotal before GST:\033[0m ₹%.2f\n", newBooking.totalBeforeGST);
                printf("\033[1;32mGST @18%%:\033[0m ₹%.2f\n", newBooking.gstAmount);
                printf("\033[1;32mTotal Amount Paid:\033[0m ₹%.2f\n", newBooking.totalAmount);
                personal_printLine();
                printf("\033[1;36mThank you for your payment!\033[0m\n");
                personal_printLine();
                printf("Press Enter to continue...");
                getchar();
            }

            strncpy(newBooking.status, "Approved", sizeof(newBooking.status) - 1);
            bookings[personal_bookingCount++] = newBooking;
            system("clear");
            printf("\033[1;32mBooking Confirmed!\033[0m\n");
        } else {
            printf("\033[1;31mPayment not completed. Booking canceled.\033[0m\n");
        }
    } else {
        printf("\033[1;31mBooking canceled.\033[0m\n");
    }

    printf("Press Enter to return to menu...");
    getchar();
}


void personal_viewBookings() {
    system("clear");

    int terminalWidth = personal_getTerminalWidth();  // Get the terminal width dynamically
    int columnWidth = terminalWidth / 8;  // Divide by 8 to fit the table columns neatly (adjust as needed)

    // Display centered title with color
    personal_printLine();
    personal_displayCenteredText("🎟️ All Bookings 🎟️", terminalWidth, "yellow");
    personal_printLine();

    if (personal_bookingCount == 0) {
        personal_displayCenteredText("No bookings found.", terminalWidth, "red");
    } else {
        // Print table header with dynamic column width
        printf("\n%-*s %-*s %-*s %-*s %-*s %-*s %-*s %-*s\n", 
               columnWidth, "ID", columnWidth, "Event", columnWidth, "Date", 
               columnWidth, "Venue", columnWidth, "Time", columnWidth, 
               "No. of People", columnWidth, "Amount Paid", columnWidth, "Status");

        personal_printLine();

        for (int i = 0; i < personal_bookingCount; i++) {
            // Print each booking in table format with dynamic column width
            printf("%-*d %-*s %-*s %-*s %-*s %-*d %-*0.2f %-*s\n", 
                   columnWidth, i + 1, columnWidth, bookings[i].eventName, columnWidth, 
                   bookings[i].date, columnWidth, bookings[i].venue, columnWidth, 
                   bookings[i].time, columnWidth, bookings[i].numberOfPeople, 
                   columnWidth, bookings[i].totalAmount, columnWidth, bookings[i].status);
        }
    }

    personal_printLine();
    personal_displayCenteredText("Press Enter to return to menu...", terminalWidth, "green");
    getchar();
}

void personal_displayQRCode(float amount_due) {
    system("clear");
    personal_printLine();
    printf("\t\t\tScan QR Code to Pay\n");
    personal_printLine();

    // Display the amount due
    printf("\n\t\t\tAmount Due: %.2f\n", amount_due);
    personal_printLine();

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
}

void personal_exitProgram() {
    printf("\nExiting...\n");
    exit(0);
}

void personal_goBack() {
    printf("\nGoing back...\n");
    printf("Press Enter to continue...");
    getchar();
}

// Function to validate date format and values
int personal_isValidDate(char *dateStr) {
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
int personal_isValidTime(char *timeStr) {
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
int personal_isFutureDateTime(char *dateStr, char *timeStr) {
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
int personal_getIntInput(char *prompt, int min, int max) {
    int value;
    char inputStr[20];
    while (1) {
        printf("%s", prompt);
        // personal_clearInputBuffer();
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

void personal_clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void freeBookingList() {
    BookingNode *current = bookingList;
    while (current) {
        BookingNode *temp = current;
        current = current->next;
        free(temp);
    }
}