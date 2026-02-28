#include "report.h"
#include "billing.h"
#include "department.h"
#include "doctor.h"
#include "patient.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

static void viewSummaryReport(void) {
  clearScreen();
  printf("=== Enterprise Analytics & Reporting ===\n\n");

  int admitted = getAdmittedCount();
  int discharged = getDischargedCount();
  int total = admitted + discharged;
  double avgStay = calculateAverageStay();
  double revenue = getTotalRevenue();

  printf("--- Patient Statistics ---\n");
  printf("Total Patients:       %d\n", total);
  printf("Currently Admitted:   %d\n", admitted);
  printf("Discharged:           %d\n", discharged);
  printf("Average Stay (Days):  %.2f\n\n", avgStay);

  printf("--- Department Statistics ---\n");
  printDepartmentStats();
  printf("\n");

  printf("--- Doctor Statistics ---\n");
  printMostActiveDoctor();
  printf("\n");

  printf("--- Financial Hub ---\n");
  printf("Total Collected Revenue: $%.2f\n\n", revenue);

  pauseScreen();
}

static void exportReport(void) {
  clearScreen();
  printf("Exporting report...\n");

  FILE *f = fopen("report_export.txt", "w");
  if (!f) {
    printf("Error: Could not create file for export.\n");
    pauseScreen();
    return;
  }

  fprintf(f, "=== HIMS Enterprise Report ===\n\n");
  fprintf(f, "Total Admitted: %d\n", getAdmittedCount());
  fprintf(f, "Total Discharged: %d\n", getDischargedCount());
  fprintf(f, "Average Stay: %.2f days\n", calculateAverageStay());
  fprintf(f, "Total Revenue: $%.2f\n", getTotalRevenue());

  fclose(f);
  printf("Report exported to 'report_export.txt' successfully.\n");
  pauseScreen();
}

void reportMenu(void) {
  int choice = 0;
  while (choice != 3) {
    clearScreen();
    printf("%s=== Reporting & Analytics ===%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s1.%s View Dashboard Summary\n", COLOR_YELLOW, COLOR_RESET);
    printf("%s2.%s Export Report to File\n", COLOR_YELLOW, COLOR_RESET);
    printf("%s3.%s Back to Main Menu\n", COLOR_YELLOW, COLOR_RESET);
    printf("%sSelect:%s ", COLOR_GREEN, COLOR_RESET);
    choice = getIntInput("");

    switch (choice) {
    case 1:
      viewSummaryReport();
      break;
    case 2:
      exportReport();
      break;
    case 3:
      break;
    default:
      printf("%sInvalid choice.%s\n", COLOR_RED, COLOR_RESET);
      pauseScreen();
    }
  }
}
