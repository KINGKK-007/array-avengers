#ifndef personalCelebs
#define personalCelebs

void personal_main();
void personal_cat_display();
void personal_bookEvent();
void personal_viewBookings();
void personal_displayQRCode(float amountDue);
void personal_printLine();
void personal_exitProgram();
void personal_goBack();
int personal_isValidDate(char *dateStr);
int personal_isValidTime(char *timeStr);
int personal_isFutureDateTime(char *dateStr, char *timeStr);
int personal_getIntInput(char *prompt, int min, int max);
void personal_clearInputBuffer();

#endif