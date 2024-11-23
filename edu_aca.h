#ifndef EDU
#define EDU

void edu_main();
void edu_cat_display();
void edu_bookEvent();
void edu_viewBookings();
void edu_displayQRCode(float amountDue);
void edu_printLine();
void edu_exitProgram();
void edu_goBack();
int edu_isValidDate(char *dateStr);
int edu_isValidTime(char *timeStr);
int edu_isFutureDateTime(char *dateStr, char *timeStr);
int edu_getIntInput(char *prompt, int min, int max);
void edu_clearInputBuffer();

#endif