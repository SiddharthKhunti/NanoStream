#ifndef SIGNAL_H
#define SIGNAL_H

#include <stddef.h>

// Holds array of raw electrical signals loaded from file
typedef struct {
    float *readings;
    size_t count;
    size_t capacity;
} SignalData;

// Function prototypes
SignalData* load_signals(const char *filename);
void free_signals(SignalData *sig);

#endif // SIGNAL_H
