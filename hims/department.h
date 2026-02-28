#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include "hims_types.h"

void initDepartments(void);
void cleanupDepartments(void);
void saveDepartmentsData(void);

void addDepartment(void);
void listDepartments(void);
int checkDepartmentExists(const char *name);
void incrementDepartmentPatientCount(const char *name);
void decrementDepartmentPatientCount(const char *name);
void incrementDepartmentDoctorCount(const char *name);
void decrementDepartmentDoctorCount(const char *name);
void printDepartmentStats(void);

void departmentMenu(void);

#endif // DEPARTMENT_H
