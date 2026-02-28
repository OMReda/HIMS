#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ANSI Color Codes
#define COLOR_RESET "\x1b[0m"
#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_BLUE "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN "\x1b[36m"
#define COLOR_WHITE "\x1b[37m"
#define COLOR_BOLD "\x1b[1m"

int getConsoleWidth(void);
void printCentered(const char *text);
void printHeader(void);
void enableANSI(void);
void clearScreen(void);
void pauseScreen(void);
void flushInput(void);
void getLine(char *buffer, int size);
int getIntInput(const char *prompt);
double getDoubleInput(const char *prompt);
unsigned long hashPassword(const char *str);
int calculateDaysBetween(int d1, int m1, int y1, int d2, int m2, int y2);
int getCurrentDate(int *day, int *month, int *year);

void msleep(int milliseconds);
void printAnimated(const char *text, int delay_ms, const char *color);
void showLoadingBar(const char *taskName, int duration_ms);

#endif // UTILS_H
