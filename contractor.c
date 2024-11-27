#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// #include <windows.h> // For UTF-8 support commented out two lines in this code for mac

#define USE_EMOJIS 1

// Structure for event/task details
typedef struct {
    int eventID;
    char eventName[50];
    char eventType[30];
    char deadline[20];
    char status[20]; // Pending, In Progress, Completed
    int progress;    // Progress percentage
} Event;

// Function prototypes
void contractorMenu(Event events[], int totalTasks);
void viewTasks(Event events[], int totalTasks);
void acceptOrDeclineTask(Event events[], int totalTasks);
void updateProgress(Event events[], int totalTasks);
void markAsCompleted(Event events[], int totalTasks);
int authenticateContractor();
void contractorRegister();
void welcomeMessage();
void displayMenu();
void decorativeLine(char ch, int length);

// Welcome message
void welcomeMessage() {
    decorativeLine('*', 50);
    printf("*           WELCOME TO THE CONTRACTOR MENU            *\n");
    decorativeLine('*', 50);
    printf("\n");

#if USE_EMOJIS
    printf("Hello Contractor! Let’s make managing your tasks smoother than ever!\n");
#else
    printf("[INFO] Hello Contractor! Let’s make managing your tasks smoother than ever!\n");
#endif

    printf("Our system is here to assist you in managing assigned events efficiently.\n\n");
}

// Display main menu
void displayMenu() {
    decorativeLine('=', 50);
    printf("      ** CONTRACTOR MENU **\n");
    decorativeLine('=', 50);

#if USE_EMOJIS
    printf("1️⃣  Contractor Login\n");
    printf("2️⃣  Contractor Register\n");
    printf("3️⃣  Exit\n");
#else
    printf("[1] Contractor Login\n");
    printf("[2] Contractor Register\n");
    printf("[3] Exit\n");
#endif

    decorativeLine('-', 50);
    printf("Please enter your choice: ");
}

// Create decorative line
void decorativeLine(char ch, int length) {
    for (int i = 0; i < length; i++) {
        printf("%c", ch);
    }
    printf("\n");
}

int main() {
    // SetConsoleOutputCP(CP_UTF8); // Enable UTF-8 encoding for Windows console
    int choice;

    Event events[5] = {
        {1, "Wedding Event", "Outdoor", "2024-12-10", "Pending", 0},
        {2, "Corporate Meeting", "Indoor", "2024-12-15", "In Progress", 50},
        {3, "Birthday Party", "Outdoor", "2024-12-20", "Pending", 0},
        {4, "Music Concert", "Outdoor", "2024-12-25", "Pending", 0},
        {5, "Conference", "Indoor", "2024-12-30", "Completed", 100}
    };

    welcomeMessage();

    while (1) {
        displayMenu();
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                if (authenticateContractor()) {
                    contractorMenu(events, 5);
                }
                break;
            case 2:
                contractorRegister();
                break;
            case 3:
#if USE_EMOJIS
                printf("\n🚪 Exiting the system. See you next time!\n");
#else
                printf("\n[EXIT] Exiting the system. See you next time!\n");
#endif
                exit(0);
            default:
#if USE_EMOJIS
                printf("\n❌ Invalid choice! Please try again.\n");
#else
                printf("\n[ERROR] Invalid choice! Please try again.\n");
#endif
        }
    }

    return 0;
}

// Contractor menu
void contractorMenu(Event events[], int totalTasks) {
    int choice;
    while (1) {
        decorativeLine('~', 50);
        printf("CONTRACTOR DASHBOARD\n");
        decorativeLine('~', 50);

        printf("1. View Assigned Tasks\n");
        printf("2. Accept/Decline Tasks\n");
        printf("3. Update Task Progress\n");
        printf("4. Mark Task as Completed\n");
        printf("5. Back to Main Menu\n");
        decorativeLine('-', 50);
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                viewTasks(events, totalTasks);
                break;
            case 2:
                acceptOrDeclineTask(events, totalTasks);
                break;
            case 3:
                updateProgress(events, totalTasks);
                break;
            case 4:
                markAsCompleted(events, totalTasks);
                break;
            case 5:
                return; // Go back to the main menu
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    }
}

// View tasks
void viewTasks(Event events[], int totalTasks) {
    decorativeLine('=', 40);
    printf("ASSIGNED TASKS\n");
    decorativeLine('=', 40);

    for (int i = 0; i < totalTasks; i++) {
        printf("Event ID: %d\n", events[i].eventID);
        printf("   Name: %s\n", events[i].eventName);
        printf("   Type: %s\n", events[i].eventType);
        printf("   Deadline: %s\n", events[i].deadline);
        printf("   Status: %s\n", events[i].status);
        printf("   Progress: %d%%\n", events[i].progress);
        decorativeLine('-', 40);
    }

    printf("\nPress Enter to go back...");
    getchar();
    getchar();
}

// Accept or decline task
void acceptOrDeclineTask(Event events[], int totalTasks) {
    int eventID, choice;
    printf("\nEnter the Event ID to accept/decline (or type -1 to go back): ");
    scanf("%d", &eventID);

    if (eventID == -1) return;

    for (int i = 0; i < totalTasks; i++) {
        if (events[i].eventID == eventID) {
            printf("\nEvent Details:\n");
            printf("   Name: %s\n", events[i].eventName);
            printf("   Type: %s\n", events[i].eventType);
            printf("   Deadline: %s\n", events[i].deadline);
            printf("   Status: %s\n", events[i].status);
            printf("   Progress: %d%%\n", events[i].progress);

            if (events[i].progress > 50) {
                printf("\n❌ This event is more than 50%% completed and cannot be declined.\n");
                return;
            }

            printf("\n1. Accept\n2. Decline\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
            if (choice == 1) {
                strcpy(events[i].status, "In Progress");
                printf("\n✅ Event Accepted.\n");
            } else if (choice == 2) {
                strcpy(events[i].status, "Declined");
                printf("\n🚫 Event Declined. Notifying Admin...\n");
            } else {
                printf("\nInvalid Choice.\n");
            }
            return;
        }
    }
    printf("\n❌ Event ID not found.\n");
}

// Update task progress
void updateProgress(Event events[], int totalTasks) {
    int eventID, progress;
    printf("\nEnter the Event ID to update progress (or type -1 to go back): ");
    scanf("%d", &eventID);

    if (eventID == -1) return;

    for (int i = 0; i < totalTasks; i++) {
        if (events[i].eventID == eventID) {
            printf("Enter Progress Percentage (0-100): ");
            scanf("%d", &progress);
            if (progress >= 0 && progress <= 100) {
                events[i].progress = progress;
                if (progress < 100) {
                    strcpy(events[i].status, "In Progress");
                }
                printf("\n✅ Progress Updated.\n");
            } else {
                printf("\nInvalid Progress Value.\n");
            }
            return;
        }
    }
    printf("\n❌ Event ID not found.\n");
}

// Mark task as completed
void markAsCompleted(Event events[], int totalTasks) {
    int eventID;
    printf("\nEnter the Event ID to mark as completed (or type -1 to go back): ");
    scanf("%d", &eventID);

    if (eventID == -1) return;

    for (int i = 0; i < totalTasks; i++) {
        if (events[i].eventID == eventID) {
            events[i].progress = 100;
            strcpy(events[i].status, "Completed");
            printf("\n🎉 Task marked as Completed! Great Job!\n");
            return;
        }
    }
    printf("\n❌ Event ID not found.\n");
}

// Authenticate contractor
// Authenticate contractor
int authenticateContractor() {
    char input[50], password[50];
    char fileID[20], filePhone[20], fileUsername[50], filePassword[50], fileCity[50], fileState[50];
    FILE *file;

    decorativeLine('~', 50);
    printf("\nCONTRACTOR LOGIN\n");
    printf("Enter your Contractor ID or Phone Number (or type 'back' to return): ");
    scanf("%s", input);
    if (strcmp(input, "back") == 0) return 0;

    printf("Enter your password: ");
    scanf("%s", password);

    file = fopen("contractors.csv", "r");
    if (file == NULL) {
        printf("\n❌ Error: Could not open contractors.csv file. Please register first.\n");
        return 0;
    }

    // Read and compare each line in the CSV file
    while (fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^,],%s\n", fileID, filePhone, fileUsername, filePassword, fileCity, fileState) != EOF) {
        if ((strcmp(input, fileID) == 0 || strcmp(input, filePhone) == 0) && strcmp(password, filePassword) == 0) {
            fclose(file);
#if USE_EMOJIS
            printf("\n✅ Login Successful! Welcome, %s! Your Contractor ID is: %s\n", fileUsername, fileID);
#else
            printf("\n[SUCCESS] Login Successful! Welcome, %s! Your Contractor ID is: %s\n", fileUsername, fileID);
#endif
            return 1; // Login successful
        }
    }

    fclose(file);
#if USE_EMOJIS
    printf("\n❌ Invalid Credentials. Please try again.\n");
#else
    printf("\n[ERROR] Invalid Credentials. Please try again.\n");
#endif
    return 0; // Login failed
}

// Register contractor
void contractorRegister() {
    char username[50], password[50], phone[20], city[50], state[50];
    char contractorID[20];
    int idCounter = 1;
    FILE *file;

    decorativeLine('~', 50);
    printf("\nCONTRACTOR REGISTRATION\n");
    printf("Enter your name: ");
    scanf("%s", username);
    printf("Enter your desired password: ");
    scanf("%s", password);
    printf("Enter your phone number: ");
    scanf("%s", phone);
    printf("Enter your city: ");
    scanf("%s", city);
    printf("Enter your state: ");
    scanf("%s", state);

    file = fopen("contractors.csv", "r");
    if (file != NULL) {
        char fileID[20];
        while (fscanf(file, "%[^,],%*[^,],%*[^,],%*s\n", fileID) != EOF) {
            sscanf(fileID, "#con%d", &idCounter);
        }
        fclose(file);
    }
    idCounter++;

    sprintf(contractorID, "#con%d", idCounter);

    file = fopen("contractors.csv", "a");
    if (file == NULL) {
        printf("\n❌ Error opening file! Registration failed.\n");
        return;
    }

    fprintf(file, "%s,%s,%s,%s,%s,%s\n", contractorID, phone, username, password, city, state);
    fclose(file);

    printf("\n✅ Registration Successful! Your Contractor ID is: %s\n", contractorID);
}
