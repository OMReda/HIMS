#ifndef FILE_H
#define FILE_H

#include <stddef.h>

void backupFile(const char *filename);
int saveGenericData(const char *filename, const void *array, size_t elemSize,
                    int count);
void *loadGenericData(const char *filename, size_t elemSize, int *count,
                      int *capacity);

#endif // FILE_H
