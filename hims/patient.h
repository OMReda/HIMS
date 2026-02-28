#ifndef PATIENT_H
#define PATIENT_H

#include "hims_types.h"

void initPatients(void);
void cleanupPatients(void);
void savePatientsData(void);

void patientMenu(void);

// Exports for other modules
void listPatientsByDoctorExtern(const char *docName);
int getDoctorPatientCountExtern(const char *docName);
int getAdmittedCount(void);
int getDischargedCount(void);
double calculateAverageStay(void);
Patient *getPatientById(int id);

#endif // PATIENT_H
