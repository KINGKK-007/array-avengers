#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_BOOKINGS 100

// Function prototypes
void health_main();
void health_cat_display();
void health_bookEvent();
void health_viewAllBookings();
void health_displayQRCode(float amountDue);
void health_printLine();
void health_exitProgram();
void health_goBack();
int health_isValidDate(char *dateStr);
int health_isValidTime(char *timeStr);
int health_isFutureDateTime(char *dateStr, char *timeStr);
int health_getIntInput(char *prompt, int min, int max);
void health_clearInputBuffer();

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
int health_bookingCount = 0;

// Event structure
typedef struct {
    char name[50];
    char description[255];
    float feePerPerson;
} Event;

// Array of Events
Event healht_events[] = {
    {"Yoga Sessions", "Join us for relaxing yoga sessions with experienced instructors.", 500.0},
    {"Marathons", "Participate in a community marathon for all ages.", 1000.0},
    {"Meditation Workshops", "Learn meditation techniques in a peaceful environment.", 750.0},
    {"Nature Walks", "Enjoy guided walks through beautiful natural landscapes.", 300.0},
    {"Trekking Events", "Experience thrilling treks suitable for all skill levels.", 1200.0},
    {"Zumba or Dance Classes", "Get moving with fun Zumba and dance classes.", 600.0},
    {"Health Check-up Camp", "Get a comprehensive health check-up by professionals.", 1500.0},
};

void health_saveBookingsToCSV(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error opening file for saving.\n");
        return;
    }

    // Write CSV header
    fprintf(file, "EventName,Date,Venue,Time,NumberOfPeople,FeePerPerson,TotalBeforeGST,GSTAmount,TotalAmount,Description,Status\n");

    for (int i = 0; i < health_bookingCount; i++) {
        fprintf(file, "\"%s\",\"%s\",\"%s\",\"%s\",%d,%.2f,%.2f,%.2f,%.2f,\"%s\",\"%s\"\n",
                bookings[i].eventName, bookings[i].date, bookings[i].venue, bookings[i].time,
                bookings[i].numberOfPeople, bookings[i].feePerPerson, bookings[i].totalBeforeGST,
                bookings[i].gstAmount, bookings[i].totalAmount, bookings[i].description, bookings[i].status);
    }

    fclose(file);
    printf("Bookings saved successfully to %s.\n", filename);
}

void health_loadBookingsFromCSV(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("No existing bookings found. Starting fresh.\n");
        return;
    }

    char line[1024];
    fgets(line, sizeof(line), file); // Skip the header line

    health_bookingCount = 0;
    while (fgets(line, sizeof(line), file) && health_bookingCount < MAX_BOOKINGS) {
        Booking booking = {0};

        sscanf(line, "\"%[^\"]\",\"%[^\"]\",\"%[^\"]\",\"%[^\"]\",%d,%f,%f,%f,%f,\"%[^\"]\",\"%[^\"]\"",
               booking.eventName, booking.date, booking.venue, booking.time, &booking.numberOfPeople,
               &booking.feePerPerson, &booking.totalBeforeGST, &booking.gstAmount, &booking.totalAmount,
               booking.description, booking.status);

        bookings[health_bookingCount++] = booking;
    }

    fclose(file);
    printf("Bookings loaded successfully from %s.\n", filename);
}

void health_main()
{

    while (1) {

        int choice;
        system("clear"); // Use "cls" if on Windows
        health_printLine();
        printf("\t\t\tHealth and Wellness Events\n");
        health_printLine();
        printf("[1] Book an Event\n");
        printf("[2] View All Bookings\n");
        printf("[3] Exit\n");
        health_printLine();
        printf("Enter your choice: ");
        scanf("%d",&choice);

        switch (choice) {
            case 1:
                health_bookEvent();
                break;
            case 2:
                health_viewAllBookings();
                break;
            case 3:
                health_exitProgram();
                break;
            default:
                printf("Invalid choice! Press Enter to try again...");
                getchar();
        }
    }
}

void health_cat_display() {
    health_printLine();
    printf("Select an Event Category:\n");
    health_printLine();
    int numEvents = sizeof(healht_events) / sizeof(healht_events[0]);
    for (int i = 0; i < numEvents; i++) {
        printf("[%d] %s\n", i + 1, healht_events[i].name);
    }
    printf("[%d] Go Back\n", numEvents + 1);
    health_printLine();
    printf("Enter your choice: ");
}

void health_bookEvent() {
    char choiceStr[10];
    int eventChoice;
    Booking newBooking;
    char confirmStr[10];
    char confirm;
    system("clear");
    health_cat_display();
    getchar();
    fgets(choiceStr, sizeof(choiceStr), stdin);
    sscanf(choiceStr, "%d", &eventChoice);

    int numEvents = sizeof(healht_events) / sizeof(healht_events[0]);

    if (eventChoice == numEvents + 1) {
        health_goBack();
        return;
    }

    if (eventChoice < 1 || eventChoice > numEvents) {
        printf("Invalid choice! Press Enter to return to menu...");
        getchar();
        return;
    }

    Event selectedEvent = healht_events[eventChoice - 1];
    strncpy(newBooking.eventName, selectedEvent.name, sizeof(newBooking.eventName) - 1);
    newBooking.eventName[sizeof(newBooking.eventName) - 1] = '\0';
    strncpy(newBooking.description, selectedEvent.description, sizeof(newBooking.description) - 1);
    newBooking.description[sizeof(newBooking.description) - 1] = '\0';
    newBooking.feePerPerson = selectedEvent.feePerPerson;

    system("clear");
    health_printLine();
    printf("Event Details:\n");
    health_printLine();
    printf("Event: %s\n", selectedEvent.name);
    printf("Description: %s\n", selectedEvent.description);
    printf("Fee per Person: ₹%.2f\n", selectedEvent.feePerPerson);
    health_printLine();

    // Ask for date, venue, time with validation
    while (1) {
        printf("Enter Date (DD/MM/YYYY): ");
        fgets(newBooking.date, sizeof(newBooking.date), stdin);
        strtok(newBooking.date, "\n"); // Remove newline character
        if (!health_isValidDate(newBooking.date)) {
            printf("Invalid date format. Please try again.\n");
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
        if (!health_isValidTime(newBooking.time)) {
            printf("Invalid time format. Please try again.\n");
            continue;
        }
        break;
    }

    // Check if date and time are in the future
    if (!health_isFutureDateTime(newBooking.date, newBooking.time)) {
        printf("Date and time must be in the future. Press Enter to return to menu...");
        getchar();
        return;
    }

    // Ask for the number of people with validation
    newBooking.numberOfPeople = health_getIntInput("Enter Number of People Attending (50-1500): ", 50, 1500);

    // Calculate costs
    newBooking.totalBeforeGST = newBooking.feePerPerson * newBooking.numberOfPeople;
    newBooking.gstAmount = newBooking.totalBeforeGST * 0.18; // 18% GST
    newBooking.totalAmount = newBooking.totalBeforeGST + newBooking.gstAmount;

    strncpy(newBooking.status, "Waiting for Approval", sizeof(newBooking.status) - 1);
    newBooking.status[sizeof(newBooking.status) - 1] = '\0';

    health_printLine();
    // Show cost breakdown
    printf("Cost Breakdown:\n");
    health_printLine();
    printf("Number of People: %d\n", newBooking.numberOfPeople);
    printf("Fee per Person: ₹%.2f\n", newBooking.feePerPerson);
    printf("Total before GST: ₹%.2f\n", newBooking.totalBeforeGST);
    printf("GST @18%%: ₹%.2f\n", newBooking.gstAmount);
    printf("Total Amount Payable: ₹%.2f\n", newBooking.totalAmount);
    health_printLine();

    printf("Confirm Booking and Proceed to Payment? (Y/N): ");
    fgets(confirmStr, sizeof(confirmStr), stdin);
    confirm = confirmStr[0];

    if (confirm == 'Y' || confirm == 'y') {
        health_displayQRCode(newBooking.totalAmount);
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
                health_printLine();
                printf("\t\t\t\tInvoice\n");
                health_printLine();
                printf("Event: %s\n", newBooking.eventName);
                printf("Date: %s\n", newBooking.date);
                printf("Venue: %s\n", newBooking.venue);
                printf("Time: %s\n", newBooking.time);
                health_printLine();
                printf("Number of People: %d\n", newBooking.numberOfPeople);
                printf("Fee per Person: ₹%.2f\n", newBooking.feePerPerson);
                printf("Total before GST: ₹%.2f\n", newBooking.totalBeforeGST);
                printf("GST @18%%: ₹%.2f\n", newBooking.gstAmount);
                printf("Total Amount Paid: ₹%.2f\n", newBooking.totalAmount);
                health_printLine();
                printf("Thank you for your payment!\n");
                health_printLine();
                printf("Press Enter to continue...");
                getchar();
            }

            strncpy(newBooking.status, "Not Approved", sizeof(newBooking.status) - 1);
            newBooking.status[sizeof(newBooking.status) - 1] = '\0';

            if (health_bookingCount < MAX_BOOKINGS) {
                bookings[health_bookingCount++] = newBooking;
                system("clear");
                health_printLine();
                printf("\t\t\tBooking Confirmed!\n");
                health_printLine();
                printf("Event: %s\n", newBooking.eventName);
                printf("Date: %s\n", newBooking.date);
                printf("Venue: %s\n", newBooking.venue);
                printf("Time: %s\n", newBooking.time);
                printf("Status: %s\n", newBooking.status);
                health_printLine();
                printf("Thank you for booking with us!\n");
                health_printLine();
                printf("Press Enter to continue...");
                getchar();
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
    const char *csvFile = "health_bookings.csv";
    health_saveBookingsToCSV(csvFile);
}

#include <stdio.h>

void health_viewAllBookings() {
    getchar(); // Clear buffer for user input
    FILE *file = fopen("health_bookings.csv", "r");
    if (file == NULL) {
        printf("No bookings found. The file does not exist or cannot be opened.\n");
        printf("Press Enter to return to the main menu...");
        getchar();
        return;
    }

    // Temporary variables to read each line of the file
    char line[1024];
    int lineCount = 0;

    printf("\n--- All Bookings ---\n");

    // Skip the header row
    if (fgets(line, sizeof(line), file) == NULL) {
        printf("No data available.\n");
        fclose(file);
        printf("Press Enter to return to the main menu...");
        getchar();
        return;
    }

    // Read the bookings from the file line by line
    while (fgets(line, sizeof(line), file)) {
        lineCount++;
        char eventName[50], date[20], venue[50], time[10], description[100], status[20];
        int numberOfPeople;
        float feePerPerson, totalBeforeGST, gstAmount, totalAmount;
        sscanf(line, "%49[^,],%19[^,],%49[^,],%9[^,],%d,%f,%f,%f,%f,%99[^,],%19[^\n]",
       eventName, date, venue, time, &numberOfPeople, &feePerPerson,
       &totalBeforeGST, &gstAmount, &totalAmount, description, status);

        // Display the booking details
        printf("\nBooking #%d:\n", lineCount);
        printf("Event Name       : %s\n", eventName);
        printf("Date             : %s\n", date);
        printf("Venue            : %s\n", venue);
        printf("Time             : %s\n", time);
        printf("Number of People : %d\n", numberOfPeople);
        printf("Fee Per Person   : %.2f\n", feePerPerson);
        printf("Total Before GST : %.2f\n", totalBeforeGST);
        printf("GST Amount       : %.2f\n", gstAmount);
        printf("Total Amount     : %.2f\n", totalAmount);
        printf("Description      : %s\n", description);
        printf("Status           : %s\n", status);
    }

    fclose(file);

    if (lineCount == 0) {
        printf("\nNo bookings found.\n");
    }

    // Provide the user with the option to return to the main menu
    int choice;
    do {
        printf("\n--- Menu ---\n");
        printf("1. Return to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            printf("\nReturning to the Main Menu...\n");
            return;
        } else {
            printf("\nInvalid choice. Please try again.\n");
        }
    } while (choice != 1);
}



void health_displayQRCode(float amount_due) {
    system("clear");
    health_printLine();
    printf("\t\t\tScan QR Code to Pay\n");
    health_printLine();

    // Display the amount due
    printf("\n\t\t\tAmount Due: %.2f\n", amount_due);
    health_printLine();

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


void health_printLine() {
    printf("\n==============================================================================\n\n");
}

void health_exitProgram() {
    printf("\nExiting...\n");
    exit(0);
}

void health_goBack() {
    printf("\nGoing back...\n");
    printf("Press Enter to continue...");
    getchar();
}

// Function to validate date format and values
int health_isValidDate(char *dateStr) {
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
int health_isValidTime(char *timeStr) {
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
int health_isFutureDateTime(char *dateStr, char *timeStr) {
    struct tm eventTime = {0};
    time_t currentTime;
    time(&currentTime);

    int day, month, year, hour, minute;
    sscanf(dateStr, "%d/%d/%d", &day, &month, &year);
    sscanf(timeStr, "%d:%d", &hour, &minute);

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
int health_getIntInput(char *prompt, int min, int max) {
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

void health_clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
