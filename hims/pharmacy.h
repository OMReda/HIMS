#ifndef PHARMACY_H
#define PHARMACY_H

typedef struct {
  int id;
  char name[100];
  int quantity;
  double price;
} Medicine;

void initPharmacy(void);
void cleanupPharmacy(void);
void savePharmacyData(void);
void pharmacyMenu(void);

#endif // PHARMACY_H
