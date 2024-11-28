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
void personal_displayQRCode();
void personal_printLine();
void personal_exitProgram();
void personal_goBack();
int personal_isValidDate(char *dateStr);
int personal_isValidTime(char *timeStr);
int personal_isFutureDateTime(char *dateStr, char *timeStr);
int personal_getIntInput(char *prompt, int min, int max);
void personal_clearInputBuffer();

typedef struct
{
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
} personal_Booking;

typedef struct personal_BookingNode
{
    personal_Booking data;
    struct personal_BookingNode *next;
} personal_BookingNode;

personal_BookingNode *personal_bookingList = NULL;

personal_Booking bookings[MAX_BOOKINGS];
int personal_bookingCount = 0;

// Event structure
typedef struct
{
    char name[50];
    char description[255];
    float feePerPerson;
} Event;

// Function to save bookings to a CSV file
void personal_saveBookingsToCSV()
{
    FILE *file = fopen("Bookings.csv", "a");
    if (!file)
    {
        printf("\033[1;31mError: Unable to open CSV file for writing.\033[0m\n");
        return;
    }

    // Write header row
    fprintf(file, "Event Name,Description,Date,Time,Venue,Number of People,Fee Per Person,Total Before GST,GST Amount,Total Amount,Status\n");

    // Write each booking
    for (int i = 0; i < personal_bookingCount; i++)
    {
        fprintf(file, "\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",%d,%.2f,%.2f,%.2f,%.2f,\"%s\"\n",
                bookings[i].eventName,
                bookings[i].description,
                bookings[i].date,
                bookings[i].time,
                bookings[i].venue,
                bookings[i].numberOfPeople,
                bookings[i].feePerPerson,
                bookings[i].totalBeforeGST,
                bookings[i].gstAmount,
                bookings[i].totalAmount,
                bookings[i].status);
    }

    fclose(file);
    printf("\033[1;32mBookings saved successfully to %s\033[0m\n", "Bookings.csv");
}

// Function to load bookings from a CSV file
void personal_loadBookingsFromCSV()
{
    FILE *file = fopen("Bookings.csv", "r");
    if (file == NULL)
    {
        printf("\033[1;33mNo existing booking file found. Starting fresh.\033[0m\n");
        return;
    }

    char line[1024];
    int lineCount = 0;

    // Skip the CSV header
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file))
    {
        lineCount++;
        personal_Booking newBooking;
        char *token;

        // Parse each column
        token = strtok(line, "\",");
        strncpy(newBooking.eventName, token, sizeof(newBooking.eventName) - 1);

        token = strtok(NULL, "\",");
        strncpy(newBooking.date, token, sizeof(newBooking.date) - 1);

        token = strtok(NULL, "\",");
        strncpy(newBooking.venue, token, sizeof(newBooking.venue) - 1);

        token = strtok(NULL, "\",");
        strncpy(newBooking.time, token, sizeof(newBooking.time) - 1);

        token = strtok(NULL, "\",");
        newBooking.numberOfPeople = atoi(token);

        token = strtok(NULL, "\",");
        newBooking.feePerPerson = atof(token);

        token = strtok(NULL, "\",");
        newBooking.totalBeforeGST = atof(token);

        token = strtok(NULL, "\",");
        newBooking.gstAmount = atof(token);

        token = strtok(NULL, "\",");
        newBooking.totalAmount = atof(token);

        token = strtok(NULL, "\",");
        strncpy(newBooking.description, token, sizeof(newBooking.description) - 1);

        token = strtok(NULL, "\",");
        strncpy(newBooking.status, token, sizeof(newBooking.status) - 1);

        // Add the booking to the linked list
        personal_BookingNode *newNode = (personal_BookingNode *)malloc(sizeof(personal_BookingNode));
        newNode->data = newBooking;
        newNode->next = personal_bookingList;
        personal_bookingList = newNode;
    }

    fclose(file);
    printf("\033[1;32m%d bookings loaded from Bookings.csv.\033[0m\n", lineCount);
}

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

int personal_getTerminalWidth()
{
    FILE *fp = popen("tput cols", "r");
    if (!fp)
        return 80; // Default width if the command fails
    int width;
    fscanf(fp, "%d", &width);
    pclose(fp);
    return width;
}

void personal_printLine()
{
    printf("\033[1;36m%s\033[0m\n", "=======================================================");
}

void personal_displayCenteredText(const char *text, int width, const char *color)
{
    int padding = (width - (int)strlen(text)) / 2;
    for (int i = 0; i < padding; i++)
        printf(" ");
    printf("%s%s%s\n", color, text, RESET);
}

void personal_displayBanner(int width)
{
    for (int i = 0; i < width; i++)
        printf("\033[1;36m=\033[0m");
    printf("\n");
}

void personal_main()
{
    while (1)
    {
        int choice;
        int width = personal_getTerminalWidth();

        // Clear the screen
        system("clear");

        // Display the header
        personal_displayBanner(width);
        personal_displayCenteredText("\xF0\x9F\x92\xAB WELCOME TO PERSONAL CELEBRATIONS \xF0\x9F\x92\xAB", width, CYAN); // 💫
        personal_displayBanner(width);

        // Menu options
        printf("\n");
        personal_displayCenteredText("1️⃣.  Book an Event", width, YELLOW BOLD); // 1️⃣
        personal_displayCenteredText("2️⃣.  View All Bookings", width, BLUE);    // 2️⃣
        personal_displayCenteredText("3️⃣.  Exit", width, RED);                  // 3️⃣
        printf("\n");

        // Footer line
        personal_displayBanner(width);

        // Prompt for choice
        personal_displayCenteredText("💡 Please make your selection below: ", width, GREEN);
        printf("\n%sEnter your choice: %s", BOLD, GREEN);
        scanf("%d", &choice);
        getchar(); // Clear newline character from input buffer

        // Handle user choice
        switch (choice)
        {
        case 1:
            system("clear");
            personal_displayCenteredText("\xF0\x9F\x8E\x89 Booking an Event... \xF0\x9F\x8E\x89", width, GREEN); // 🎉
            personal_bookEvent();
            sleep(2);
            break;

        case 2:
            system("clear");
            personal_displayCenteredText("\xF0\x9F\x93\x8C Viewing All Bookings... \xF0\x9F\x93\x8C", width, BLUE); // 📌
            personal_viewBookings();
            sleep(2);
            break;

        case 3:
            system("clear");
            personal_displayCenteredText("\xF0\x9F\x9A\xAA Exiting the Program. Thank you! \xF0\x9F\x9A\xAA", width, BLUE); // 🚪
            sleep(2);
            exit(0);

        default:
            system("clear");
            personal_displayCenteredText("\xF0\x9F\x98\xB1 Invalid choice! Please try again. \xF0\x9F\x98\xB1", width, RED); // 😱
            sleep(2);
        }
    }
}

void personal_cat_display()
{
    int width = personal_getTerminalWidth(); // Dynamically determine terminal width

    // Clear the screen for a fresh UI
    system("clear");

    // Display header
    printf("\n");
    personal_displayCenteredText("\xF0\x9F\x8E\x89 Personal Event Categories", width, MAGENTA BOLD); // 🎉
    printf("\n");
    for (int i = 0; i < width; i++) // Print top border
        printf("%s=%s", CYAN, RESET);
    printf("\n");

    // Display event categories
    int numEvents = sizeof(personal_events) / sizeof(personal_events[0]);
    for (int i = 0; i < numEvents; i++)
    {
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

void personal_bookEvent()
{
    char choiceStr[10];
    int eventChoice;
    personal_Booking newBooking;
    char confirmStr[10];
    char confirm;

    system("clear");

    // Display event categories
    personal_cat_display();

    // Get user's event choice
    printf("\n");
    personal_displayCenteredText("\xF0\x9F\x93\x8A Enter your choice: \xF0\x9F\x93\x8A", personal_getTerminalWidth(), YELLOW BOLD); // 📊
    printf("%s> %s", BOLD, RESET);
    fgets(choiceStr, sizeof(choiceStr), stdin);
    sscanf(choiceStr, "%d", &eventChoice);

    int numEvents = sizeof(personal_events) / sizeof(personal_events[0]);

    // Handle "Go Back" choice
    if (eventChoice == numEvents + 1)
    {
        personal_goBack();
        return;
    }

    // Validate choice
    if (eventChoice < 1 || eventChoice > numEvents)
    {
        personal_displayCenteredText("\xF0\x9F\x98\xA5 Invalid choice! Returning to menu... \xF0\x9F\x98\xA5", personal_getTerminalWidth(), RED); // 😥
        getchar();
        return;
    }

    // Fetch selected event details
    Event selectedEvent = personal_events[eventChoice - 1];
    strncpy(newBooking.eventName, selectedEvent.name, sizeof(newBooking.eventName) - 1);
    newBooking.eventName[sizeof(newBooking.eventName) - 1] = '\0';
    strncpy(newBooking.description, selectedEvent.description, sizeof(newBooking.description) - 1);
    newBooking.description[sizeof(newBooking.description) - 1] = '\0';
    newBooking.feePerPerson = selectedEvent.feePerPerson;

    system("clear");

    // Display selected event details
    personal_displayBanner(personal_getTerminalWidth());
    personal_displayCenteredText("\xF0\x9F\x93\x9C Event Details \xF0\x9F\x93\x9C", personal_getTerminalWidth(), CYAN BOLD); // 📜
    personal_displayBanner(personal_getTerminalWidth());
    printf("\033[1;32mEvent:\033[0m %s\n", selectedEvent.name);
    printf("\033[1;32mDescription:\033[0m %s\n", selectedEvent.description);
    printf("\033[1;32mFee per Person:\033[0m ₹%.2f\n", selectedEvent.feePerPerson);

    // Input Date
    while (1)
    {
        printf("\033[1;33mEnter Date (DD/MM/YYYY): \033[0m");
        fgets(newBooking.date, sizeof(newBooking.date), stdin);
        strtok(newBooking.date, "\n");
        if (!personal_isValidDate(newBooking.date))
        {
            personal_displayCenteredText("\xF0\x9F\x9A\xA8 Invalid date format. Try again! \xF0\x9F\x9A\xA8", personal_getTerminalWidth(), RED); // 🚨
            continue;
        }
        if (!personal_isFutureDateTime(newBooking.date, "00:00"))
        {
            personal_displayCenteredText("\xF0\x9F\x93\x86 Date must be in the future. Try again! \xF0\x9F\x93\x86", personal_getTerminalWidth(), RED); // 📆
            continue;
        }
        break;
    }

    // Input Venue
    printf("\033[1;33mEnter Venue: \033[0m");
    fgets(newBooking.venue, sizeof(newBooking.venue), stdin);
    strtok(newBooking.venue, "\n");

    // Input Time
    while (1)
    {
        printf("\033[1;33mEnter Time (HH:MM): \033[0m");
        fgets(newBooking.time, sizeof(newBooking.time), stdin);
        strtok(newBooking.time, "\n");
        if (!personal_isValidTime(newBooking.time))
        {
            personal_displayCenteredText("\xF0\x9F\x94\x8C Invalid time format. Try again! \xF0\x9F\x94\x8C", personal_getTerminalWidth(), RED); // ❌
            continue;
        }
        if (!personal_isFutureDateTime(newBooking.date, newBooking.time))
        {
            personal_displayCenteredText("\xF0\x9F\x95\x93 Time must be in the future. Try again! \xF0\x9F\x95\x93", personal_getTerminalWidth(), RED); // 🕓
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
    personal_displayBanner(personal_getTerminalWidth());
    personal_displayCenteredText("\xF0\x9F\x92\xB0 Cost Breakdown \xF0\x9F\x92\xB0", personal_getTerminalWidth(), CYAN BOLD); // 💰
    personal_displayBanner(personal_getTerminalWidth());
    printf("\033[1;32mNumber of People:\033[0m %d\n", newBooking.numberOfPeople);
    printf("\033[1;32mFee per Person:\033[0m ₹%.2f\n", newBooking.feePerPerson);
    printf("\033[1;32mTotal before GST:\033[0m ₹%.2f\n", newBooking.totalBeforeGST);
    printf("\033[1;32mGST @18%%:\033[0m ₹%.2f\n", newBooking.gstAmount);
    printf("\033[1;32mTotal Amount Payable:\033[0m ₹%.2f\n", newBooking.totalAmount);

    // Confirm booking
    personal_displayCenteredText("\xF0\x9F\x93\xA6 Confirm Booking and Proceed to Payment? (Y/N): \xF0\x9F\x93\xA6", personal_getTerminalWidth(), YELLOW); // 📦
    fgets(confirmStr, sizeof(confirmStr), stdin);
    confirm = confirmStr[0];

    if (confirm == 'Y' || confirm == 'y')
    {
        personal_displayQRCode();
        printf("\033[1;36mPlease pay ₹%.2f\033[0m\n", newBooking.totalAmount);
        personal_displayCenteredText("\xF0\x9F\x92\xB3 Payment made? (Y/N): \xF0\x9F\x92\xB3", personal_getTerminalWidth(), YELLOW); // 💳
        fgets(confirmStr, sizeof(confirmStr), stdin);
        confirm = confirmStr[0];

        if (confirm == 'Y' || confirm == 'y')
        {
            system("clear");
            personal_displayBanner(personal_getTerminalWidth());
            personal_displayCenteredText("\xF0\x9F\x93\x85 Invoice \xF0\x9F\x93\x85", personal_getTerminalWidth(), GREEN BOLD); // 🗓️
            personal_displayBanner(personal_getTerminalWidth());
            printf("\033[1;32mEvent:\033[0m %s\n", newBooking.eventName);
            printf("\033[1;32mDate:\033[0m %s\n", newBooking.date);
            printf("\033[1;32mVenue:\033[0m %s\n", newBooking.venue);
            printf("\033[1;32mTime:\033[0m %s\n", newBooking.time);
            printf("\033[1;32mTotal Amount Paid:\033[0m ₹%.2f\n", newBooking.totalAmount);
            personal_displayBanner(personal_getTerminalWidth());
            personal_displayCenteredText("\xF0\x9F\x98\x8A Thank you for your payment! \xF0\x9F\x98\x8A", personal_getTerminalWidth(), BLUE); // 😊
        }

        // Save booking and confirm
        strncpy(newBooking.status, "Approved", sizeof(newBooking.status) - 1);
        bookings[personal_bookingCount++] = newBooking;
        personal_saveBookingsToCSV();

        system("clear");
        personal_displayCenteredText("\xF0\x9F\x8E\x89 Booking Confirmed! \xF0\x9F\x8E\x89", personal_getTerminalWidth(), GREEN BOLD); // 🎉
    }
    else
    {
        personal_displayCenteredText("\xF0\x9F\x9A\xAB Booking canceled. Returning to menu... \xF0\x9F\x9A\xAB", personal_getTerminalWidth(), RED); // 🚫
    }

    printf("\033[1;33mPress Enter to return to menu...\033[0m");
    getchar();
}

void personal_viewBookings()
{
    system("clear");

    int terminalWidth = personal_getTerminalWidth(); // Dynamically get terminal width
    int columnWidth = terminalWidth / 8;             // Divide into columns (adjust as needed)

    // Manually print a line of dashes based on terminal width
    for (int i = 0; i < terminalWidth; i++)
    {
        printf("=");
    }
    printf("\n");

    // Display centered title with color and emoji
    personal_displayCenteredText("🎟️ All Bookings 🎟️", terminalWidth, MAGENTA BOLD); // 🎟️ All Bookings

    // Manually print a line of dashes based on terminal width
    for (int i = 0; i < terminalWidth; i++)
    {
        printf("=");
    }
    printf("\n");

    if (personal_bookingCount == 0)
    {
        // If no bookings found, show error message in red
        personal_displayCenteredText("❌ No bookings found. ❌", terminalWidth, RED);
    }
    else
    {
        // Table header with dynamic column widths and emojis for each column
        printf("\n");
        personal_displayCenteredText("📑 Event Details 📑", terminalWidth, YELLOW);
        printf("\n");

        // Manually print a line of dashes based on terminal width
        for (int i = 0; i < terminalWidth; i++)
        {
            printf("=");
        }
        printf("\n");

        // Print the table header
        printf("%-*s %-*s %-*s %-*s %-*s %-*s %-*s %-*s\n",
               columnWidth, "ID", columnWidth, "Event", columnWidth, "Date",
               columnWidth, "Venue", columnWidth, "Time", columnWidth,
               "No. of People", columnWidth, "Amount Paid", columnWidth, "Status");

        // Manually print a line of dashes based on terminal width
        for (int i = 0; i < terminalWidth; i++)
        {
            printf("=");
        }
        printf("\n");

        // Loop through and display all bookings in a neat table format
        for (int i = 0; i < personal_bookingCount; i++)
        {
            // Display booking details for each entry
            printf("%-*d %-*s %-*s %-*s %-*s %-*d ₹%-*0.2f %-*s\n",
                   columnWidth, i + 1, columnWidth, bookings[i].eventName,
                   columnWidth, bookings[i].date, columnWidth, bookings[i].venue,
                   columnWidth, bookings[i].time, columnWidth, bookings[i].numberOfPeople,
                   columnWidth, bookings[i].totalAmount, columnWidth, bookings[i].status);
        }
    }

    // Manually print a line of dashes based on terminal width
    for (int i = 0; i < terminalWidth; i++)
    {
        printf("=");
    }
    printf("\n");

    // Prompt user to return to the menu
    personal_displayCenteredText("🔙 Press Enter to return to menu... 🔙", terminalWidth, GREEN);
    getchar(); // Wait for user input to return to the menu
}

void personal_displayQRCode()
{
    system("clear");
    personal_printLine();
    printf("\t\t\tScan QR Code to Pay\n");
    personal_printLine();

    srand(time(0));

    for (int j = 0; j < 30; j++)
    {
        for (int i = 0; i < 30; i++)
        {
            if ((j == 2 && i > 1 && i < 9) || (j == 8 && i > 1 && i < 9) || (j == 2 && i < 28 && i > 20) || (j == 8 && i < 28 && i > 20) ||
                (i == 2 && j > 1 && j < 9) || (i == 8 && j > 1 && j < 9) || (i == 2 && j < 28 && j > 20) || (i == 8 && j < 28 && j > 20) ||
                (j == 27 && i > 1 && i < 9) || (j == 21 && i > 1 && i < 9) || (i == 27 && j > 1 && j < 9) || (i == 21 && j > 1 && j < 9) ||
                (j == 4 && i > 3 && i < 7) || (j == 6 && i > 3 && i < 7) || (j == 4 && i < 26 && i > 22) || (j == 6 && i < 26 && i > 22) ||
                (i == 4 && j > 3 && j < 7) || (i == 6 && j > 3 && j < 7) || (i == 4 && j < 26 && j > 22) || (i == 6 && j < 26 && j > 22) ||
                (j == 25 && i > 3 && i < 7) || (j == 23 && i > 3 && i < 7) || (i == 23 && j > 3 && j < 7) || (i == 23 && j > 3 && j < 7) ||
                (i == 5 && j == 5) || (i == 25 && j == 5) || (i == 24 && j == 5) || (i == 5 && j == 24))
            {
                printf("██");
            }
            else if ((j > 1 && j < 28 && i > 9 && i < 20) || (i > 1 && i < 28 && j > 9 && j < 20) || (j > 19 && j < 28 && i > 19 && i < 28))
            {
                if (rand() % 2 == 0)
                {
                    printf("██");
                }
                else
                {
                    printf("  ");
                }
            }
            else
            {
                printf("  ");
            }
        }
        printf("\n");
    }
    personal_printLine();
}

void personal_exitProgram()
{
    printf("\nExiting...\n");
    exit(0);
}

void personal_goBack()
{
    printf("\nGoing back...\n");
    printf("Press Enter to continue...");
    getchar();
}

// Function to validate date format and values
int personal_isValidDate(char *dateStr)
{
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
    if (month == 2)
    {
        // Leap year check
        int isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (day > (isLeap ? 29 : 28))
            return 0;
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        if (day > 30)
            return 0;
    }
    return 1;
}

// Function to validate time format and values
int personal_isValidTime(char *timeStr)
{
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
int personal_isFutureDateTime(char *dateStr, char *timeStr)
{
    struct tm eventTime = {0};
    time_t currentTime;
    time(&currentTime);

    int day, month, year, hour = 0, minute = 0;
    sscanf(dateStr, "%d/%d/%d", &day, &month, &year);
    if (strcmp(timeStr, "00:00") != 0)
    {
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
int personal_getIntInput(char *prompt, int min, int max)
{
    int value;
    char inputStr[20];
    while (1)
    {
        printf("%s", prompt);
        // personal_clearInputBuffer();
        fgets(inputStr, sizeof(inputStr), stdin);
        if (sscanf(inputStr, "%d", &value) != 1)
        {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        if (value < min || value > max)
        {
            printf("Please enter a number between %d and %d.\n", min, max);
            continue;
        }
        return value;
    }
}

void personal_clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void personal_freeBookingList()
{
    personal_BookingNode *current = personal_bookingList;
    while (current)
    {
        personal_BookingNode *temp = current;
        current = current->next;
        free(temp);
    }
}