#ifndef BASECALL_H
#define BASECALL_H

#include "signal.h"

// Represents a group of similar raw readings (an "event" or plateau)
typedef struct {
    float mean_current;
    size_t duration;
} SignalEvent;

// Holds a simple model mapping current ranges to k-mers/bases
typedef struct {
    float min_pa;
    float max_pa;
    const char *kmer;
} CurrentMap;

// Function declarations
void process_and_basecall(const SignalData *sig, const char *output_filename);

#endif // BASECALL_H
