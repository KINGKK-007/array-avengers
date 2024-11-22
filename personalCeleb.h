#ifndef personalCelebs
#define personalCelebs

void main_menu_personal();
void personalcat_displayMenu();
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