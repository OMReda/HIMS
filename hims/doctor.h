#ifndef DOCTOR_H
#define DOCTOR_H

#include "hims_types.h"

void initDoctors(void);
void cleanupDoctors(void);
void saveDoctorsData(void);

void doctorMenu(void);
int checkDoctorExists(const char *name);
void printMostActiveDoctor(void);

#endif // DOCTOR_H
