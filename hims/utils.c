#include "utils.h"
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

int getConsoleWidth(void) {
#ifdef _WIN32
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
#endif
  return 80; /* fallback */
}

void printCentered(const char *text) {
  int width = getConsoleWidth();
  int len = (int)strlen(text);
  int pad = (width - len) / 2;
  if (pad < 0)
    pad = 0;
  printf("%*s%s\n", pad, "", text);
}

void printHeader(void) {
  int width = getConsoleWidth();
  char border[256];
  int i;
  for (i = 0; i < width - 1 && i < 254; i++)
    border[i] = '=';
  border[i] = '\0';

  /* Logo lines — each is 29 visible chars wide */
  const char *logo[] = {
      "  _   _  _____ __  __  _____  ",  " | | | ||_   _|  \\/  |/ ____| ",
      " | |_| |  | | | \\  / | (___   ", " |  _  |  | | | |\\/| |\\___ \\  ",
      " | | | | _| |_| |  | |____) | ",  " |_| |_||_____|_|  |_|_____/  "};
  const char *subtitle = "ENTERPRISE HOSPITAL INFORMATION MANAGEMENT SYSTEM";

  printf("%s%s%s\n", COLOR_CYAN, border, COLOR_RESET);
  for (i = 0; i < 6; i++)
    printCentered(logo[i]);
  printf("\n");
  printCentered(subtitle);
  printf("%s%s%s\n\n", COLOR_CYAN, border, COLOR_RESET);
}

void enableANSI(void) {
#ifdef _WIN32
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  if (hOut == INVALID_HANDLE_VALUE)
    return;
  DWORD dwMode = 0;
  if (!GetConsoleMode(hOut, &dwMode))
    return;
  dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  SetConsoleMode(hOut, dwMode);
#endif
}

void clearScreen(void) {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

void pauseScreen(void) {
  printf("\nPress Enter to continue...");
  flushInput();
}

void flushInput(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

void getLine(char *buffer, int size) {
  if (fgets(buffer, size, stdin) != NULL) {
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
      buffer[len - 1] = '\0';
    } else {
      flushInput();
    }
  }
}

int getIntInput(const char *prompt) {
  int value;
  printf("%s", prompt);
  while (scanf("%d", &value) != 1) {
    printf("Invalid input. Please enter an integer: ");
    flushInput();
  }
  flushInput();
  return value;
}

double getDoubleInput(const char *prompt) {
  double value;
  printf("%s", prompt);
  while (scanf("%lf", &value) != 1) {
    printf("Invalid input. Please enter a decmial number: ");
    flushInput();
  }
  flushInput();
  return value;
}

unsigned long hashPassword(const char *str) {
  unsigned long hash = 5381;
  int c;
  while ((c = *str++)) {
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  }
  return hash;
}

// Simple day calculation mapping (ignores complex leap logic for brevity, uses
// average) A production system would use mktime but let's implement basic for
// simplicity or use mktime.
int calculateDaysBetween(int d1, int m1, int y1, int d2, int m2, int y2) {
  struct tm start = {0};
  struct tm end = {0};

  start.tm_mday = d1;
  start.tm_mon = m1 - 1;
  start.tm_year = y1 - 1900;

  end.tm_mday = d2;
  end.tm_mon = m2 - 1;
  end.tm_year = y2 - 1900;

  time_t time1 = mktime(&start);
  time_t time2 = mktime(&end);

  if (time1 == (time_t)-1 || time2 == (time_t)-1) {
    return 0;
  }

  double difference = difftime(time2, time1) / (60 * 60 * 24);
  if (difference < 0)
    difference = 0;
  return (int)difference;
}

int getCurrentDate(int *day, int *month, int *year) {
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  *day = tm.tm_mday;
  *month = tm.tm_mon + 1;
  *year = tm.tm_year + 1900;
  return 1;
}

void msleep(int milliseconds) {
#ifdef _WIN32
  Sleep(milliseconds);
#else
  struct timespec ts;
  ts.tv_sec = milliseconds / 1000;
  ts.tv_nsec = (milliseconds % 1000) * 1000000;
  nanosleep(&ts, NULL);
#endif
}

void printAnimated(const char *text, int delay_ms, const char *color) {
  if (color)
    printf("%s", color);
  while (*text) {
    putchar(*text++);
    fflush(stdout);
    msleep(delay_ms);
  }
  if (color)
    printf("%s", COLOR_RESET);
}

void showLoadingBar(const char *taskName, int duration_ms) {
  int steps = 20;
  int sleep_time = duration_ms / steps;
  printf("%s%s...%s [", COLOR_CYAN, taskName, COLOR_RESET);
  for (int i = 0; i < steps; i++) {
    printf("%s#%s", COLOR_GREEN, COLOR_RESET);
    fflush(stdout);
    msleep(sleep_time);
  }
  printf("] %sDone!%s\n", COLOR_GREEN, COLOR_RESET);
  msleep(200);
}
