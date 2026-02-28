#include "pharmacy.h"
#include "file.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static Medicine *inventory = NULL;
static int medCount = 0;
static int medCapacity = 0;
static int nextMedId = 1;

static const char *FILENAME = "data/pharmacy.dat";

void initPharmacy(void) {
  inventory = (Medicine *)loadGenericData(FILENAME, sizeof(Medicine), &medCount,
                                          &medCapacity);
  for (int i = 0; i < medCount; i++) {
    if (inventory[i].id >= nextMedId) {
      nextMedId = inventory[i].id + 1;
    }
  }
}

void cleanupPharmacy(void) {
  savePharmacyData();
  if (inventory)
    free(inventory);
}

void savePharmacyData(void) {
  saveGenericData(FILENAME, inventory, sizeof(Medicine), medCount);
}

static void ensureMedCapacity(void) {
  if (medCount >= medCapacity) {
    medCapacity = (medCapacity == 0) ? 10 : medCapacity * 2;
    Medicine *temp = realloc(inventory, medCapacity * sizeof(Medicine));
    if (temp)
      inventory = temp;
  }
}

static void addMedicine(void) {
  clearScreen();
  printf("%s=== Add Medicine to Inventory ===%s\n", COLOR_CYAN, COLOR_RESET);

  char name[100];
  printf("Medicine Name: ");
  getLine(name, sizeof(name));

  int qty = getIntInput("Quantity: ");
  double price = getDoubleInput("Price per unit ($): ");

  ensureMedCapacity();
  Medicine *m = &inventory[medCount];
  m->id = nextMedId++;
  strncpy(m->name, name, sizeof(m->name) - 1);
  m->name[sizeof(m->name) - 1] = '\0';
  m->quantity = qty;
  m->price = price;

  medCount++;
  savePharmacyData();
  printAnimated("\nMedicine added successfully.\n", 15, COLOR_GREEN);
  pauseScreen();
}

static void listInventory(void) {
  clearScreen();
  printf("%s=== Pharmacy Inventory ===%s\n", COLOR_CYAN, COLOR_RESET);
  printf("%-5s | %-25s | %-10s | %-10s\n", "ID", "Name", "Stock", "Price");
  printf("----------------------------------------------------------\n");
  for (int i = 0; i < medCount; i++) {
    printf("%-5d | %-25s | %-10d | $%-9.2f\n", inventory[i].id,
           inventory[i].name, inventory[i].quantity, inventory[i].price);
  }
  pauseScreen();
}

static void dispenseMedicine(void) {
  clearScreen();
  printf("%s=== Dispense Medicine ===%s\n", COLOR_CYAN, COLOR_RESET);
  int id = getIntInput("Enter Medicine ID: ");

  for (int i = 0; i < medCount; i++) {
    if (inventory[i].id == id) {
      printf("Medicine: %s (Stock: %d, Price: $%.2f)\n", inventory[i].name,
             inventory[i].quantity, inventory[i].price);
      int qty = getIntInput("Quantity to dispense: ");
      if (qty > inventory[i].quantity) {
        printf("%sInsufficient stock!%s\n", COLOR_RED, COLOR_RESET);
      } else {
        inventory[i].quantity -= qty;
        savePharmacyData();
        printf("%sDispensed %d units. Client owes $%.2f.%s\n", COLOR_GREEN, qty,
               qty * inventory[i].price, COLOR_RESET);
      }
      pauseScreen();
      return;
    }
  }
  printf("%sMedicine not found.%s\n", COLOR_RED, COLOR_RESET);
  pauseScreen();
}

void pharmacyMenu(void) {
  int choice = 0;
  while (choice != 4) {
    clearScreen();
    printf("%s=== Pharmacy & Inventory Module ===%s\n", COLOR_CYAN,
           COLOR_RESET);
    printf("%s1.%s List Inventory\n", COLOR_YELLOW, COLOR_RESET);
    printf("%s2.%s Add Medicine Stock \n", COLOR_YELLOW, COLOR_RESET);
    printf("%s3.%s Dispense Medicine\n", COLOR_YELLOW, COLOR_RESET);
    printf("%s4.%s Back to Main Menu\n", COLOR_YELLOW, COLOR_RESET);

    printf("%sSelect an option:%s ", COLOR_GREEN, COLOR_RESET);
    choice = getIntInput("");

    switch (choice) {
    case 1:
      listInventory();
      break;
    case 2:
      addMedicine();
      break;
    case 3:
      dispenseMedicine();
      break;
    case 4:
      break;
    default:
      printf("%sInvalid choice.%s\n", COLOR_RED, COLOR_RESET);
      pauseScreen();
    }
  }
}
