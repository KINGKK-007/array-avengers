#ifndef PROF
#define PROF

void corp_main();
void corp_cat_display();
void corp_bookEvent();
void corp_viewBookings();
void corp_displayQRCode(float amountDue);
void corp_printLine();
void corp_exitProgram();
void corp_goBack();
int corp_isValidDate(char *dateStr);
int corp_isValidTime(char *timeStr);
int corp_isFutureDateTime(char *dateStr, char *timeStr);
int corp_getIntInput(char *prompt, int min, int max);
void corp_clearInputBuffer();

#endif