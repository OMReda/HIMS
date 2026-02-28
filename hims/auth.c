#include "auth.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

int authenticateAdmin(void) {
  char username[50];
  char password[50];
  int attempts = 0;

  unsigned long targetHash = hashPassword("admin123");

  while (attempts < MAX_LOGIN_ATTEMPTS) {
    clearScreen();
    printHeader();
    printf("%s=== Admin Login ===%s\n", COLOR_YELLOW, COLOR_RESET);
    printf("%sUsername:%s ", COLOR_GREEN, COLOR_RESET);
    getLine(username, sizeof(username));
    printf("%sPassword:%s ", COLOR_GREEN, COLOR_RESET);
    getLine(password, sizeof(password));

    if (strcmp(username, "admin") == 0 &&
        hashPassword(password) == targetHash) {
      printAnimated("\nAuthenticating credentials...\n", 30, COLOR_CYAN);
      msleep(500);
      printAnimated("Login successful! Access granted.\n", 20, COLOR_GREEN);
      pauseScreen();
      return 1;
    } else {
      printf("\nInvalid credentials.\n");
      attempts++;
      printf("Attempts remaining: %d\n", MAX_LOGIN_ATTEMPTS - attempts);
      pauseScreen();
    }
  }
  printf("\nToo many failed attempts. Security lockout initiated.\n");
  return 0;
}
