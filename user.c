#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "health.h"
#include "personalCeleb.h"
#include "com_cul.h"
#include "edu_aca.h"
#include "prof.h"
#include "corp.h"

void health_main();
void personal_main();
void com_main();
void edu_main();
void prof_main();
void corp_main();

#define CSV_FILE "user_data.csv"

int check_number_in_csv(int number);
void save_user_to_csv(const char *name, const char *city, int number, int password);
void user_first_page();
void user_up();
void user_in();
void userLogin();

void save_user_to_csv(const char *name, const char *city, int number, int password) {
    FILE *file = fopen(CSV_FILE, "a");
    if (!file) {
        printf("Error: Could not open CSV file for writing.\n");
        exit(1);
    }
    fprintf(file, "%s,%s,%d,%d\n", name, city, number, password);
    fclose(file);
}

int check_number_in_csv(int number) {
    FILE *file = fopen(CSV_FILE, "r");
    if (!file) return 0; // If file doesn't exist, assume no duplicate

    char line[256];
    int stored_number;

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%*[^,],%*[^,],%d,%*d", &stored_number); // Extract the number
        if (stored_number == number) {
            fclose(file);
            return 1; // Number found
        }
    }

    fclose(file);
    return 0; // Number not found
}

void user_first_page()
{
    // while(1){
        int choice;
        printf("\n=========================================================================== Event Managner System ===========================================================================\n");
        printf("\n");
        printf("1.Personal Development\n");
        printf("2.Community and Cultural Events\n");
        printf("3.Educational and Academic Events\n");
        printf("4.Professional Development\n");
        printf("5.Health and Welness Events\n");
        printf("6.Corporate Events\n");
        printf("7.Others\n");

        scanf("%d",&choice);
        switch (choice)
        {
        case 1:
            personal_main();
            break;
        case 2:
            com_main();
            break;
        case 3:
            edu_main();
            break;
        case 4:
            prof_main();
            break;
        case 5:
            health_main();
            break;
        case 6:
            corp_main();
            break;
        case 7:
            // others();
            break;
        
        default:
            printf("Invalid choice, please try again.\n");
            break;
        }
    }
// }

void user_up()
{
    while (1)
    {
        printf("\n=========================================================================== User Sign up ===========================================================================\n");
        char name[100], city[100];
        int customer_number, enter_password, reenter_password;
        printf("Enter your name: ");
        getchar();
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = '\0';
        printf("Enter your city: ");
        fgets(city, sizeof(city), stdin);
        city[strcspn(city, "\n")] = '\0';
        printf("Enter Mobile Number to register: ");
        scanf("%d", &customer_number);
        if (check_number_in_csv(customer_number))
        {
            int choice;
            printf("Number already found in database, try signing up :)\n");
            printf("Press 1 to sign up\nPress 2 to make new account\nPress 3 to exit the system.\n");
            scanf("%d", &choice);
            if (choice == 1)
            {
                user_in();
            }
            if (choice == 2)
            {
                user_up();
            }
            if(choice == 3){
                exit(0);
            }
        }
        while(1){
                printf("Enter password: ");
            scanf("%d", &enter_password);
            printf("Re-enter password for: ");
            scanf("%d", &reenter_password);
            if (enter_password != reenter_password)
            {
                printf("Both the entries didn't matched please try again.");
            }
            else{
                save_user_to_csv(name, city, customer_number, enter_password);
                user_first_page();
                return;
            }
        }
    }
}

void user_in() {
    printf("\n=========================================================================== User Login ===========================================================================\n");
    int customer_number, password;

    printf("Enter your registered Mobile Number: ");
    scanf("%d", &customer_number);

    printf("Enter your password: ");
    scanf("%d", &password);

    FILE *file = fopen(CSV_FILE, "r");
    if (!file) {
        printf("Error: No user data found. Please sign up first.\n");
        return;
    }

    char line[256], stored_name[100], stored_city[100];
    int stored_number, stored_password;

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^,],%[^,],%d,%d", stored_name, stored_city, &stored_number, &stored_password);

        if (stored_number == customer_number && stored_password == password) {
            printf("Login successful! Welcome back, %s from %s.\n", stored_name, stored_city);
            fclose(file);
            user_first_page();
            return;
        }
    }

    fclose(file);
    printf("Invalid credentials. Please try again or sign up if you don't have an account.\n");
}

void userLogin()
{
    
    while(1)
    {    
    printf("\n=========================================================================== User Sign in/ Sign up ===========================================================================\n");
    printf("");
    printf("1.Sign in/ Login in\n");
    printf("2.Sign up/ Login up\n");
    printf("3.Exit\n");
    int choice;
    scanf("%d", &choice);
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
        printf("Invalid choice :( , please try again.\n");
    }
}}
