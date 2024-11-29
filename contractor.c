#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define USE_EMOJIS 1

// Structure for event/task details
typedef struct {
    char category[50];
    char eventName[50];
    char deadline[20];
    char contractorID[20];
    char status[20]; // Pending, In Progress, Completed
    int progress;    // Progress percentage
} Event;

// Function prototypes
void contractorMenu(Event *events, int totalTasks, char *contractorID);
void viewTasks(Event *events, int totalTasks, char *contractorID);
void acceptOrDeclineTask(Event *events, int totalTasks, char *contractorID);
void writeEventsToFile(const char* filename, Event *events, int totalTasks);
void updateProgress(Event *events, int totalTasks, char *contractorID);
void markAsCompleted(Event *events, int totalTasks, char *contractorID);
int authenticateContractor(char *contractorID);
void contractorRegister();
void welcomeMessage();
void contractor_displayMenu();
void decorativeLine(char ch, int length);
Event* loadEventsFromFile(const char* filename, int *totalTasks);

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
void contractor_displayMenu() {
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

void ContractorLogin() {
    int choice;
    int totalTasks = 0;
    Event *events = loadEventsFromFile("event_new.csv", &totalTasks);
    char contractorID[20] = "";  // To store the logged-in contractor's ID

    welcomeMessage();

    while (1) {
        contractor_displayMenu();
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                if (authenticateContractor(contractorID)) {
                    contractorMenu(events, totalTasks, contractorID);
                }
                break;
            case 2:
                contractorRegister();
                break;
            case 3:
#if USE_EMOJIS
                printf("\n🚪 Exiting...\n");
#else
                printf("\n[EXIT] Exiting...\n");
#endif
                free(events);  // Free dynamically allocated memory
                exit(0);
            default:
#if USE_EMOJIS
                printf("\n❌ Invalid choice! Please try again.\n");
#else
                printf("\n[ERROR] Invalid choice! Please try again.\n");
#endif
        }
    }
}

// Load events from CSV file
Event* loadEventsFromFile(const char* filename, int *totalTasks) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("\n❌ Error: Could not open file %s\n", filename);
        exit(1);  // Exit if the file cannot be opened
    }

    // First count the number of lines in the CSV file
    int count = 0;
    char buffer[200];
    while (fgets(buffer, sizeof(buffer), file)) {
        count++;
    }

    // Allocate memory for events
    Event *events = (Event*)malloc(count * sizeof(Event));
    if (!events) {
        printf("\n❌ Memory allocation failed!\n");
        exit(1);
    }

    // Rewind the file to read the events
    rewind(file);

    // Read events into the array
    int i = 0;
    while (fscanf(file, "%49[^,],%49[^,],%19[^,],%19[^,],%19[^,],%d\n", 
                  events[i].category, events[i].eventName, events[i].deadline, 
                  events[i].contractorID, events[i].status, &events[i].progress) != EOF) {
        i++;
    }

    fclose(file);
    *totalTasks = count;
    return events;
}

// Contractor menu
void contractorMenu(Event *events, int totalTasks, char *contractorID) {
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
                viewTasks(events, totalTasks, contractorID);
                break;
            case 2:
                acceptOrDeclineTask(events, totalTasks, contractorID);
                break;
            case 3:
                updateProgress(events, totalTasks, contractorID);
                break;
            case 4:
                markAsCompleted(events, totalTasks, contractorID);
                break;
            case 5:
                return; // Go back to the main menu
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    }
}

// View tasks
void viewTasks(Event *events, int totalTasks, char *contractorID) {
    decorativeLine('=', 40);
    printf("ASSIGNED TASKS\n");
    decorativeLine('=', 40);

    int taskFound = 0;
    for (int i = 0; i < totalTasks; i++) {
        if (strcmp(events[i].contractorID, contractorID) == 0) {
            taskFound = 1;
            printf("Event ID: %d\n", i+1);
            printf("   Category: %s\n", events[i].category);
            printf("   Event Name: %s\n", events[i].eventName);
            printf("   Deadline: %s\n", events[i].deadline);
            printf("   Contractor ID: %s\n", events[i].contractorID);
            printf("   Status: %s\n", events[i].status);
            printf("   Progress: %d%%\n", events[i].progress);
            decorativeLine('-', 40);
        }
    }

    if (!taskFound) {
        printf("\nNo tasks assigned to you.\n");
    }

    printf("\nPress Enter to go back...");
    getchar();
    getchar();
}


// Accept or Decline a task
void acceptOrDeclineTask(Event *events, int totalTasks, char *contractorID) {
    int eventID, choice;
    printf("\nEnter the Event ID to accept/decline (or type -1 to go back): ");
    scanf("%d", &eventID);

    if (eventID == -1) return; // Go back if the user enters -1

    // Check if the event is valid and assigned to the current contractor
    if (eventID < 1 || eventID > totalTasks || strcmp(events[eventID-1].contractorID, contractorID) != 0) {
        printf("\n❌ Invalid Event ID or this event is not assigned to you.\n");
        return;
    }

    // Display event details
    printf("\nEvent Details:\n");
    printf("   Event Name: %s\n", events[eventID-1].eventName);
    printf("   Deadline: %s\n", events[eventID-1].deadline);
    printf("   Status: %s\n", events[eventID-1].status);
    printf("   Progress: %d%%\n", events[eventID-1].progress);

    // If the task has more than 50% progress, it cannot be declined
    if (events[eventID-1].progress > 50) {
        printf("\n❌ This event is more than 50%% completed and cannot be declined.\n");
        return;
    }

    // Prompt the contractor to accept or decline the task
    printf("\n1. Accept\n2. Decline\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    while(getchar() != '\n');  // Clear the input buffer

    if (choice == 1) {
        strcpy(events[eventID-1].status, "In Progress");
        printf("\n✅ Event Accepted.\n");
    } else if (choice == 2) {
        strcpy(events[eventID-1].status, "Declined");
        printf("\n🚫 Event Declined. Notifying Admin...\n");
    } else {
        printf("\nInvalid Choice.\n");
        return;
    }

    // After updating the status, write the updated events to the CSV
    writeEventsToFile("event_new.csv", events, totalTasks);

    // After accepting or declining, return to the main contractor dashboard
    printf("\nReturning to the contractor dashboard...\n");
}


// Function to write updated events back to the CSV file
// Function to write updated events back to the CSV file
void writeEventsToFile(const char* filename, Event *events, int totalTasks) {
    FILE *file = fopen(filename, "r+");  // Open file in read-write mode
    if (!file) {
        printf("\n❌ Error: Could not open file %s for reading\n", filename);
        return;
    }

    char line[256];
    int headerWritten = 0;

    // Check if the header exists
    if (fgets(line, sizeof(line), file)) {
        if (strstr(line, "Category") != NULL) {  // Check if the header is already there
            headerWritten = 1;
        }
    }
    fclose(file);

    // Reopen the file in write mode to write events
    file = fopen(filename, "w");  // Open file in write mode
    if (!file) {
        printf("\n❌ Error: Could not open file %s for writing\n", filename);
        return;
    }

    // Write the header if it's not written yet
    if (!headerWritten) {
        fprintf(file, "Category,Event Name,Deadline,Contractor ID,Status,Progress\n");
    }

    // Write the events data
    for (int i = 0; i < totalTasks; i++) {
        fprintf(file, "%s,%s,%s,%s,%s,%d\n",
                events[i].category,
                events[i].eventName,
                events[i].deadline,
                events[i].contractorID,
                events[i].status,
                events[i].progress);
    }

    fclose(file);
    printf("\n✅ Task status updated successfully and saved to the file!\n");
}





// Update task progress
// Update task progress
void updateProgress(Event *events, int totalTasks, char *contractorID) {
    int eventID, progress;
    printf("\nEnter the Event ID to update progress (or type -1 to go back): ");
    scanf("%d", &eventID);

    if (eventID == -1) return;

    if (eventID < 1 || eventID > totalTasks || strcmp(events[eventID-1].contractorID, contractorID) != 0) {
        printf("\n❌ Invalid Event ID or this event is not assigned to you.\n");
        return;
    }

    printf("Enter Progress Percentage (0-100): ");
    scanf("%d", &progress);
    if (progress >= 0 && progress <= 100) {
        events[eventID-1].progress = progress;
        if (progress < 100) {
            strcpy(events[eventID-1].status, "In Progress");
        }
        printf("\n✅ Progress Updated.\n");

        // Write the updated events to the file
        writeEventsToFile("event_new.csv", events, totalTasks);
    } else {
        printf("\nInvalid Progress Value.\n");
    }
}


// Mark task as completed
// Mark task as completed
void markAsCompleted(Event *events, int totalTasks, char *contractorID) {
    int eventID;
    printf("\nEnter the Event ID to mark as completed (or type -1 to go back): ");
    scanf("%d", &eventID);

    if (eventID == -1) return;  // Go back to the dashboard

    // Check if the event ID is valid and belongs to the contractor
    if (eventID < 1 || eventID > totalTasks || strcmp(events[eventID - 1].contractorID, contractorID) != 0) {
        printf("\n❌ Invalid Event ID or this event is not assigned to you.\n");
        return;
    }

    // Update the task's status and progress
    events[eventID - 1].progress = 100;
    strcpy(events[eventID - 1].status, "Completed");
    printf("\n🎉 Task marked as Completed! Great Job!\n");

    // Write the updated events to the file
    writeEventsToFile("event_new.csv", events, totalTasks);  // Ensure this function is correctly implemented
}


// Authenticate contractor
int authenticateContractor(char *contractorID) {
    char input[50], password[50];
    FILE *file;
    char fileID[20], filePhone[20], fileUsername[50], filePassword[50], fileCity[50], fileState[50];

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
            strcpy(contractorID, fileID);  // Store contractorID
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