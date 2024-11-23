#ifndef COM_CUL
#define COM_CUL

void com_main();
void com_cat_display();
void com_bookEvent();
void com_viewBookings();
void com_displayQRCode(float amountDue);
void com_printLine();
void com_exitProgram();
void com_goBack();
int com_isValidDate(char *dateStr);
int com_isValidTime(char *timeStr);
int com_isFutureDateTime(char *dateStr, char *timeStr);
int com_getIntInput(char *prompt, int min, int max);
void com_clearInputBuffer();

#endif