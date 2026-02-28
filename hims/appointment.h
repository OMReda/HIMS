#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include "hims_types.h"

typedef struct {
  int id;
  char patientName[50];
  char doctorName[50];
  Date date;
  int isCompleted;
} Appointment;

void initAppointments(void);
void cleanupAppointments(void);
void saveAppointmentsData(void);
void appointmentMenu(void);

#endif // APPOINTMENT_H
