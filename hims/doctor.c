#include "doctor.h"
#include "department.h"
#include "file.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Doctor *doctors = NULL;
static int docCount = 0;
static int docCapacity = 0;
static int nextDoctorId = 1;

static const char *FILENAME = "data/doctors.dat";

void listPatientsByDoctorExtern(const char *docName); // Exported from patient.c
int getDoctorPatientCountExtern(const char *docName); // Exported from patient.c

void initDoctors(void) {
  doctors = (Doctor *)loadGenericData(FILENAME, sizeof(Doctor), &docCount,
                                      &docCapacity);
  for (int i = 0; i < docCount; i++) {
    if (doctors[i].id >= nextDoctorId) {
      nextDoctorId = doctors[i].id + 1;
    }
  }
}

void cleanupDoctors(void) {
  saveDoctorsData();
  if (doctors) {
    free(doctors);
    doctors = NULL;
  }
}

void saveDoctorsData(void) {
  saveGenericData(FILENAME, doctors, sizeof(Doctor), docCount);
}

static void ensureDocCapacity(void) {
  if (docCount >= docCapacity) {
    docCapacity = (docCapacity == 0) ? 10 : docCapacity * 2;
    Doctor *temp = realloc(doctors, docCapacity * sizeof(Doctor));
    if (temp)
      doctors = temp;
  }
}

static void registerDoctor(void) {
  clearScreen();
  printf("=== Register Doctor ===\n");
  char name[50];
  printf("Enter doctor name: ");
  getLine(name, sizeof(name));

  char dept[50];
  printf("Enter department: ");
  getLine(dept, sizeof(dept));

  if (!checkDepartmentExists(dept)) {
    printf("\nDepartment does not exist. Please add it in Department "
           "Management first.\n");
    pauseScreen();
    return;
  }

  ensureDocCapacity();
  Doctor *d = &doctors[docCount];
  d->id = nextDoctorId++;
  strncpy(d->name, name, sizeof(d->name) - 1);
  d->name[sizeof(d->name) - 1] = '\0';
  strncpy(d->department, dept, sizeof(d->department) - 1);
  d->department[sizeof(d->department) - 1] = '\0';

  docCount++;
  incrementDepartmentDoctorCount(dept);
  saveDoctorsData();

  printf("\nDoctor registered successfully! ID: %d\n", d->id);
  pauseScreen();
}

static void listDoctorsByDepartment(void) {
  clearScreen();
  printf("=== List Doctors by Department ===\n");
  char dept[50];
  printf("Enter department name: ");
  getLine(dept, sizeof(dept));

  printf("\nDoctors in %s:\n", dept);
  printf("%-5s | %-25s\n", "ID", "Name");
  printf("-----------------------------------\n");
  int found = 0;
  for (int i = 0; i < docCount; i++) {
    if (strcmp(doctors[i].department, dept) == 0) {
      printf("%-5d | %-25s\n", doctors[i].id, doctors[i].name);
      found++;
    }
  }
  if (found == 0) {
    printf("No doctors found in this department.\n");
  }
  pauseScreen();
}

int checkDoctorExists(const char *name) {
  for (int i = 0; i < docCount; i++) {
    if (strcmp(doctors[i].name, name) == 0)
      return 1;
  }
  return 0;
}

static void viewDoctorPatients(void) {
  clearScreen();
  printf("=== View Patients under a Doctor ===\n");
  char docName[50];
  printf("Enter doctor name: ");
  getLine(docName, sizeof(docName));

  if (!checkDoctorExists(docName)) {
    printf("\nDoctor not found.\n");
    pauseScreen();
    return;
  }

  listPatientsByDoctorExtern(docName);
  pauseScreen();
}

void printMostActiveDoctor(void) {
  if (docCount == 0) {
    printf("No doctors registered.\n");
    return;
  }

  int maxPatients = -1;
  char bestDoc[50] = "";

  for (int i = 0; i < docCount; i++) {
    int cnt = getDoctorPatientCountExtern(doctors[i].name);
    if (cnt > maxPatients) {
      maxPatients = cnt;
      strcpy(bestDoc, doctors[i].name);
    }
  }

  if (maxPatients >= 0) {
    printf("Most active doctor: %s (%d patients)\n", bestDoc, maxPatients);
  }
}

void doctorMenu(void) {
  int choice = 0;
  while (choice != 4) {
    clearScreen();
    printf("%s=== Doctor Management ===%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s1.%s Register Doctor\n", COLOR_YELLOW, COLOR_RESET);
    printf("%s2.%s List Doctors by Department\n", COLOR_YELLOW, COLOR_RESET);
    printf("%s3.%s Show all patients under a doctor\n", COLOR_YELLOW,
           COLOR_RESET);
    printf("%s4.%s Back to Main Menu\n", COLOR_YELLOW, COLOR_RESET);
    printf("%sSelect an option:%s ", COLOR_GREEN, COLOR_RESET);
    choice = getIntInput("");

    switch (choice) {
    case 1:
      registerDoctor();
      break;
    case 2:
      listDoctorsByDepartment();
      break;
    case 3:
      viewDoctorPatients();
      break;
    case 4:
      break;
    default:
      printf("%sInvalid choice.%s\n", COLOR_RED, COLOR_RESET);
      pauseScreen();
    }
  }
}
