#ifndef HEALTH
#define HEALTH

void main_menu_health();
void healthcat_displayMenu();
void bookEvent();
void viewBookings();
void displayQRCode(float amountDue);
void printLine();
void exitProgram();
void goBack();
int isValidDate(char *dateStr);
int isValidTime(char *timeStr);
int isFutureDateTime(char *dateStr, char *timeStr);
int getIntInput(char *prompt, int min, int max);
void clearInputBuffer();

#endif