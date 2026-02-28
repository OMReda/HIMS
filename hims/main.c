#include "appointment.h"
#include "auth.h"
#include "billing.h"
#include "department.h"
#include "doctor.h"
#include "patient.h"
#include "pharmacy.h"
#include "report.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

void initSystem(void) {
  initDepartments();
  initDoctors();
  initPatients();
  initBilling();
  initPharmacy();
  initAppointments();
}

void cleanupSystem(void) {
  cleanupPatients();
  cleanupDoctors();
  cleanupDepartments();
  cleanupBilling();
  cleanupPharmacy();
  cleanupAppointments();
}

int main(void) {
  enableANSI();
  clearScreen();
  showLoadingBar("Initializing HIMS core modules", 1000);

  initSystem();

  if (!authenticateAdmin()) {
    cleanupSystem();
    return 1;
  }

  int choice = 0;
  while (choice != 9) {
    clearScreen();
    printHeader();
    printf("%s1.%s Patient Management\n", COLOR_YELLOW, COLOR_RESET);
    printf("%s2.%s Doctor Management\n", COLOR_YELLOW, COLOR_RESET);
    printf("%s3.%s Department Management\n", COLOR_YELLOW, COLOR_RESET);
    printf("%s4.%s Billing System\n", COLOR_YELLOW, COLOR_RESET);
    printf("%s5.%s Reports & Analytics\n", COLOR_YELLOW, COLOR_RESET);
    printf("%s6.%s Pharmacy & Inventory\n", COLOR_YELLOW, COLOR_RESET);
    printf("%s7.%s Appointments\n", COLOR_YELLOW, COLOR_RESET);
    printf("%s8.%s Backup Data\n", COLOR_YELLOW, COLOR_RESET);
    printf("%s9.%s Exit\n", COLOR_RED, COLOR_RESET);
    printf("\n");
    printf("%sSelect an option:%s ", COLOR_GREEN, COLOR_RESET);
    choice = getIntInput("");

    switch (choice) {
    case 1:
      patientMenu();
      break;
    case 2:
      doctorMenu();
      break;
    case 3:
      departmentMenu();
      break;
    case 4:
      billingMenu();
      break;
    case 5:
      reportMenu();
      break;
    case 6:
      pharmacyMenu();
      break;
    case 7:
      appointmentMenu();
      break;
    case 8:
      saveDepartmentsData();
      saveDoctorsData();
      savePatientsData();
      saveBillingData();
      savePharmacyData();
      saveAppointmentsData();
      printf("%sManual backup point created in data/backup folder.%s\n",
             COLOR_GREEN, COLOR_RESET);
      pauseScreen();
      break;
    case 9:
      printf("%sExiting system. Saving data safely...%s\n", COLOR_YELLOW,
             COLOR_RESET);
      break;
    default:
      printf("%sInvalid choice.%s\n", COLOR_RED, COLOR_RESET);
      pauseScreen();
    }
  }

  cleanupSystem();
  // Confirm graceful shutdown
  printf("%sSystem shutdown gracefully.%s\n", COLOR_GREEN, COLOR_RESET);
  return 0;
}
