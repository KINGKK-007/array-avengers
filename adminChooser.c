#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For sleep
#include <string.h> // For string manipulation

void masterLogin();
void Alogin();

// Define color codes
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define BOLD "\033[1m"


void adminLogin() {
    int choice;

    while (1) {
        printf("\n" BOLD BLUE "--- Main Menu ---\n" RESET);
        printf(BLUE "1. Master Admin\n" RESET);
        printf(BLUE "2. Admin\n" RESET);
        printf(BLUE "3. Exit\n" RESET);
        printf(BOLD "Enter your choice: " RESET);
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                masterLogin();
            break;
            case 2:
                Alogin();
            break;
            case 3:
                printf("\n" BOLD RED "Exiting program. Goodbye!\n" RESET);
                system("pkill afplay");
                exit(0);
            default:
                printf("\n" BOLD RED "Invalid choice. Please try again.\n" RESET);
        }
    }
}