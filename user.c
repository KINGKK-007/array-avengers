#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "health.h"
#include "personalCeleb.h"
#include "com_cul.h"
#include "edu_aca.h"
#include "prof.h"
#include "corp.h"
#include "global_user.h"

// ANSI color codes for macOS/Linux
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define BOLD "\033[1m"

void health_main();
void personal_main();
void com_main();
void edu_main();
void prof_main();
void corp_main();
int user_getTerminalWidth();
void user_displayWelcomeBanner(int width);
void user_displayCenteredText(const char *text, int width, const char *color);
int check_number_in_csv(int number);
void save_user_to_csv(const char *name, const char *city, int number, int password, const char *email);
void user_first_page();
void user_up();
void user_in();
void userLogin();

#define CSV_FILE "user_data.csv"

char name[100], city[100], email[100];
int customer_number, enter_password, reenter_password;

void save_user_to_csv(const char *name, const char *city, int number, int password, const char *email)
{
    FILE *file = fopen(CSV_FILE, "a");
    if (!file)
    {
        printf("Error: Could not open CSV file for writing.\n");
        exit(1);
    }
    fprintf(file, "%s,%s,%d,%d,%s\n", name, city, number, password, email);
    fclose(file);
}

int check_email_in_csv(const char *email)
{
    FILE *file = fopen(CSV_FILE, "r");
    if (!file)
        return 0; // If file doesn't exist, assume no duplicate

    char line[256];
    char stored_email[100];

    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%*[^,],%*[^,],%*d,%*d,%99[^,\n]", stored_email); // Extract the email
        if (strcmp(stored_email, email) == 0)
        {
            fclose(file);
            return 1; // Email found
        }
    }

    fclose(file);
    return 0; // Email not found
}

int check_number_in_csv(int number)
{
    FILE *file = fopen(CSV_FILE, "r");
    if (!file)
        return 0; // If file doesn't exist, assume no duplicate

    char line[256];
    int stored_number;

    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%*[^,],%*[^,],%d,%*d", &stored_number); // Extract the number
        if (stored_number == number)
        {
            fclose(file);
            return 1; // Number found
        }
    }

    fclose(file);
    return 0; // Number not found
}

void user_up()
{
    int width = user_getTerminalWidth(); // Get terminal width for centering

    while (1)
    {
        system("clear");
        printf("\n");
        user_displayCenteredText("\xF0\x9F\x93\x9D User Sign Up", width, CYAN); // 📝
        printf("\n%s\n", GREEN "================================================================================================================================================" RESET);

        // Prompt for name
        user_displayCenteredText("Enter your name:", width, YELLOW);
        getchar(); // Clear input buffer
        printf("%s> %s", BOLD, RESET);
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = '\0';

        // Prompt for city
        user_displayCenteredText("Enter your city:", width, YELLOW);
        printf("%s> %s", BOLD, RESET);
        fgets(city, sizeof(city), stdin);
        city[strcspn(city, "\n")] = '\0';

        // Prompt for mobile number
        user_displayCenteredText("Enter Mobile Number to register:", width, YELLOW);
        printf("%s> %s", BOLD, RESET);
        scanf("%d", &customer_number);

        // Check if number exists
        if (check_number_in_csv(customer_number))
        {
            user_displayCenteredText("\xF0\x9F\x98\x95 Number already found in database. Choose an option below:", width, RED); // 😕
            printf("\n");
            user_displayCenteredText("1️⃣. Sign in", width, CYAN);            // 1️⃣
            user_displayCenteredText("2️⃣. Create new account", width, CYAN); // 2️⃣
            user_displayCenteredText("3️⃣. Exit", width, CYAN);               // 3️⃣
            printf("\n%s> %s", BOLD, RESET);

            int choice;
            scanf("%d", &choice);

            if (choice == 1)
            {
                user_in();
                return;
            }
            else if (choice == 2)
            {
                user_up();
            }
            else if (choice == 3)
            {
                exit(0);
            }
            else
            {
                user_displayCenteredText("Invalid choice! Returning to sign-up menu.", width, RED);
                usleep(2000000); // Pause for 2 seconds
            }
        }

        // Prompt for email ID
        getchar(); // Clear newline character left in the buffer
        while (1)
        {
            user_displayCenteredText("Enter your email ID:", width, YELLOW);
            printf("%s> %s", BOLD, RESET);
            fgets(email, sizeof(email), stdin);
            email[strcspn(email, "\n")] = '\0';

            if (check_email_in_csv(email))
            {
                user_displayCenteredText("\xF0\x9F\x98\x95 Email already exists. Please enter a different email ID.", width, RED); // 😕
            }
            else
            {
                break;
            }
        }

        // Prompt for password
        while (1)
        {
            user_displayCenteredText("Enter your password:", width, YELLOW);
            printf("%s> %s", BOLD, RESET);
            scanf("%d", &enter_password);

            user_displayCenteredText("Re-enter your password:", width, YELLOW);
            printf("%s> %s", BOLD, RESET);
            scanf("%d", &reenter_password);

            if (enter_password != reenter_password)
            {
                user_displayCenteredText("\xF0\x9F\x98\x9F Passwords do not match. Please try again.", width, RED); // 😟
            }
            else
            {
                save_user_to_csv(name, city, customer_number, enter_password, email);
                user_displayCenteredText("\xF0\x9F\x8E\x89 Registration successful!", width, GREEN); // 🎉
                usleep(2000000);                                                                     // Pause for 2 seconds
                mobile_number = customer_number;  
                user_first_page();
                return;
            }
        }
    }
}

void user_in()
{
    // Get terminal width
    int width = user_getTerminalWidth();

    // Clear the screen
    system("clear");

    // Display the login banner
    user_displayWelcomeBanner(width);
    user_displayCenteredText("\xF0\x9F\x85\x9F User Login", width, CYAN); // 1️⃣

    // Variables for login
    int customer_number, password;

    // Prompt for mobile number
    printf("\n");
    user_displayCenteredText("Enter your registered Mobile Number:", width, YELLOW);
    printf("%s> %s", BOLD, RESET);
    scanf("%d", &customer_number);

    // Prompt for password
    printf("\n");
    user_displayCenteredText("Enter your password:", width, YELLOW);
    printf("%s> %s", BOLD, RESET);
    scanf("%d", &password);

    // Open the CSV file
    FILE *file = fopen(CSV_FILE, "r");
    if (!file)
    {
        printf("\n");
        user_displayCenteredText("Error: No user data found. Please sign up first.", width, RED);
        sleep(2);
        return;
    }

    // Variables to read data from the file
    char line[256], stored_name[100], stored_city[100];
    int stored_number, stored_password;

    // Read the CSV file line by line
    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%[^,],%[^,],%d,%d", stored_name, stored_city, &stored_number, &stored_password);

        // Check if the credentials match
        if (stored_number == customer_number && stored_password == password)
        {
            printf("\n");
            char welcome_msg[200];
            snprintf(welcome_msg, sizeof(welcome_msg),
                     "\xF0\x9F\x8E\x89 Login successful! Welcome back, %s from %s.", stored_name, stored_city);
            user_displayCenteredText(welcome_msg, width, GREEN); // 🎉
            fclose(file);
            sleep(2);

            mobile_number = customer_number;
            user_first_page();
            return;
        }
    }

    // If credentials are invalid
    fclose(file);
    printf("\n");
    user_displayCenteredText("\xF0\x9F\x98\xB1 Invalid credentials. Please try again or sign up if you don't have an account.", width, RED); // 😱
    sleep(2);
}

void userLogin()
{
    while (1)
    {
        int choice;

        // Get terminal width
        int width = user_getTerminalWidth();

        // Clear screen
        system("clear");

        // Display banner
        user_displayWelcomeBanner(width);

        // Menu options with emojis
        user_displayCenteredText("1️⃣. Sign in / Log in\n", width, GREEN);  // 1️⃣
        user_displayCenteredText("2️⃣. Sign up / Log up\n", width, YELLOW); // 2️⃣
        user_displayCenteredText("3️⃣. Exit\n", width, RED);                // 3️⃣

        // Prompt for user input
        printf("\n%sEnter your choice: %s", BOLD, RESET);
        scanf("%d", &choice);

        // Menu logic
        switch (choice)
        {
        case 1:
            user_in();
            break;
        case 2:
            user_up();
            break;
        case 3:
            exit(0);
        default:
            printf("\n");
            user_displayCenteredText("Invalid choice :( Please try again.", width, RED);
            sleep(2);
        }
    }
}

// Function to get the terminal width dynamically
int user_getTerminalWidth()
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
void user_displayCenteredText(const char *text, int width, const char *color)
{
    int len = strlen(text);
    int padding = (width - len) / 2; // Calculate padding for centering
    printf("%s%*s%s%s\n", color, padding, "", text, RESET);
}

// Function to display the welcome banner
void user_displayWelcomeBanner(int width)
{
    printf("\n");
    for (int i = 0; i < width; i++) // Print top border
        printf("%s=%s", CYAN, RESET);
    printf("\n");
    user_displayCenteredText("EVENT MANAGER SYSTEM", width, MAGENTA BOLD);
    for (int i = 0; i < width; i++) // Print bottom border
        printf("%s=%s", CYAN, RESET);
    printf("\n\n");
}

// Function for the user-specific menu
void user_first_page()
{
    while (1)
    {
        int choice;

        // Get terminal width
        int width = user_getTerminalWidth();

        // Clear screen
        system("clear");

        // Display banner
        user_displayWelcomeBanner(width);

        // Menu options
        user_displayCenteredText("1️⃣. Personal Development\n", width, GREEN);                           // all done.
        user_displayCenteredText("2️⃣. Community and Cultural Events\n", width, YELLOW); // done only ui
        user_displayCenteredText("3️⃣. Educational and Academic Events\n", width, CYAN); // done mine
        user_displayCenteredText("4️⃣. Professional Development\n", width, BLUE);        // done mine
        user_displayCenteredText("5️⃣. Health and Wellness Events\n", width, MAGENTA);   // all done
        user_displayCenteredText("6️⃣. Corporate Events\n", width, RED);                 // done mine

        // Prompt for user input
        printf("\n%sEnter your choice: %s", BOLD, RESET);
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            personal_main();
            printf("\n");
            sleep(2); // Wait for 2 seconds
            break;
        case 2:
            // com_main();
            printf("\n");
            sleep(2); // Wait for 2 seconds
            break;
        case 3:
            edu_main();
            printf("\n");
            sleep(2); // Wait for 2 seconds
            break;
        case 4:
            prof_main();
            printf("\n");
            sleep(2); // Wait for 2 seconds
            break;
        case 5:
            health_main();
            printf("\n");
            sleep(2); // Wait for 2 seconds
            break;
        case 6:
            corp_main();
            printf("\n");
            sleep(2); // Wait for 2 seconds
            break;
        default:
            printf("\n");
            user_displayCenteredText("Invalid choice. Please enter a number between 1 and 7.", width, RED);
            sleep(2); // Wait for 2 seconds
        }
    }
}