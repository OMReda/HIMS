#include "billing.h"
#include "file.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static BillingRecord *records = NULL;
static int recordCount = 0;
static int recordCapacity = 0;

static const char *FILENAME = "data/billing.dat";
static const double DAILY_RATE = 500.0;

void initBilling(void) {
  records = (BillingRecord *)loadGenericData(FILENAME, sizeof(BillingRecord),
                                             &recordCount, &recordCapacity);
}

void cleanupBilling(void) {
  saveBillingData();
  if (records) {
    free(records);
    records = NULL;
  }
}

void saveBillingData(void) {
  saveGenericData(FILENAME, records, sizeof(BillingRecord), recordCount);
}

static void ensureRecordCapacity(void) {
  if (recordCount >= recordCapacity) {
    recordCapacity = (recordCapacity == 0) ? 10 : recordCapacity * 2;
    BillingRecord *temp =
        realloc(records, recordCapacity * sizeof(BillingRecord));
    if (temp)
      records = temp;
  }
}

void generateBill(int patientId, int days) {
  ensureRecordCapacity();
  BillingRecord *r = &records[recordCount];
  r->patientId = patientId;
  r->dailyRate = DAILY_RATE;
  r->totalBill = DAILY_RATE * days;
  r->isPaid = 0; // unpaid
  recordCount++;
  saveBillingData();
}

void markBillPaid(void) {
  clearScreen();
  printf("=== Mark Bill as Paid ===\n");
  int id = getIntInput("Enter Patient ID: ");

  int found = 0;
  for (int i = 0; i < recordCount; i++) {
    if (records[i].patientId == id && !records[i].isPaid) {
      records[i].isPaid = 1;
      found = 1;
      saveBillingData();
      printf("Bill of $%.2f marked as PAID for Patient ID %d.\n",
             records[i].totalBill, id);
    }
  }

  if (!found) {
    printf("No unpaid bills found for this Patient ID.\n");
  }
  pauseScreen();
}

void viewBillHistory(void) {
  clearScreen();
  printf("=== Billing History ===\n");
  printf("%-10s | %-10s | %-12s | %-10s\n", "Patient ID", "Daily Rate",
         "Total Bill", "Status");
  printf("------------------------------------------------------\n");

  for (int i = 0; i < recordCount; i++) {
    printf("%-10d | $%-9.2f | $%-11.2f | %-10s\n", records[i].patientId,
           records[i].dailyRate, records[i].totalBill,
           records[i].isPaid ? "PAID" : "UNPAID");
  }

  pauseScreen();
}

double getTotalRevenue(void) {
  double total = 0.0;
  for (int i = 0; i < recordCount; i++) {
    if (records[i].isPaid) {
      total += records[i].totalBill;
    }
  }
  return total;
}

void billingMenu(void) {
  int choice = 0;
  while (choice != 3) {
    clearScreen();
    printf("%s=== Billing System ===%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s1.%s View Bill History\n", COLOR_YELLOW, COLOR_RESET);
    printf("%s2.%s Mark Bill as Paid\n", COLOR_YELLOW, COLOR_RESET);
    printf("%s3.%s Back to Main Menu\n", COLOR_YELLOW, COLOR_RESET);
    printf("%sSelect:%s ", COLOR_GREEN, COLOR_RESET);
    choice = getIntInput("");

    switch (choice) {
    case 1:
      viewBillHistory();
      break;
    case 2:
      markBillPaid();
      break;
    case 3:
      break;
    default:
      printf("%sInvalid choice.%s\n", COLOR_RED, COLOR_RESET);
      pauseScreen();
    }
  }
}
