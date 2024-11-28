#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For sleep
#include <string.h> // For string manipulation

// ANSI color codes for macOS/Linux
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define BOLD "\033[1m"

// Function declarations
void userLogin();
void displayWelcomeBanner(int width);
void displayCenteredText(const char *text, int width, const char *color);
int getTerminalWidth();
void MainMenu();

// Function to get the terminal width dynamically
int getTerminalWidth()
{
    // Use `tput cols` to get the terminal width
    FILE *fp = popen("tput cols", "r");
    if (!fp)
        return 80; // Default width if the command fails
    int width;
    fscanf(fp, "%d", &width);
    pclose(fp);
    return width;
}

// Function to display centered text
void displayCenteredText(const char *text, int width, const char *color)
{
    int len = strlen(text);
    int padding = (width - len) / 2; // Calculate padding for centering
    printf("%s%*s%s%s\n", color, padding, "", text, RESET);
}

// Function to display the welcome banner
void displayWelcomeBanner(int width)
{
    printf("\n");
    for (int i = 0; i < width; i++) // Print top border
        printf("%s=%s", CYAN, RESET);
    printf("\n");
    displayCenteredText("EVENT MANAGER SYSTEM", width, MAGENTA BOLD);
    for (int i = 0; i < width; i++) // Print bottom border
        printf("%s=%s", CYAN, RESET);
    printf("\n\n");
}

// Main menu function
void MainMenu()
{
    while (1)
    {
        int choice;

        // Get terminal width
        int width = getTerminalWidth();

        // Clear screen
        system("clear");

        // Display banner
        displayWelcomeBanner(width);

        // Menu options
        displayCenteredText("1️⃣. User Login\n", width, GREEN);
        displayCenteredText("2️⃣. Admin Login\n", width, YELLOW);
        displayCenteredText("3️⃣. Contractor Login\n", width, BLUE);
        displayCenteredText("4️⃣. Exit\n", width, RED);

        printf("\n%sEnter your choice: %s", BOLD, RESET);
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            userLogin();
            break;
        case 2:
            // AdminLogin();
            printf("\n");
            displayCenteredText("[Feature Coming Soon: Admin Login]", width, CYAN);
            sleep(2); // Wait for 2 seconds
            break;
        case 3:
            // ContractorLogin();
            printf("\n");
            displayCenteredText("[Feature Coming Soon: Contractor Login]", width, CYAN);
            sleep(2); // Wait for 2 seconds
            break;
        case 4:
            printf("\n");
            displayCenteredText("Thank you for using Event Manager System. Goodbye!", width, MAGENTA);
            exit(0);
        default:
            printf("\n");
            displayCenteredText("Invalid choice. Please enter a number between 1 and 4.", width, RED);
            sleep(2); // Wait for 2 seconds
        }
    }
}

// Main function
int main()
{
    MainMenu();
    return 0;
}
