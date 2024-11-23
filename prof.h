#ifndef PROF
#define PROF

void prof_main();
void prof_cat_display();
void prof_bookEvent();
void prof_viewBookings();
void prof_displayQRCode(float amountDue);
void prof_printLine();
void prof_exitProgram();
void prof_goBack();
int prof_isValidDate(char *dateStr);
int prof_isValidTime(char *timeStr);
int prof_isFutureDateTime(char *dateStr, char *timeStr);
int prof_getIntInput(char *prompt, int min, int max);
void prof_clearInputBuffer();

#endif