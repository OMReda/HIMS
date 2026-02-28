#include "file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define BACKUP_DIR "data/backup/"

void backupFile(const char *filename) {
  FILE *src = fopen(filename, "rb");
  if (!src)
    return; // Nothing to backup

  const char *base = strrchr(filename, '/');
  if (!base)
    base = strrchr(filename, '\\');
  if (base)
    base++;
  else
    base = filename;

  char backupPath[256];
  snprintf(backupPath, sizeof(backupPath), "%s%s", BACKUP_DIR, base);

  FILE *dst = fopen(backupPath, "wb");
  if (!dst) {
    fclose(src);
    return;
  }

  char buffer[4096];
  size_t bytes;
  while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
    fwrite(buffer, 1, bytes, dst);
  }

  fclose(src);
  fclose(dst);
}

int saveGenericData(const char *filename, const void *array, size_t elemSize,
                    int count) {
  // Backup before overwriting
  backupFile(filename);

  FILE *file = fopen(filename, "wb");
  if (!file)
    return 0;

  size_t writtenCount = fwrite(&count, sizeof(int), 1, file);
  if (writtenCount != 1) {
    fclose(file);
    return 0;
  }

  if (count > 0) {
    size_t writtenData = fwrite(array, elemSize, count, file);
    if (writtenData != (size_t)count) {
      fclose(file);
      return 0;
    }
  }

  fclose(file);
  return 1;
}

void *loadGenericData(const char *filename, size_t elemSize, int *count,
                      int *capacity) {
  FILE *file = fopen(filename, "rb");
  if (!file) {
    *count = 0;
    *capacity = 10;
    return malloc(elemSize * (*capacity));
  }

  int readCount;
  if (fread(&readCount, sizeof(int), 1, file) != 1) {
    // Corrupted or empty
    fclose(file);
    *count = 0;
    *capacity = 10;
    return malloc(elemSize * (*capacity));
  }

  *count = readCount;
  *capacity = (readCount > 10) ? readCount * 2 : 10;

  void *array = malloc(elemSize * (*capacity));
  if (!array) {
    fclose(file);
    *count = 0;
    *capacity = 0;
    return NULL;
  }

  if (readCount > 0) {
    size_t readData = fread(array, elemSize, readCount, file);
    if (readData != (size_t)readCount) {
      // Corrupt file data length mismatch -> partial read handled by resetting
      // to 0
      free(array);
      fclose(file);
      *count = 0;
      *capacity = 10;
      return malloc(elemSize * (*capacity));
    }
  }

  fclose(file);
  return array;
}
