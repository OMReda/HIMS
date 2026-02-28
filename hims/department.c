#include "department.h"
#include "file.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Department *departments = NULL;
static int deptCount = 0;
static int deptCapacity = 0;

static const char *FILENAME = "data/departments.dat";

void initDepartments(void) {
  departments = (Department *)loadGenericData(FILENAME, sizeof(Department),
                                              &deptCount, &deptCapacity);
  if (deptCount == 0 && deptCapacity > 0) {
    // Predefined departments
    strcpy(departments[0].name, "General");
    departments[0].id = 1;
    departments[0].patientCount = 0;
    departments[0].doctorCount = 0;

    strcpy(departments[1].name, "Emergency");
    departments[1].id = 2;
    departments[1].patientCount = 0;
    departments[1].doctorCount = 0;

    deptCount = 2;
    saveDepartmentsData();
  }
}

void cleanupDepartments(void) {
  saveDepartmentsData();
  if (departments) {
    free(departments);
    departments = NULL;
  }
}

void saveDepartmentsData(void) {
  saveGenericData(FILENAME, departments, sizeof(Department), deptCount);
}

static void ensureDeptCapacity(void) {
  if (deptCount >= deptCapacity) {
    deptCapacity = (deptCapacity == 0) ? 10 : deptCapacity * 2;
    Department *temp = realloc(departments, deptCapacity * sizeof(Department));
    if (temp)
      departments = temp;
  }
}

void addDepartment(void) {
  clearScreen();
  printf("=== Add New Department ===\n");
  char name[50];
  printf("Enter department name: ");
  getLine(name, sizeof(name));

  if (checkDepartmentExists(name)) {
    printf("\nDepartment already exists!\n");
    pauseScreen();
    return;
  }

  ensureDeptCapacity();
  Department *d = &departments[deptCount];
  d->id = deptCount + 1; // simple ID assignment
  strncpy(d->name, name, sizeof(d->name) - 1);
  d->name[sizeof(d->name) - 1] = '\0';
  d->patientCount = 0;
  d->doctorCount = 0;

  deptCount++;
  saveDepartmentsData();
  printf("\nDepartment added successfully.\n");
  pauseScreen();
}

void listDepartments(void) {
  clearScreen();
  printf("=== Departments ===\n");
  printf("%-5s | %-20s | %-10s | %-10s\n", "ID", "Name", "Patients", "Doctors");
  printf("----------------------------------------------------------\n");
  for (int i = 0; i < deptCount; i++) {
    printf("%-5d | %-20s | %-10d | %-10d\n", departments[i].id,
           departments[i].name, departments[i].patientCount,
           departments[i].doctorCount);
  }
  pauseScreen();
}

int checkDepartmentExists(const char *name) {
  for (int i = 0; i < deptCount; i++) {
    if (strcmp(departments[i].name, name) == 0) {
      return 1;
    }
  }
  return 0;
}

void incrementDepartmentPatientCount(const char *name) {
  for (int i = 0; i < deptCount; i++) {
    if (strcmp(departments[i].name, name) == 0) {
      departments[i].patientCount++;
      saveDepartmentsData();
      return;
    }
  }
}

void decrementDepartmentPatientCount(const char *name) {
  for (int i = 0; i < deptCount; i++) {
    if (strcmp(departments[i].name, name) == 0 &&
        departments[i].patientCount > 0) {
      departments[i].patientCount--;
      saveDepartmentsData();
      return;
    }
  }
}

void incrementDepartmentDoctorCount(const char *name) {
  for (int i = 0; i < deptCount; i++) {
    if (strcmp(departments[i].name, name) == 0) {
      departments[i].doctorCount++;
      saveDepartmentsData();
      return;
    }
  }
}

void decrementDepartmentDoctorCount(const char *name) {
  for (int i = 0; i < deptCount; i++) {
    if (strcmp(departments[i].name, name) == 0 &&
        departments[i].doctorCount > 0) {
      departments[i].doctorCount--;
      saveDepartmentsData();
      return;
    }
  }
}

void printDepartmentStats(void) {
  if (deptCount == 0) {
    printf("No departments available.\n");
    return;
  }
  printf("--- Department Stats ---\n");
  for (int i = 0; i < deptCount; i++) {
    printf("%-20s: %d Patients, %d Doctors\n", departments[i].name,
           departments[i].patientCount, departments[i].doctorCount);
  }
}

void departmentMenu(void) {
  int choice = 0;
  while (choice != 3) {
    clearScreen();
    printf("%s=== Department Management ===%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s1.%s List Departments\n", COLOR_YELLOW, COLOR_RESET);
    printf("%s2.%s Add Department\n", COLOR_YELLOW, COLOR_RESET);
    printf("%s3.%s Back to Main Menu\n", COLOR_YELLOW, COLOR_RESET);
    printf("%sSelect an option:%s ", COLOR_GREEN, COLOR_RESET);
    choice = getIntInput("");

    switch (choice) {
    case 1:
      listDepartments();
      break;
    case 2:
      addDepartment();
      break;
    case 3:
      break;
    default:
      printf("%sInvalid choice.%s\n", COLOR_RED, COLOR_RESET);
      pauseScreen();
    }
  }
}
