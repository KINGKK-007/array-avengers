#ifndef USER
#define USER

void userLogin();
int user_getTerminalWidth();
void user_displayWelcomeBanner(int width);
void user_displayCenteredText(const char *text, int width, const char *color);

#endif