#ifndef HEALTH
#define HEALTH

void health_main();
void health_cat_display();
void health_bookEvent();
void health_viewAllBookings();
void health_displayQRCode(float amountDue);
void health_printLine();
void health_exitProgram();
void health_goBack();
int health_isValidDate(char *dateStr);
int health_isValidTime(char *timeStr);
int health_isFutureDateTime(char *dateStr, char *timeStr);
int health_getIntInput(char *prompt, int min, int max);
void health_clearInputBuffer();

#endif