#include <stdio.h>
#include <stdlib.h>

#include "user.h"
// #include "admin.h"
// #include "contractor.h"

void userLogin();


void MainMenu()
{
    while (1)
    {
        int choice;
        printf("\n=========================================================================== Event Managner System ===========================================================================\n");
        printf("\n");
        printf("1. User Login\n");
        printf("2. Admin Login\n");
        printf("3. Contractor Login\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            userLogin();
            break;
        case 2:
            // AdminLogin();
            break;
        case 3:
            // ContractorLogin();
            break;
        case 4:
            exit(0);
        default:
            printf("Invalid choice, please try again.\n");
        }
    }
}

int main()
{
    MainMenu();
    return 0;
}