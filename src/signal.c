#include <stdio.h>
#include <stdlib.h>
#include "signal.h"

SignalData* load_signals(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening signal file");
        return NULL;
    }

    SignalData *sig = malloc(sizeof(SignalData));
    if (!sig) {
        fclose(file);
        return NULL;
    }

    sig->capacity = 16;
    sig->count = 0;
    sig->readings = malloc(sig->capacity * sizeof(float));

    if (!sig->readings) {
        free(sig);
        fclose(file);
        return NULL;
    }

    float value;
    while (fscanf(file, "%f", &value) == 1) {
        // Resize array if capacity is reached
        if (sig->count >= sig->capacity) {
            sig->capacity *= 2;
            float *temp = realloc(sig->readings, sig->capacity * sizeof(float));
            if (!temp) {
                free_signals(sig);
                fclose(file);
                return NULL;
            }
            sig->readings = temp;
        }
        sig->readings[sig->count++] = value;
    }

    fclose(file);
    return sig;
}

void free_signals(SignalData *sig) {
    if (sig) {
        if (sig->readings) {
            free(sig->readings);
        }
        free(sig);
    }
}
