#ifndef BILLING_H
#define BILLING_H

#include "hims_types.h"

void initBilling(void);
void cleanupBilling(void);
void saveBillingData(void);

void generateBill(int patientId, int days);
void markBillPaid(void);
void viewBillHistory(void);
void billingMenu(void);

double getTotalRevenue(void);

#endif // BILLING_H
