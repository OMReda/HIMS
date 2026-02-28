#include "appointment.h"
#include "doctor.h"
#include "file.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static Appointment *appts = NULL;
static int apptCount = 0;
static int apptCapacity = 0;
static int nextApptId = 1;

static const char *FILENAME = "data/appointments.dat";

void initAppointments(void) {
  appts = (Appointment *)loadGenericData(FILENAME, sizeof(Appointment),
                                         &apptCount, &apptCapacity);
  for (int i = 0; i < apptCount; i++) {
    if (appts[i].id >= nextApptId) {
      nextApptId = appts[i].id + 1;
    }
  }
}

void cleanupAppointments(void) {
  saveAppointmentsData();
  if (appts)
    free(appts);
}

void saveAppointmentsData(void) {
  saveGenericData(FILENAME, appts, sizeof(Appointment), apptCount);
}

static void ensureApptCapacity(void) {
  if (apptCount >= apptCapacity) {
    apptCapacity = (apptCapacity == 0) ? 10 : apptCapacity * 2;
    Appointment *temp = realloc(appts, apptCapacity * sizeof(Appointment));
    if (temp)
      appts = temp;
  }
}

static void scheduleAppointment(void) {
  clearScreen();
  printf("%s=== Schedule Appointment ===%s\n", COLOR_CYAN, COLOR_RESET);

  char pname[50], dname[50];
  printf("Patient Name: ");
  getLine(pname, sizeof(pname));
  printf("Doctor Name: ");
  getLine(dname, sizeof(dname));

  if (!checkDoctorExists(dname)) {
    printf("%sDoctor not found!%s\n", COLOR_RED, COLOR_RESET);
    pauseScreen();
    return;
  }

  int d = getIntInput("Day (1-31): ");
  int m = getIntInput("Month (1-12): ");
  int y = getIntInput("Year (YYYY): ");

  ensureApptCapacity();
  Appointment *a = &appts[apptCount];
  a->id = nextApptId++;
  strncpy(a->patientName, pname, sizeof(a->patientName) - 1);
  a->patientName[sizeof(a->patientName) - 1] = '\0';
  strncpy(a->doctorName, dname, sizeof(a->doctorName) - 1);
  a->doctorName[sizeof(a->doctorName) - 1] = '\0';
  a->date.day = d;
  a->date.month = m;
  a->date.year = y;
  a->isCompleted = 0;

  apptCount++;
  saveAppointmentsData();

  printAnimated("\nAppointment Scheduled Successfully!\n", 15, COLOR_GREEN);
  pauseScreen();
}

static void viewAppointments(void) {
  clearScreen();
  printf("%s=== Upcoming Appointments ===%s\n", COLOR_CYAN, COLOR_RESET);
  printf("%-5s | %-20s | %-20s | %-12s | %-10s\n", "ID", "Patient", "Doctor",
         "Date", "Status");
  printf("---------------------------------------------------------------------"
         "-----------\n");
  for (int i = 0; i < apptCount; i++) {
    printf("%-5d | %-20s | %-20s | %02d/%02d/%04d | %-10s\n", appts[i].id,
           appts[i].patientName, appts[i].doctorName, appts[i].date.day,
           appts[i].date.month, appts[i].date.year,
           appts[i].isCompleted ? "Completed" : "Scheduled");
  }
  pauseScreen();
}

static void markCompleted(void) {
  clearScreen();
  printf("%s=== Mark Appointment Completed ===%s\n", COLOR_CYAN, COLOR_RESET);
  int id = getIntInput("Enter Appointment ID: ");

  for (int i = 0; i < apptCount; i++) {
    if (appts[i].id == id) {
      appts[i].isCompleted = 1;
      saveAppointmentsData();
      printf("%sAppointment marked as completed.%s\n", COLOR_GREEN,
             COLOR_RESET);
      pauseScreen();
      return;
    }
  }
  printf("%sAppointment not found.%s\n", COLOR_RED, COLOR_RESET);
  pauseScreen();
}

void appointmentMenu(void) {
  int choice = 0;
  while (choice != 4) {
    clearScreen();
    printf("%s=== Appointments System ===%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s1.%s Schedule Appointment\n", COLOR_YELLOW, COLOR_RESET);
    printf("%s2.%s View All Appointments\n", COLOR_YELLOW, COLOR_RESET);
    printf("%s3.%s Mark Appointment Completed\n", COLOR_YELLOW, COLOR_RESET);
    printf("%s4.%s Back to Main Menu\n", COLOR_YELLOW, COLOR_RESET);

    printf("%sSelect an option:%s ", COLOR_GREEN, COLOR_RESET);
    choice = getIntInput("");

    switch (choice) {
    case 1:
      scheduleAppointment();
      break;
    case 2:
      viewAppointments();
      break;
    case 3:
      markCompleted();
      break;
    case 4:
      break;
    default:
      printf("%sInvalid choice.%s\n", COLOR_RED, COLOR_RESET);
      pauseScreen();
    }
  }
}
