#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Define a buffer size for reading input
#define BUFFER_SIZE 1024
#define MAX_USERS 100
#define MAX_BOOKINGS 100
#define MAX_LINE_LENGTH 1024

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define PINK "\033[38;5;200m" 
#define WHITE "\033[0;97m"  
#define MAX_COLUMNS 20

void showTenBookings();
int compareDates(const void *a, const void *b);

// Function to split a line into columns by a comma
int split_line(char *line, char columns[MAX_COLUMNS][MAX_LINE_LENGTH]) {
    int col_count = 0;
    char *token = strtok(line, ",");
    
    while (token != NULL) {
        strncpy(columns[col_count], token, MAX_LINE_LENGTH);
        col_count++;
        token = strtok(NULL, ",");
    }
    return col_count;
}

void create_new_event_csv(const char *input_file, const char *output_file) {
    FILE *infile = fopen(input_file, "r");
    if (infile == NULL) {
        printf("Error opening input file: %s\n", input_file);
        return;
    }

    FILE *outfile = fopen(output_file, "w");
    if (outfile == NULL) {
        printf("Error opening output file: %s\n", output_file);
        fclose(infile);
        return;
    }

    char line[MAX_LINE_LENGTH];
    char columns[MAX_COLUMNS][MAX_LINE_LENGTH];

    // Write the header row to the output file
    fprintf(outfile, "Genre,Sub Genre,Date,Contractor ID,Work Status,Work Progress\n");

    // Read and process each line of the input file
    while (fgets(line, sizeof(line), infile)) {
        // Remove the newline character at the end if present
        line[strcspn(line, "\n")] = '\0';

        // Split the line into columns
        int col_count = split_line(line, columns);

        if (col_count >= 13) {
            // Extract the specific columns based on the required indices (1-based index)
            const char *genre = columns[0];              // Column 1: Genre
            const char *sub_genre = columns[1];          // Column 2: Sub Genre
            const char *date = columns[3];               // Column 4: Date
            const char *contractor_id = columns[12];     // Column 13: Contractor ID
            
            // Write the new row to the output file
            fprintf(outfile, "%s,%s,%s,%s,pending,0\n", genre, sub_genre, date, contractor_id);
        }
    }

    // Close the files
    fclose(infile);
    fclose(outfile);

    printf("New CSV file '%s' has been created successfully.\n", output_file);
}

void clearScreen() {
    // Check the OS and run the appropriate command to clear the terminal screen
    #if defined(_WIN32) || defined(_WIN64)  // For Windows 64-bit
        system("cls");
    #elif defined(__APPLE__) || defined(__MACH__)  // For macOS
        system("clear");
    #elif defined(__linux__)  // For Linux
        system("clear");
    #else
        printf("Unsupported OS\n");
    #endif
}

// Function prototypes
void signUp();
void AdminCenteredText(const char* text, int width, const char* color) {
    int padding = (width - (int)strlen(text)) / 2;
    for (int i = 0; i < padding; i++) printf(" ");
    printf("%s%s%s\n", color, text, RESET);
}

int AdmingetTerminalWidth() {
    FILE *fp = popen("tput cols", "r");
    if (!fp)
        return 80; // Default width if the healthmand fails
    int width;
    fscanf(fp, "%d", &width);
    pclose(fp);
    return width;
}
int logIn();
void displayMenu();
void viewCustomerDetails();
void viewEventDetails();
void showLatestBookings();
void showBookingsByGenre();
// void customerQuery();
// void addMessageToQueryFile(const char *customerId, const char *adminId, const char *message, const char *senderType);
void Alogin() {
    int choice;

    while (1) {
        clearScreen();
        // printf("\n" MAGENTA "Event Management System - Admin Portal" RESET "\n");
        int width = AdmingetTerminalWidth();
        for (int i = 0; i < width; i++) // Print top border
        printf("%s=%s", CYAN, RESET);
        printf("\n");
        printf("\n");
        AdminCenteredText("Event Management System - Admin Portal", width, CYAN);
        printf("\n");
        for (int i = 0; i < width; i++) // Print top border
        printf("%s=%s", CYAN, RESET);
        printf("\n");
        printf("\n");
        AdminCenteredText("[1] Sign Up (New Admin)", width, GREEN);
        AdminCenteredText("[2] Log In (Existing Admin)", width, BLUE);
        AdminCenteredText("[3] Exit\n", width, RED);
        printf(WHITE "Enter your choice: " RESET);
        scanf("%d", &choice);
        clearScreen();
        getchar(); // Clear newline character left by scanf

        switch (choice) {
            case 1:
                signUp();
                break;
            case 2:
                if (logIn()) {
                    displayMenu();
                } else {
                    printf(RED "Invalid login credentials. Please try again.\n" RESET);
                }
                break;
            case 3:
                printf(GREEN "Exiting the system. Goodbye!\n" RESET);
                system("pkill afplay");
                exit(0);
            default:
                printf(RED "Invalid choice. Please try again.\n" RESET);
        }
    }
}
// Define a structure for Booking
typedef struct {
    char genre[100];
    char subgenre[100];
    char customerPhone[20];
    char date[20];
    char status[10];
} Booking;

// Define a node in the linked list for Booking
typedef struct BookingNode {
    Booking booking;
    struct BookingNode *next;
} BookingNode;

// Function to create a new booking node
BookingNode* createNode(Booking booking) {
    BookingNode *newNode = (BookingNode*)malloc(sizeof(BookingNode));
    if (newNode) {
        newNode->booking = booking;
        newNode->next = NULL;
    }
    return newNode;
}

// Function to add a booking to the linked list
void addBooking(BookingNode **head, Booking booking) {
    BookingNode *newNode = createNode(booking);
    if (*head == NULL) {
        *head = newNode;
    } else {
        BookingNode *temp = *head;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

// Function to display bookings from the linked list
void displayBookings(BookingNode *head) {
    printf("\n"CYAN "%-20s %-25s %-15s %-19s %-10s\n", "Genre", "Subgenre", "Customer Phone", "Date", "Status");
    printf(CYAN "----------------------------------------------------------------------------\n");

    while (head) {
        printf(PINK "%-20s %-25s %-15s %-15s %-10s\n",
               head->booking.genre,
               head->booking.subgenre,
               head->booking.customerPhone,
               head->booking.date,
               head->booking.status);
        head = head->next;
    }
    printf(RESET);
}

// Function to show the latest 10 bookings from the linked list
void showTenBookings() {
    FILE *file = fopen("bookings.csv", "r");
    if (!file) {
        perror(RED "Unable to open bookings.csv");
        return;
    }

    BookingNode *head = NULL;
    char line[MAX_LINE_LENGTH];
    int bookingCount = 0;

    // Read the CSV file line by line and add each booking to the linked list
    while (fgets(line, sizeof(line), file)) {
        Booking booking;
        sscanf(line, "%99[^,],%99[^,],%19[^,],%19[^,],%9[^,]",
               booking.genre,
               booking.subgenre,
               booking.customerPhone,
               booking.date,
               booking.status);

        addBooking(&head, booking);
        bookingCount++;
    }

    fclose(file);

    // Sort the linked list based on the booking date (latest first)
    BookingNode *temp = head;
    BookingNode *arr[bookingCount];
    int i = 0;

    while (temp) {
        arr[i++] = temp;
        temp = temp->next;
    }

    qsort(arr, bookingCount, sizeof(BookingNode*), compareDates);

    // Reconstruct the sorted linked list
    head = arr[0];
    for (i = 1; i < bookingCount; i++) {
        arr[i - 1]->next = arr[i];
    }
    arr[bookingCount - 1]->next = NULL;

    // Display the sorted bookings (latest first)
    displayBookings(head);
}

void signUp() {
    char adminId[6], adminName[50], adminPhone[15], adminPassword[50];
    char buffer[BUFFER_SIZE];

    FILE *readFile = fopen("admin.csv", "r");
    if (!readFile) {
        perror("Unable to open admin.csv for reading");
        return;
    }

    printf(CYAN "Enter Admin ID (Starts with 'A' and 5 letters): " RESET);
    scanf("%5s", adminId);

    if (adminId[0] != 'A' || strlen(adminId) != 5) {
        
        printf(RED "Invalid Admin ID format. It must start with 'A' and be 5 letters long.\n" RESET);
        fclose(readFile);
        return;
    }

    while (fgets(buffer, sizeof(buffer), readFile)) {
        char *fileAdminId = strtok(buffer, ",");
        if (fileAdminId && strcmp(adminId, fileAdminId) == 0) {
            printf(RED "Admin ID %s already exists. Please try again.\n" RESET, adminId);
            fclose(readFile);
            return;
        }
    }
    fclose(readFile);

    FILE *file = fopen("admin.csv", "a");
    if (!file) {
        perror("Unable to open admin.csv for appending");
        return;
    }

    printf(CYAN "Enter Admin Name: " RESET);
    scanf(" %[^\n]", adminName);

    while (1) {
        printf(CYAN "Enter Admin Phone Number (10 digits): " RESET);
        scanf("%s", adminPhone);

        if (strlen(adminPhone) == 10 && strspn(adminPhone, "0123456789") == 10) {
            break;
        } else {
            printf(RED "Invalid phone number. Please enter exactly 10 digits.\n" RESET);
        }
    }

    printf(CYAN "Enter Admin Password: " RESET);
    scanf(" %[^\n]", adminPassword);

    fprintf(file, "%s,%s,%s,%s\n", adminId, adminName, adminPhone, adminPassword);
    fclose(file);

    printf(GREEN "Admin registered successfully!\n" RESET);
}

int logIn() {
    char adminId[6], adminPassword[50];
    char buffer[BUFFER_SIZE];
    int found = 0;

    FILE *file = fopen("admin.csv", "r");

    if (!file) {
        perror("Unable to open admin.csv");
        return 0;
    }

    printf(CYAN "Enter Admin ID: " RESET);
    scanf("%5s", adminId);

    printf(CYAN "Enter Admin Password: " RESET);
    scanf(" %[^\n]", adminPassword);
    clearScreen();

    while (fgets(buffer, sizeof(buffer), file)) {
        char *fileAdminId = strtok(buffer, ",");
        strtok(NULL, ","); 
        strtok(NULL, ","); 
        char *fileAdminPassword = strtok(NULL, "\n");

        if (fileAdminId != NULL) fileAdminId[strcspn(fileAdminId, "\r\n")] = '\0';
        if (fileAdminPassword != NULL) fileAdminPassword[strcspn(fileAdminPassword, "\r\n")] = '\0';

        if (fileAdminId != NULL && fileAdminPassword != NULL &&
            strcmp(adminId, fileAdminId) == 0 &&
            strcmp(adminPassword, fileAdminPassword) == 0) {
            found = 1;
            break;
        }
    }

    fclose(file);
    if (found) {
        int width = AdmingetTerminalWidth();
        AdminCenteredText("Login successful. Welcome, Admin!", width, GREEN);
    } else {
        int width = AdmingetTerminalWidth();
        AdminCenteredText("Invalid Admin ID or Password. Please try again.", width, RED);
    }

    return found;
}

void viewEventDetails() {
    int subChoice;
    printf("\n" CYAN "View Event Details Options" RESET "\n");
    printf(YELLOW "1. Show Latest 5 Bookings\n" RESET);
    printf(YELLOW "2. Show All Bookings by Genre\n" RESET);
    printf(CYAN "Enter your choice: " RESET);
    scanf("%d", &subChoice);
    clearScreen();
    getchar();

    switch (subChoice) {
        case 1:
            showLatestBookings();
            break;
        case 2:
            showBookingsByGenre();
            break;
        default:
            printf(RED "Invalid choice. Please try again.\n" RESET);
    }
}


void showLatestBookings() {
    FILE *file = fopen("Bookings.csv", "r");
    if (!file) {
        perror(RED "Unable to open Bookings.csv");
        return;
    }

    char buffer[1024];
    char bookings[100][15][256]; // To store up to 100 rows with 10 columns max
    int rowCount = 0;

    // Read the CSV file line by line
    while (fgets(buffer, sizeof(buffer), file)) {
        if (rowCount == 0) { // Skip the header row
            rowCount++;
            continue;
        }

        char *token = strtok(buffer, ",");
        int colIndex = 0;

        while (token != NULL) {
            strcpy(bookings[rowCount - 1][colIndex++], token);
            token = strtok(NULL, ",");
        }
        rowCount++;
    }
    fclose(file);

    // Sort the bookings by date in descending order
    for (int i = 1; i < rowCount - 1; i++) {
        for (int j = 0; j < rowCount - i - 1; j++) {
            if (strcmp(bookings[j][2], bookings[j + 1][2]) < 0) { // Compare dates
                // Swap rows
                for (int k = 0; k < 10; k++) {
                    char temp[256];
                    strcpy(temp, bookings[j][k]);
                    strcpy(bookings[j][k], bookings[j + 1][k]);
                    strcpy(bookings[j + 1][k], temp);
                }
            }
        }
    }

    // Display the latest 5 bookings
    printf("\n"CYAN "Latest 5 Bookings:\n");
    printf(CYAN "-------------------------------------------------------------------------------------------------------------------\n");
    printf(PINK "%-28s %-25s %-15s %-15s %-10s %-10s %-9s\n", "Genre", "Event Type", "Mobile No.", "Date", "Location", "Time", "Status");
    printf(CYAN "-------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < 5 && i < rowCount - 1; i++) {
        printf(PINK "%-28s %-25s %-15s %-15s %-10s %-10s %-9s\n",
               bookings[i][0], // Genre
               bookings[i][1], // Event Type
               bookings[i][2], // Mobile No.
               bookings[i][3], // Date
               bookings[i][4], // Location
               bookings[i][5], // Time
               bookings[i][10]); // Status

    }
    printf("-------------------------------------------------------------------------------------------------------------------\n");
}

void showBookingsByGenre() {
    const char *genres[] = {
        "Health and Wellness",
        "Community and Cultural",
        "Professional Development",
        "Personal Celebrations",
        "Educational and Academic",
        "Corporate"
    };
    
    int genreChoice;
    char selectedGenre[100];

    // Display genres
    printf("\n" CYAN "Available Genres:\n" RESET);
    for (int i = 0; i < 6; i++) {
        printf("%d. %s\n", i + 1, genres[i]);
    }
    printf(CYAN "Select a genre (1-6): " RESET);
    scanf("%d", &genreChoice);
    clearScreen();
    getchar();  // Consume newline after entering number

    if (genreChoice < 1 || genreChoice > 6) {
        printf(RED "Invalid genre choice. Please try again.\n" RESET);
        return;
    }

    // Set the selected genre based on user choice
    strcpy(selectedGenre, genres[genreChoice - 1]);

    FILE *file = fopen("bookings.csv", "r");
    if (file == NULL) {
        printf(RED "Could not open the file!\n");
        return;
    }

    char line[MAX_LINE_LENGTH];  // Allocate memory for the line
    int headerPrinted = 0;

    // Read each line of the CSV file
    while (fgets(line, sizeof(line), file)) {
        char genre[100], subgenre[100], date[20], location[20], time[20], status[20];

        // Parse the CSV line by separating fields by commas
        sscanf(line, "%99[^,],%99[^,],%19[^,],%19[^,],%19[^,],%*s,%*s,%*s,%*s,%19[^,],%*s",
               genre, subgenre, date, location, time, status);

        // If the genre matches the selected genre, print the row without the genre
        if (strcmp(genre, selectedGenre) == 0) {
            // Print the header once
            if (!headerPrinted) {
                printf(CYAN "----------------------------------------------------------------\n");
                printf(CYAN "%-25s %-15s %-10s %-15s\n", "Subgenre", "Mobile No.", "Venue", "Date");
                printf(CYAN "----------------------------------------------------------------\n");
                headerPrinted = 1;
            }

            // Print the booking details (subgenre, date, time, location, and status)
            printf(PINK "%-25s %-15s %-10s %-15s\n", subgenre, date, time, location);
        }
    }
    printf(CYAN "----------------------------------------------------------------\n");


    fclose(file);
}
// Function to view customer details
void viewCustomerDetails() {
    FILE *file = fopen("user.csv", "r");
    if (!file) {
        perror(RED "Unable to open user.csv");
        return;
    }

    char buffer[1024];
    int row = 0;
    
    // Print table header
    printf("\n" GREEN "Customer Details:\n" RESET);
    printf(CYAN "--------------------------------------------------------------------\n");    
    printf("%-10s | %-15s | %-15s | %-30s\n", 
           "Name", "Location", "Mobile No.", "Email");
    printf(CYAN "--------------------------------------------------------------------\n");
    
    // Read file line by line
    while (fgets(buffer, sizeof(buffer), file)) {
        if (row == 0) {
            // Skip the header row in the CSV
            row++;
            continue;
        }

        // Tokenize the line by commas
        char *name = strtok(buffer, ",");
        char *location = strtok(NULL, ",");
        char *phone = strtok(NULL, ",");
        char *pass = strtok(NULL, ",");
        char *email = strtok(NULL, "\n"); // Email is last field

        // Check if email is NULL or not
        if (email == NULL) {
            email = ""; // Set to empty string if email is NULL
        }

        // Print the customer details
        printf(PINK "%-10s | %-15s | %-15s | %-30s\n" RESET, 
               name, location, phone, email);
    }

    printf(CYAN "--------------------------------------------------------------------\n");

    fclose(file);
}


// Function to compare two dates (for sorting in descending order)
int compareDates(const void *a, const void *b) {
    return strcmp(((Booking*)b)->date, ((Booking*)a)->date);  // Sort descending (latest first)
}

void viewAllContractors() {
    FILE *file = fopen("contractors.csv", "r");
    if (!file) {
        perror(RED "Unable to open contractors.csv");
        return;
    }

    char buffer[256];
    printf("\n" GREEN "All Contractors:\n"RESET);
    printf(YELLOW "-----------------------------------------------------------------------------\n");
    printf("| %-10s | %-15s | %-20s | %-10s | %-10s |\n", "ID", "Phone", "Name", "City", "State");
    printf(YELLOW "-----------------------------------------------------------------------------\n");

    while (fgets(buffer, sizeof(buffer), file)) {
        char *contractorId = strtok(buffer, ",");
        char *phone = strtok(NULL, ",");
        char *name = strtok(NULL, ",");
        char *city = strtok(NULL, ",");
        char *state = strtok(NULL, "\n");

        if (contractorId && phone && name && city && state) {
            printf("| %-10s | %-15s | %-20s | %-10s | %-10s |\n", contractorId, phone, name, city, state);
        }
    }

    printf(YELLOW "-------------------------------------------------------------------\n");
    fclose(file);
}
void copyBookingsToEvents() {
    FILE *srcFile = fopen("bookings.csv", "r");  // Open the source file for reading
    if (srcFile == NULL) {
        perror("Unable to open bookings.csv");
        return;
    }

    FILE *destFile = fopen("events.csv", "w");  // Open the destination file for writing
    if (destFile == NULL) {
        perror("Unable to open events.csv");
        fclose(srcFile);
        return;
    }

    char line[1024];  // Buffer to read each line

    // Read each line from the source file and write it to the destination file
    while (fgets(line, sizeof(line), srcFile)) {
        fputs(line, destFile);  // Write the current line to the destination file
    }

    // Close both files
    fclose(srcFile);
    fclose(destFile);

    printf("Data has been copied from bookings.csv to events.csv successfully.\n");
}


void displayEvents() {
    FILE *file = fopen("events.csv", "r");  // Open events.csv for reading
    if (file == NULL) {
        perror("Unable to open events.csv");
        return;
    }

    char line[1024];

    // Print the header for the first four columns
    printf(CYAN"Displaying Genre, Sub-Genre/Event, Phone Number, and Date from events.csv:\n");
    printf(CYAN"----------------------------------------------------------------------------------------\n");
    printf(PINK"%-25s %-25s %-25s %-15s\n",
           "Genre", "Sub Genre/Event", "Phone Number", "Date");
    printf(CYAN"----------------------------------------------------------------------------------------\n");

    // Read and display each line
    while (fgets(line, sizeof(line), file)) {
        char *token;
        int tokenIndex = 0;

        // Tokenize the line by commas
        token = strtok(line, ",");

        // Display the first four columns (Genre, Sub Genre/Event, Phone Number, Date)
        while (token != NULL && tokenIndex < 4) {
            printf("%-25s ", token);
            token = strtok(NULL, ",");  // Get the next token (comma separated values)
            tokenIndex++;
        }
        
        printf("\n");  // Move to the next line
    }

    fclose(file);
}
void displayContractors() {
    FILE *file = fopen("contractors.csv", "r");  // Open contractors.csv for reading
    if (file == NULL) {
        perror("Unable to open contractors.csv");
        return;
    }

    char line[1024];
    printf("\n"CYAN"Displaying all contractors data from contractors.csv:\n");
    printf(CYAN"-----------------------------------------------------\n");

    // Read and display each line from contractors.csv
    while (fgets(line, sizeof(line), file)) {
        printf(PINK"%s", line);  // Print the line as is
    }

    fclose(file);
}
void assignContractorsToCustomers() {
    // Copy bookings data to events.csv if not already done
    displayEvents();

    FILE *file = fopen("events.csv", "r+");  // Open events.csv for reading and updating
    if (file == NULL) {
        perror(RED"Unable to open events.csv");
        return;
    }

    char line[1024];
    char phone[20];       // To store the phone number entered by user
    char contractorID[100];  // To store contractor ID entered by user
    int phoneFound = 0;

    // Prompt user for phone number
    printf(YELLOW"Enter phone number to assign a Contractor ID: ");
    fgets(phone, sizeof(phone), stdin);
    phone[strcspn(phone, "\n")] = 0;  // Remove the newline character from input

    // Temporary array to store all the lines
    char fileContents[1024][1024]; 
    int lineCount = 0;

    // Read the file line by line
    while (fgets(line, sizeof(line), file)) {
        // Remove the newline character at the end of the line (if present)
        line[strcspn(line, "\n")] = 0;  // Remove newline character at the end of the line

        // Copy each line into the temporary array
        strcpy(fileContents[lineCount], line); 
        lineCount++;

        // Check if the line contains the phone number
        if (strstr(line, phone)) {
            phoneFound = 1;

            // Prompt for Contractor ID
            printf(GREEN"Phone number found!\n");
            displayContractors();
            printf(YELLOW"\nEnter Contractor ID: ");
            fgets(contractorID, sizeof(contractorID), stdin);
            contractorID[strcspn(contractorID, "\n")] = 0;  // Remove newline from contractor ID input

            // Append Contractor ID to the same line (new column)
            // This ensures the contractor ID is appended to the same line without creating a new line
            snprintf(fileContents[lineCount - 1], sizeof(fileContents[lineCount - 1]), "%s,%s", line, contractorID);
        }
    }

    // If phone number was not found
    if (!phoneFound) {
        printf(RED"Phone number not found in events.csv.\n");
        clearScreen();
    }

    // Close the file before reopening it in write mode
    fclose(file); 
    file = fopen("events.csv", "w");  // Open the file in write mode to overwrite it

    // Write all the updated lines back without extra newlines between them
    for (int i = 0; i < lineCount; i++) {
        fputs(fileContents[i], file);  // Write the line (with contractor ID appended)

        // Add a newline after each line, except the last one
        if (i < lineCount - 1) {
            fputs("\n", file);  // Add newline between lines, but not after the last line
        }
    }

    fclose(file);

    if (phoneFound) {
        printf(GREEN"Contractor ID has been successfully assigned to the customer with phone number %s.\n", phone);
        create_new_event_csv("events.csv", "event_new.csv");
    }
}


void displayMenu() {
    int choice;

    while (1) {
        int width = AdmingetTerminalWidth();
        printf("\n");
        for (int i = 0; i < width; i++) // Print top border
            printf("%s=%s", CYAN, RESET);
        printf("\n");
        AdminCenteredText("Admin Menu", width, MAGENTA);
        for (int i = 0; i < width; i++) // Print top border
            printf("%s=%s", CYAN, RESET);
        printf("\n");
        AdminCenteredText("[1] Assign Contractors to Customers", width, BLUE);
        AdminCenteredText("[2] View Event Details", width, GREEN);
        AdminCenteredText("[3] View Customer Details", width, PINK);
        AdminCenteredText("[4] Show latest 10 bookings", width, CYAN);
        // AdminCenteredText("[6] Customer Query", width, CYAN);
        AdminCenteredText("[5]. Log Out", width, RED);
        printf(WHITE "Enter your choice: " RESET);
        scanf("%d", &choice);
        clearScreen();
        getchar();

        switch (choice) {
            case 1:
                printf(GREEN "Option 1: Assign Contractors to Customers selected.\n" RESET);
                assignContractorsToCustomers();
                break;
            case 2:
                viewEventDetails();
                break;
            case 3:
                viewCustomerDetails();
                break;
            case 4:
                showTenBookings();
                break;
            case 5:
                AdminCenteredText("Logging out...\n", width, RED);
                return;
                break;
            default:
                printf(RED "Invalid choice. Please try again.\n" RESET);
        }
    }
}

// void customerQuery() {
//     char customerph[10], adminId[10], message[256], choice;
//     FILE *queryFile = fopen("query.csv", "a+");
//     if (!queryFile) {
//         perror("Unable to open query.csv");
//         return;
//     }

//     printf("Enter Customer phone: ");
//     scanf("%s", customerph);

//     printf("Enter Admin ID: ");
//     scanf("%s", adminId);

//     printf("\nMessages between Customer %s and Admin %s:\n", customerph, adminId);
//     printf("--------------------------------------------------------\n");

//     // Display conversation history
//     rewind(queryFile);
//     char line[512];
//     while (fgets(line, sizeof(line), queryFile)) {
//         char *fileCustomerId = strtok(line, ",");
//         char *fileAdminId = strtok(NULL, ",");
//         char *fileMessage = strtok(NULL, ",");
//         char *fileSenderType = strtok(NULL, ",");
//         char *fileTimestamp = strtok(NULL, "\n");

//         if (strcmp(customerph, fileCustomerId) == 0 && strcmp(adminId, fileAdminId) == 0) {
//             printf("[%s] %s: %s\n", fileSenderType, fileTimestamp, fileMessage);
//         }
//     }

//     // Append new messages
//     do {
//         printf("\nEnter your message (type 'exit' to end): ");
//         scanf(" %[^\n]", message);

//         if (strcmp(message, "exit") == 0) break;

//         // Determine sender type
//         char senderType[10];
//         printf("Are you the Customer or Admin? (C/A): ");
//         scanf(" %c", &choice);
//         if (choice == 'C' || choice == 'c') {
//             strcpy(senderType, "Customer");
//         } else if (choice == 'A' || choice == 'a') {
//             strcpy(senderType, "Admin");
//         } else {
//             printf("Invalid sender type. Try again.\n");
//             continue;
//         }

//         // Append to the file
//         addMessageToQueryFile(customerph, adminId, message, senderType);
//     } while (1);

//     fclose(queryFile);
// }

// void addMessageToQueryFile(const char *customerId, const char *adminId, const char *message, const char *senderType) {
//     FILE *file = fopen("query.csv", "a");
//     if (!file) {
//         perror("Unable to open query.csv for appending");
//         return;
//     }

//     // Get the current timestamp
//     time_t now = time(NULL);
//     char timestamp[20];
//     strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));

//     fprintf(file, "%s,%s,%s,%s,%s\n", customerId, adminId, message, senderType, timestamp);
//     fclose(file);
//     printf("Message sent successfully.\n");
// }