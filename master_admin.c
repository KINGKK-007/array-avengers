#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h> // For sleep


#define MAX_LINE_LENGTH 200
// Function prototypes
int masterAdminLogin();
void masterAdminMenu();
void manageContractorsMenu();
void manageAdminsMenu();
void displayRevenueByGenre();
void removeContractor();
void addNewContractor();;
void viewAllCustomerContractorLinks();
void master_viewAllContractors();
void displayAllFunds();
void manageFundsMenu();
void allAdminList();
void master_viewAllContractors();
void addNewAdmin();
void manageAdminsMenu();
void removeAdmin();
void read_event_data();

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define CYAN "\033[36m"
#define BOLD "\033[1m"
#define PINK "\033[38;5;200m" 
#define MAGENTA "\033[35m"
void master_clearScreen() {
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

void masterAdminCenteredText(const char* text, int width, const char* color) {
    int padding = (width - (int)strlen(text)) / 2;
    for (int i = 0; i < padding; i++) printf(" ");
    printf("%s%s%s\n", color, text, RESET);
}

int mastergetTerminalWidth() {
    FILE *fp = popen("tput cols", "r");
    if (!fp)
        return 80; // Default width if the healthmand fails
    int width;
    fscanf(fp, "%d", &width);
    pclose(fp);
    return width;
}

void masterLogin() {
    while (1) { 
        int loggedIn = masterAdminLogin();
        if (loggedIn) {
            master_clearScreen();
            masterAdminMenu();
        }
    }

}

int masterAdminLogin(){
    master_clearScreen();
    const char masterAdminId[]="M123";
    const char masterAdminPassword[] = "passkey";
    char enteredId[10], enteredPassword[20];
     while (1) {

        int width = mastergetTerminalWidth();

        for (int i = 0; i < width; i++) // Print top border
        printf("%s=%s", CYAN, RESET);
        printf("\n");
        printf("\n");
        
        masterAdminCenteredText(BOLD CYAN "Master Admin Login", width, YELLOW);
        printf("\n");
        for (int i = 0; i < width; i++) // Print top border
        printf("%s=%s", CYAN, RESET);
        printf("\n");
        printf(YELLOW "\nEnter Master Admin ID: " RESET);
        scanf("%s", enteredId);
        printf(YELLOW "Enter Master Admin Password: " RESET);
        scanf("%s", enteredPassword);
        if (strcmp(enteredId, masterAdminId) == 0 && strcmp(enteredPassword, masterAdminPassword) == 0) {
            printf(GREEN "\n Login successful. Welcome, Master Admin!\n" RESET);
            return 1;
            master_clearScreen();
        } else {
            printf(RED "\n Invalid ID or Password. Please try again.\n" RESET);
        }
    }
}

void masterAdminMenu() {
    int choice;
    int width = mastergetTerminalWidth();
    while (1) {
        for (int i = 0; i < width; i++) // Print top border
        printf("%s=%s", CYAN, RESET);
        printf("\n");
        printf("\n");
        masterAdminCenteredText("Master Admin Menu\n", width, YELLOW);

        for (int i = 0; i < width; i++) // Print top border
        printf("%s=%s", CYAN, RESET);
        printf("\n");
        masterAdminCenteredText("[1] Manage Admins", width, BLUE);
        masterAdminCenteredText("[2] Manage Contractors", width,  GREEN);
        masterAdminCenteredText("[3] Manage Funds", width, CYAN);
        masterAdminCenteredText("[4] View all Contractors assigned to customer", width, MAGENTA);
        masterAdminCenteredText("[5] Log Out\n", width, RED);
        printf(YELLOW "Enter your choice: " RESET);
        scanf("%d", &choice);
        master_clearScreen();

        switch (choice) {

            case 1:
                manageAdminsMenu(); // Call the Manage Admins menu
                break;
            case 2:
                printf("Option 2: Manage Contractors selected.\n");
                manageContractorsMenu();
                break;
            case 3:
                printf("Option 3: Manage Funds selected.\n");
                manageFundsMenu();
                break;
            case 4:
                read_event_data();
                break;
            case 5:
                masterAdminCenteredText("Logging out...\n", width, RED);
                return;
             // Exit the menu and return to login
            default:
                masterAdminCenteredText("Invalid choice. Please try again.\n", width, RED);
        }
    }
}
void manageFundsMenu() {
    int choice;
    int width = mastergetTerminalWidth();
    while (1) {
        for (int i = 0; i < width; i++) // Print top border
        printf("%s=%s", CYAN, RESET);
        printf("\n");
        masterAdminCenteredText("Manage Funds Menu\n", width, CYAN);
        printf("\n");
        for (int i = 0; i < width; i++) // Print top border
        printf("%s=%s", CYAN, RESET);
        printf("\n");
        masterAdminCenteredText("[1] See All Funds", width, MAGENTA);
        masterAdminCenteredText("[2] Revenue by Genre", width, GREEN);
        masterAdminCenteredText("[3] Back\n", width, RED);
        printf(CYAN "Enter your choice: ");
        scanf("%d", &choice);
        master_clearScreen();

        switch (choice) {
            case 1:
                displayAllFunds();
                break;
            case 2:
                displayRevenueByGenre();
                break;
            case 3:
                return; // Exit the Manage Funds menu
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

void manageAdminsMenu() {
    int choice;
    int width = mastergetTerminalWidth();

    while (1) {
        for (int i = 0; i < width; i++) // Print top border
        printf("%s=%s", CYAN, RESET);
        printf("\n");
        printf("\n");
        masterAdminCenteredText("Manage Admins Menu", width, YELLOW);
        printf("\n");
        for (int i = 0; i < width; i++) // Print top border
        printf("%s=%s", CYAN, RESET);
        printf("\n");
        masterAdminCenteredText("[1] Remove Admin", width, RED);
        masterAdminCenteredText("[2] Add a New Admin", width, MAGENTA);
        masterAdminCenteredText("[3] All Admin List", width, GREEN);
        masterAdminCenteredText("[4] Back",width, RED);
        printf("Enter your choice: ");
        scanf("%d", &choice);
        master_clearScreen();

        switch (choice) {
            case 1:
                printf(CYAN "Option 1: Remove Admin selected.\n");
                removeAdmin();
                // Add functionality to remove an admin here
                break;
            case 2:
                printf(CYAN "Option 2: Add a New Admin selected.\n");
                addNewAdmin();
                // Add functionality to add a new admin here
                break;
            case 3:
                printf(CYAN "Option 3: All Admin List selected.\n");
                allAdminList();
                // Add functionality to display all admins here
                break;
            case 4:
                printf(CYAN "Returning to Master Admin Menu...\n");
                return; // Exit the Manage Admins menu
            default:
                printf(RED "Invalid choice. Please try again.\n");
        }
    }
}
void allAdminList() {
    FILE *file = fopen("admin.csv", "r");
    if (!file) {
        perror("Unable to open admin.csv");
        return;
    }

    char buffer[1024];
    printf(YELLOW "\nAll Admins:\n");
    printf(CYAN"-------------------------------------------------------\n");
    printf(CYAN "| %-10s | %-20s | %-15s |\n", "Admin ID", "Admin Name", "Phone Number");
    printf(CYAN "-------------------------------------------------------\n");

    while (fgets(buffer, sizeof(buffer), file)) {
        char *adminId = strtok(buffer, ",");
        char *adminName = strtok(NULL, ",");
        char *adminPhone = strtok(NULL, ",");
        char *adminPassword = strtok(NULL, "\n");

        if (adminId && adminName && adminPhone) {
            printf(PINK "| %-10s | %-20s | %-15s |\n", adminId, adminName, adminPhone);
        }
    }

    printf(CYAN "-------------------------------------------------------\n");
    fclose(file);
}

void trimWhitespace(char *str) {
    int start = 0;
    int end = strlen(str) - 1;

    // Trim leading spaces
    while (isspace((unsigned char)str[start])) {
        start++;
    }

    // Trim trailing spaces
    while (end >= start && isspace((unsigned char)str[end])) {
        end--;
    }

    // Shift the string to the left
    for (int i = 0; i <= end - start; i++) {
        str[i] = str[start + i];
    }

    str[end - start + 1] = '\0';  // Null-terminate the trimmed string
}

// Function to convert string to lowercase
void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

// Function to add a new admin
void addNewAdmin() {
    char adminId[10], adminName[50], adminPhone[15], adminPassword[50];
    int adminExists = 0;

    // Prompt for new admin details
    printf("\n" YELLOW "Enter details for the new admin:\n" RESET);
    printf(GREEN "Enter Admin ID (Starts with 'A' and 5 letters): ");
    scanf("%s", adminId);

    printf(GREEN "Enter Admin Name: ");
    scanf(" %[^\n]", adminName); // Allow spaces in the name

    printf(GREEN "Enter Admin Phone Number: ");
    scanf("%s", adminPhone);

    printf(GREEN "Enter Admin Password: ");
    scanf("%s", adminPassword);

    // Open the admin.csv file in read mode to check if the admin ID already exists
    FILE *file = fopen("admin.csv", "r");
    if (!file) {
        perror(RED "Unable to open admin.csv" RESET);
        return;
    }

    char buffer[1024];
    // Check if the admin ID already exists in the file
    while (fgets(buffer, sizeof(buffer), file)) {
        char existingAdminId[10];
        // Parse the existing admin ID from the line
        sscanf(buffer, "%s", existingAdminId);

        // Trim whitespace and convert both IDs to lowercase for case-insensitive comparison
        trimWhitespace(existingAdminId);
        toLowerCase(existingAdminId);
        char adminIdToCompare[10];
        strncpy(adminIdToCompare, adminId, sizeof(adminIdToCompare));
        trimWhitespace(adminIdToCompare);
        toLowerCase(adminIdToCompare);

        // If a matching admin ID is found, set adminExists to 1 and break
        if (strcmp(existingAdminId, adminIdToCompare) == 0) {
            adminExists = 1; // Admin ID already exists
            break;
        }
    }
    fclose(file);

    // If the admin ID already exists, print a message and return
    if (adminExists) {
        printf(RED "Admin ID %s already exists. Please use a different Admin ID.\n" RESET, adminId);
        return;
    }

    // If the admin ID doesn't exist, proceed with adding the new admin to the file
    file = fopen("admin.csv", "a");
    if (!file) {
        perror(RED "Unable to open admin.csv" RESET);
        return;
    }

    fprintf(file, "%s,%s,%s,%s\n", adminId, adminName, adminPhone, adminPassword);
    fclose(file);

    printf(PINK "New admin added successfully!\n" RESET);
}

void removeAdmin() {
    char adminIdToRemove[10];
    int found = 0;

    printf("\n"CYAN "Enter Admin ID to remove: ");
    scanf("%s", adminIdToRemove);
    master_clearScreen();

    FILE *file = fopen("admin.csv", "r");
    if (!file) {
        perror(RED "Unable to open admin.csv for reading");
        return;
    }

    FILE *tempFile = fopen("temp_admin.csv", "w");
    if (!tempFile) {
        perror(RED "Unable to open temp_admin.csv for writing");
        fclose(file);
        return;
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file)) {
        char *adminId = strtok(buffer, ",");
        char *adminName = strtok(NULL, ",");
        char *adminPhone = strtok(NULL, ",");
        char *adminPassword = strtok(NULL, "\n");

        // If admin ID matches, skip writing it to the temp file
        if (adminId && strcmp(adminId, adminIdToRemove) == 0) {
            printf("Admin ID %s removed successfully.\n", adminIdToRemove);
            found = 1;
            continue;
        }

        // Write the line to the temporary file
        if (adminId && adminName && adminPhone && adminPassword) {
            fprintf(tempFile, "%s,%s,%s,%s\n", adminId, adminName, adminPhone, adminPassword);
        }
    }

    fclose(file);
    fclose(tempFile);

    if (!found) {
        printf(RED "Admin ID %s not found.\n", adminIdToRemove);
        remove("temp_admin.csv"); // Clean up temp file if no changes made
        return;
    }

    // Replace original file with the updated temp file
    if (remove("admin.csv") != 0) {
        perror(RED "Error deleting original admin.csv");
        return;
    }

    if (rename("temp_admin.csv", "admin.csv") != 0) {
        perror(RED "Error renaming temp_admin.csv to admin.csv");
        return;
    }
}
void manageContractorsMenu() {
    int choice;
    int width = mastergetTerminalWidth();

    while (1) {

        for (int i = 0; i < width; i++) // Print top border
        printf("%s=%s", CYAN, RESET);
        printf("\n");
        printf("\n");
        masterAdminCenteredText("👷 Manage Contractors Menu", width, YELLOW);
        printf("\n");
        for (int i = 0; i < width; i++) // Print top border
        printf("%s=%s", CYAN, RESET);
        printf("\n");
        masterAdminCenteredText("[1] Add a New Contractor", width, GREEN);
        masterAdminCenteredText("[2] Remove Contractor", width, RED);
        masterAdminCenteredText("[3] View All Contractors", width, MAGENTA);
        masterAdminCenteredText("[4] Back\n", width, RED);
        printf(YELLOW "Enter your choice: " RESET);
        scanf("%d", &choice);
        master_clearScreen();

        switch (choice) {
            case 1:
                addNewContractor();
                break;
            case 2:
                removeContractor();
                break;
            case 3:
                master_viewAllContractors();
                break;
            case 4:
                printf(BLUE "Returning to Master Admin Menu...\n" RESET);
                return; 
            default:
                 printf(RED "❌ Invalid choice. Please try again.\n" RESET);
        }
    }
}
void addNewContractor() {
    char contractorId[20], phone[20], name[50], password[50], city[50], state[50];

    printf("\n"YELLOW"Enter details for the new contractor:\n");
    printf(CYAN"Enter Contractor ID (e.g., #con123): ");
    scanf("%s", contractorId);
    printf(CYAN "Enter Contractor Name: ");
    scanf(" %[^\n]", name);
    printf(CYAN "Enter Phone Number: ");
    scanf("%s", phone);
    printf(CYAN "Enter Password: ");
    scanf("%s", password);
    printf(CYAN "Enter City: ");
    scanf("%s", city);
    printf(CYAN "Enter State: ");
    scanf("%s", state);
    master_clearScreen();

    FILE *file = fopen("contractors.csv", "a");
    if (!file) {
        perror(RED "Unable to open contractors.csv");
        return;
    }

    fprintf(file, "%s,%s,%s,%s,%s,%s\n", contractorId, phone, name, password, city, state);
    fclose(file);

    printf(PINK "Contractor added successfully!\n");
}
void removeContractor() {
    char contractorIdToRemove[50];
    int found = 0;

    // Prompt for the contractor ID to remove
    printf("\n" CYAN "Enter Contractor ID to remove (e.g., #con2): ");
    scanf("%s", contractorIdToRemove);
    master_clearScreen();

    FILE *file = fopen("contractors.csv", "r");
    if (!file) {
        perror(RED "Unable to open contractor.csv for reading" RESET);
        return;
    }

    FILE *tempFile = fopen("temp_contractors.csv", "w");
    if (!tempFile) {
        perror(RED "Unable to open temp_contractor.csv for writing" RESET);
        fclose(file);
        return;
    }

    char buffer[MAX_LINE_LENGTH];
    while (fgets(buffer, sizeof(buffer), file)) {
        char contractorId[50], contractorPhone[50], contractorName[50], contractorPassword[50];
        char contractorPlace[50], contractorState[50];

        // Parse each field from the CSV line
        sscanf(buffer, "%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^\n]", 
               contractorId, contractorPhone, contractorName, contractorPassword, contractorPlace, contractorState);

        // If contractor ID matches, skip writing it to the temp file
        if (strcmp(contractorId, contractorIdToRemove) == 0) {
            printf("Contractor ID %s removed successfully.\n", contractorIdToRemove);
            found = 1;
            continue;  // Skip this contractor and don't write to the temp file
        }

        // Otherwise, write the contractor's data to the temp file
        fprintf(tempFile, "%s,%s,%s,%s,%s,%s\n", contractorId, contractorPhone, contractorName, contractorPassword, contractorPlace, contractorState);
    }

    fclose(file);
    fclose(tempFile);

    if (!found) {
        printf(RED "Contractor ID %s not found.\n" RESET, contractorIdToRemove);
        remove("temp_contractors.csv");  // Clean up temp file if no contractor was removed
        return;
    }

    // Replace the original file with the updated temp file
    if (remove("contractors.csv") != 0) {
        perror(RED "Error deleting original contractor.csv" RESET);
        return;
    }

    if (rename("temp_contractors.csv", "contractors.csv") != 0) {
        perror(RED "Error renaming temp_contractor.csv to contractor.csv" RESET);
        return;
    }

    printf("Contractor data updated successfully.\n");
}
void master_viewAllContractors() {
    FILE *file = fopen("contractors.csv", "r");
    if (!file) {
        perror(RED "Unable to open contractors.csv");
        return;
    }

    char buffer[256];
    printf("\n"CYAN"All Contractors:\n");
    printf(CYAN"---------------------------------------------------------------------------------\n");
    printf(CYAN "| %-10s | %-15s | %-20s | %-10s | %-10s |\n", "ID", "Phone", "Name", "City", "State");
    printf(CYAN "---------------------------------------------------------------------------------\n");

    while (fgets(buffer, sizeof(buffer), file)) {
        char *contractorId = strtok(buffer, ",");
        char *phone = strtok(NULL, ",");
        char *name = strtok(NULL, ",");
        char *password = strtok(NULL, ",");
        char *city = strtok(NULL, ",");
        char *state = strtok(NULL, "\n");

        if (contractorId && phone && name && city && state) {
            printf(PINK "| %-10s | %-15s | %-20s | %-10s | %-10s |\n", contractorId, phone, name, city, state);
        }
    }

    printf(CYAN "---------------------------------------------------------------------------------\n");
    fclose(file);
}
void displayAllFunds() {
    FILE *file = fopen("Bookings.csv", "r");
    if (!file) {
        perror(RED "Unable to open Bookings.csv");
        return;
    }

    char buffer[1024];
    // int isHeader = 1;

    // Mapping genre codes to descriptive names
    char *genreNames[] =  {
        RED "Health and Wellness",
        RED "Community and Cultural",
        RED "Professional Development",
        RED "Personal Celebrations",
        RED "Educational and Academic",
        RED "Corporate"
    };

    printf("\n"CYAN "All Funds (Simplified):\n");
    printf(CYAN"---------------------------------------------------------------------------------------------------------------------\n");
    printf(CYAN"%-25s %-25s %-13s %-18s %-20s %-20s\n", "Genre", "Event Name", "Phone", "Date","Before GST","After GST");
    printf(CYAN"---------------------------------------------------------------------------------------------------------------------\n");

        int isHeader = 1; // Add this to identify the first row

    while (fgets(buffer, sizeof(buffer), file)) {
        // Skip the header row
        if (isHeader) {
            isHeader = 0;
            continue;
        }

        char *columns[10]; // Assuming max 10 columns
        int colIndex = 0;

        char *token = strtok(buffer, ",");
        while (token != NULL) {
            columns[colIndex++] = token;
            token = strtok(NULL, ",");
        }

        if (colIndex < 9) {
            printf(RED "Error: Missing columns in the row.\n");
            continue;
        }

        // Map genre code to descriptive name
        for (int i = 0; i < 4; i++) {
            if (strcmp(columns[0], genreNames[i]) == 0) {
                break;
            }
        }

        // Print the simplified row with descriptive genre name and payment status
        printf(PINK "%-25s %-25s %-13s %-18s %-20s %-20s\n", columns[0], columns[1], columns[2], columns[3], columns[7], columns[9]);
    }


    printf(CYAN "---------------------------------------------------------------------------------------------------------------------\n");
    fclose(file);
}
void displayRevenueByGenre() {
    // Genres and corresponding genre codes
    char *genres[] = {
        "Health and Wellness",
        "Community and Cultural",
        "Professional Development",
        "Personal Celebrations",
        "Educational and Academic",
        "Corporate"
    };

    // Array to store the total revenue before and after GST for each genre
    double revenueBeforeGST[6] = {0}, revenueAfterGST[6] = {0};

    FILE *file = fopen("Bookings.csv", "r");

    if (!file) {
        perror("Unable to open Bookings.csv");
        return;
    }

    char line[1024];  // Buffer to read each line from the CSV

    // Skip the header line (assuming the first line is a header)
    fgets(line, sizeof(line), file);

    // Read the file line by line
    while (fgets(line, sizeof(line), file)) {
        char *token;
        char genre[100];
        double priceBeforeGST = 0, priceAfterGST = 0;
        int tokenIndex = 0;

        

        // Tokenize the line by commas
        token = strtok(line, ",");
        
        // Extract genre (1st column)
        if (token != NULL) {
            strncpy(genre, token, sizeof(genre) - 1);
            genre[sizeof(genre) - 1] = '\0';  // Ensure null termination
        }

        // Skip columns that we don't need (Phone, Date, Location, Time, etc.)
        for (tokenIndex = 0; tokenIndex < 7 && token != NULL; tokenIndex++) {
            token = strtok(NULL, ",");  // Skipping unnecessary fields
        }

        // Extract Revenue Before GST (8th column)
        if (token != NULL) {
            priceBeforeGST = atof(token);  // Convert to double
        }

        // Skip GST column (9th column)
        token = strtok(NULL, ",");  // This should be the GST column; we don't need it.

        // Extract Revenue After GST (10th column)
        token = strtok(NULL, ",");
        if (token != NULL) {
            priceAfterGST = atof(token);  // Convert to double
        }

        // Check if the genre matches the ones we care about
        for (int i = 0; i < 6; i++) {
            if (strcmp(genre, genres[i]) == 0) {
                // Add the revenue to the respective genre
                revenueBeforeGST[i] += priceBeforeGST;
                revenueAfterGST[i] += priceAfterGST;
                break;
            }
        }
    }

    // Close the file
    fclose(file);

    // Print the results
    printf("\n" GREEN "Revenue by Genre (Before and After GST):\n" RESET);
    printf(CYAN "-------------------------------------------------------------\n");
    printf(CYAN "%-25s %-20s %-20s\n", "Genre", "Revenue Before GST", "Revenue After GST");
    printf(CYAN "-------------------------------------------------------------\n");

    // Display the revenue for each genre
    for (int i = 0; i < 6; i++) {
        printf(PINK "%-25s %-20.2f %-20.2f\n", genres[i], revenueBeforeGST[i], revenueAfterGST[i]);
    }

    printf(CYAN "-------------------------------------------------------------\n");
}

void read_event_data() {
    FILE *file = fopen("events.csv", "r");  // Open the CSV file for reading
    if (!file) {
        perror("Error opening events.csv");  // Handle file open error
        return;
    }

    char buffer[1024];  // Buffer to store each line of the file

    // Print header for output
    printf("Genre                 | Sub-genre            | Phone Number  | Date       | Contractor ID\n");
    printf("--------------------------------------------------------------------------------------------\n");

    // Read each line of the file
    while (fgets(buffer, sizeof(buffer), file)) {
        // Tokenize the line by commas
        char *genre = strtok(buffer, ",");
        char *sub_genre = strtok(NULL, ",");
        char *phone = strtok(NULL, ",");
        char *date = strtok(NULL, ",");

        // Skip the next 7 columns (5 to 11)
        for (int i = 0; i < 8; i++) {
            strtok(NULL, ",");  // Skip columns 5 to 11
        }

        // Now extract the 12th column (Contractor ID)
        char *contractor_id = strtok(NULL, ",");

        // Print the required fields
        if (genre && sub_genre && phone && date && contractor_id) {
            printf("%-25s | %-25s | %-13s | %-10s | %s\n", genre, sub_genre, phone, date, contractor_id);
        }
    }

    fclose(file);  // Close the file
}
