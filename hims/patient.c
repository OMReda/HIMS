#include "patient.h"
#include "billing.h"
#include "department.h"
#include "doctor.h"
#include "file.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Patient *patients = NULL;
static int patientCount = 0;
static int patientCapacity = 0;
static int nextPatientId = 1;

static const char *FILENAME = "data/patients.dat";

void initPatients(void) {
  patients = (Patient *)loadGenericData(FILENAME, sizeof(Patient),
                                        &patientCount, &patientCapacity);
  for (int i = 0; i < patientCount; i++) {
    if (patients[i].id >= nextPatientId) {
      nextPatientId = patients[i].id + 1;
    }
  }
}

void cleanupPatients(void) {
  savePatientsData();
  if (patients) {
    free(patients);
    patients = NULL;
  }
}

void savePatientsData(void) {
  saveGenericData(FILENAME, patients, sizeof(Patient), patientCount);
}

static void ensurePatientCapacity(void) {
  if (patientCount >= patientCapacity) {
    patientCapacity = (patientCapacity == 0) ? 10 : patientCapacity * 2;
    Patient *temp = realloc(patients, patientCapacity * sizeof(Patient));
    if (temp)
      patients = temp;
  }
}

static void registerPatient(void) {
  clearScreen();
  printf("=== Register Patient ===\n");
  char name[50], dept[50], doc[50], diag[150];
  int age;

  printf("Name: ");
  getLine(name, sizeof(name));
  age = getIntInput("Age: ");
  printf("Diagnosis: ");
  getLine(diag, sizeof(diag));
  printf("Department: ");
  getLine(dept, sizeof(dept));

  if (!checkDepartmentExists(dept)) {
    printf("Department not found. Please add it first.\n");
    pauseScreen();
    return;
  }

  printf("Doctor Name: ");
  getLine(doc, sizeof(doc));
  if (!checkDoctorExists(doc)) {
    printf("Doctor not found. Please register doctor first.\n");
    pauseScreen();
    return;
  }

  ensurePatientCapacity();
  Patient *p = &patients[patientCount];
  p->id = nextPatientId++;
  strncpy(p->name, name, sizeof(p->name) - 1);
  p->name[sizeof(p->name) - 1] = '\0';
  p->age = age;
  strncpy(p->diagnosis, diag, sizeof(p->diagnosis) - 1);
  p->diagnosis[sizeof(p->diagnosis) - 1] = '\0';
  strncpy(p->department, dept, sizeof(p->department) - 1);
  p->department[sizeof(p->department) - 1] = '\0';
  strncpy(p->doctorName, doc, sizeof(p->doctorName) - 1);
  p->doctorName[sizeof(p->doctorName) - 1] = '\0';

  getCurrentDate(&p->admissionDate.day, &p->admissionDate.month,
                 &p->admissionDate.year);
  p->dischargeDate.day = 0;
  p->dischargeDate.month = 0;
  p->dischargeDate.year = 0;
  p->isAdmitted = 1;

  patientCount++;
  incrementDepartmentPatientCount(dept);
  savePatientsData();

  printf("\nPatient Registered! ID: %d\n", p->id);
  pauseScreen();
}

static void updatePatient(void) {
  int id = getIntInput("Enter Patient ID to update: ");
  for (int i = 0; i < patientCount; i++) {
    if (patients[i].id == id) {
      printf("Enter new Diagnosis (leave empty to keep current): ");
      char diag[150];
      getLine(diag, sizeof(diag));
      if (strlen(diag) > 0) {
        strncpy(patients[i].diagnosis, diag, sizeof(patients[i].diagnosis) - 1);
        patients[i].diagnosis[sizeof(patients[i].diagnosis) - 1] = '\0';
        savePatientsData();
        printf("Record updated.\n");
      }
      pauseScreen();
      return;
    }
  }
  printf("Patient not found.\n");
  pauseScreen();
}

static void dischargePatient(void) {
  int id = getIntInput("Enter Patient ID to discharge: ");
  for (int i = 0; i < patientCount; i++) {
    if (patients[i].id == id) {
      if (!patients[i].isAdmitted) {
        printf("Already discharged.\n");
        pauseScreen();
        return;
      }
      patients[i].isAdmitted = 0;
      getCurrentDate(&patients[i].dischargeDate.day,
                     &patients[i].dischargeDate.month,
                     &patients[i].dischargeDate.year);

      int days = calculateDaysBetween(
          patients[i].admissionDate.day, patients[i].admissionDate.month,
          patients[i].admissionDate.year, patients[i].dischargeDate.day,
          patients[i].dischargeDate.month, patients[i].dischargeDate.year);
      if (days == 0)
        days = 1;

      decrementDepartmentPatientCount(patients[i].department);
      savePatientsData();

      generateBill(id, days);
      printf("Patient discharged. Billed for %d days.\n", days);
      pauseScreen();
      return;
    }
  }
  printf("Patient not found.\n");
  pauseScreen();
}

static void readmitPatient(void) {
  int id = getIntInput("Enter Patient ID to re-admit: ");
  for (int i = 0; i < patientCount; i++) {
    if (patients[i].id == id) {
      if (patients[i].isAdmitted) {
        printf("Patient is already admitted.\n");
        pauseScreen();
        return;
      }
      patients[i].isAdmitted = 1;
      getCurrentDate(&patients[i].admissionDate.day,
                     &patients[i].admissionDate.month,
                     &patients[i].admissionDate.year);
      patients[i].dischargeDate.day = 0;
      patients[i].dischargeDate.month = 0;
      patients[i].dischargeDate.year = 0;

      incrementDepartmentPatientCount(patients[i].department);
      savePatientsData();
      printf("Patient re-admitted successfully.\n");
      pauseScreen();
      return;
    }
  }
  printf("Patient not found.\n");
  pauseScreen();
}

static void deletePatient(void) {
  int id = getIntInput("Enter Patient ID to delete: ");
  for (int i = 0; i < patientCount; i++) {
    if (patients[i].id == id) {
      if (patients[i].isAdmitted) {
        decrementDepartmentPatientCount(patients[i].department);
      }
      // Shift array to delete
      for (int j = i; j < patientCount - 1; j++) {
        patients[j] = patients[j + 1];
      }
      patientCount--;
      savePatientsData();
      printf("Patient deleted.\n");
      pauseScreen();
      return;
    }
  }
  printf("Patient not found.\n");
  pauseScreen();
}

static void printPatient(Patient *p) {
  printf("ID: %d | %s | Dept: %s | Doc: %s | Adm: %d/%d/%d | Status: %s\n",
         p->id, p->name, p->department, p->doctorName, p->admissionDate.month,
         p->admissionDate.day, p->admissionDate.year,
         p->isAdmitted ? "Admitted" : "Discharged");
}

static void listPatientsHelper(int admittedFilter, int dischargedFilter) {
  for (int i = 0; i < patientCount; i++) {
    if (admittedFilter && !patients[i].isAdmitted)
      continue;
    if (dischargedFilter && patients[i].isAdmitted)
      continue;
    printPatient(&patients[i]);
  }
  pauseScreen();
}

static void searchPatients(void) {
  clearScreen();
  printf("=== Search ===\n1. By ID\n2. By Name\n3. By Department\nSelect: ");
  int choice = getIntInput("");
  if (choice == 1) {
    int id = getIntInput("ID: ");
    for (int i = 0; i < patientCount; i++) {
      if (patients[i].id == id) {
        printPatient(&patients[i]);
        pauseScreen();
        return;
      }
    }
  } else if (choice == 2) {
    char name[50];
    printf("Name: ");
    getLine(name, sizeof(name));
    for (int i = 0; i < patientCount; i++) {
      if (strstr(patients[i].name, name))
        printPatient(&patients[i]);
    }
    pauseScreen();
  } else if (choice == 3) {
    char dept[50];
    printf("Dept: ");
    getLine(dept, sizeof(dept));
    for (int i = 0; i < patientCount; i++) {
      if (strcmp(patients[i].department, dept) == 0)
        printPatient(&patients[i]);
    }
    pauseScreen();
  }
}

static int cmpName(const void *a, const void *b) {
  return strcmp(((Patient *)a)->name, ((Patient *)b)->name);
}
static int cmpDept(const void *a, const void *b) {
  return strcmp(((Patient *)a)->department, ((Patient *)b)->department);
}
static int cmpDate(const void *a, const void *b) {
  Patient *p1 = (Patient *)a, *p2 = (Patient *)b;
  if (p1->admissionDate.year != p2->admissionDate.year)
    return p1->admissionDate.year - p2->admissionDate.year;
  if (p1->admissionDate.month != p2->admissionDate.month)
    return p1->admissionDate.month - p2->admissionDate.month;
  return p1->admissionDate.day - p2->admissionDate.day;
}

static void sortPatientsMenu(void) {
  clearScreen();
  printf("Sort by: 1. Name  2. Department  3. Admission Date\nSelect: ");
  int choice = getIntInput("");
  if (choice == 1)
    qsort(patients, patientCount, sizeof(Patient), cmpName);
  else if (choice == 2)
    qsort(patients, patientCount, sizeof(Patient), cmpDept);
  else if (choice == 3)
    qsort(patients, patientCount, sizeof(Patient), cmpDate);

  savePatientsData();
  printf("Sorted successfully.\n");
  pauseScreen();
}

// Exports
void listPatientsByDoctorExtern(const char *docName) {
  int found = 0;
  for (int i = 0; i < patientCount; i++) {
    if (strcmp(patients[i].doctorName, docName) == 0) {
      printPatient(&patients[i]);
      found++;
    }
  }
  if (!found)
    printf("No patients found for this doctor.\n");
}

int getDoctorPatientCountExtern(const char *docName) {
  int c = 0;
  for (int i = 0; i < patientCount; i++) {
    if (strcmp(patients[i].doctorName, docName) == 0)
      c++;
  }
  return c;
}

int getAdmittedCount(void) {
  int c = 0;
  for (int i = 0; i < patientCount; i++)
    if (patients[i].isAdmitted)
      c++;
  return c;
}

int getDischargedCount(void) {
  int c = 0;
  for (int i = 0; i < patientCount; i++)
    if (!patients[i].isAdmitted)
      c++;
  return c;
}

double calculateAverageStay(void) {
  int totalDays = 0, count = 0;
  for (int i = 0; i < patientCount; i++) {
    if (!patients[i].isAdmitted) {
      int d = calculateDaysBetween(
          patients[i].admissionDate.day, patients[i].admissionDate.month,
          patients[i].admissionDate.year, patients[i].dischargeDate.day,
          patients[i].dischargeDate.month, patients[i].dischargeDate.year);
      if (d == 0)
        d = 1;
      totalDays += d;
      count++;
    }
  }
  return count > 0 ? (double)totalDays / count : 0.0;
}

Patient *getPatientById(int id) {
  for (int i = 0; i < patientCount; i++) {
    if (patients[i].id == id)
      return &patients[i];
  }
  return NULL;
}

void patientMenu(void) {
  int choice = 0;
  while (choice != 11) {
    clearScreen();
    printf("%s=== Patient Management ===%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s1.%s Register Patient\n%s2.%s Update Record\n%s3.%s Discharge "
           "Patient\n%s4.%s "
           "Re-admit Patient\n",
           COLOR_YELLOW, COLOR_RESET, COLOR_YELLOW, COLOR_RESET, COLOR_YELLOW,
           COLOR_RESET, COLOR_YELLOW, COLOR_RESET);
    printf("%s5.%s Delete Patient\n%s6.%s Search\n%s7.%s List All\n%s8.%s List "
           "Admitted\n%s9.%s "
           "List Discharged\n",
           COLOR_YELLOW, COLOR_RESET, COLOR_YELLOW, COLOR_RESET, COLOR_YELLOW,
           COLOR_RESET, COLOR_YELLOW, COLOR_RESET, COLOR_YELLOW, COLOR_RESET);
    printf("%s10.%s Sort Patients\n%s11.%s Back to Main Menu\n", COLOR_YELLOW,
           COLOR_RESET, COLOR_YELLOW, COLOR_RESET);
    printf("%sSelect:%s ", COLOR_GREEN, COLOR_RESET);
    choice = getIntInput("");

    if (choice == 1)
      registerPatient();
    else if (choice == 2)
      updatePatient();
    else if (choice == 3)
      dischargePatient();
    else if (choice == 4)
      readmitPatient();
    else if (choice == 5)
      deletePatient();
    else if (choice == 6)
      searchPatients();
    else if (choice == 7)
      listPatientsHelper(0, 0);
    else if (choice == 8)
      listPatientsHelper(1, 0);
    else if (choice == 9)
      listPatientsHelper(0, 1);
    else if (choice == 10)
      sortPatientsMenu();
  }
}
