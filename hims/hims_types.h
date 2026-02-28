#ifndef HIMS_TYPES_H
#define HIMS_TYPES_H

typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    int id;
    char name[50];
    int age;
    char diagnosis[150];
    char doctorName[50];
    char department[50];
    Date admissionDate;
    Date dischargeDate;
    int isAdmitted;  // 1 = admitted, 0 = discharged
} Patient;

typedef struct {
    int id;
    char name[50];
    char department[50];
} Doctor;

typedef struct {
    int patientId;
    double dailyRate;
    double totalBill;
    int isPaid;      // 1 = paid, 0 = unpaid
} BillingRecord;

typedef struct {
    int id;
    char name[50];
    int patientCount;
    int doctorCount;
} Department;

#endif // HIMS_TYPES_H
