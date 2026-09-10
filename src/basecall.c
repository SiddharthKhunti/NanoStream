#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "basecall.h"

// Simple lookup model mapping pA ranges to 3-mers
static const CurrentMap MODEL[] = {
    {40.0f, 45.0f, "AAA"}, // Low current ~42 pA
    {50.0f, 60.0f, "CGT"}, // Mid current ~54 pA
    {75.0f, 85.0f, "TAC"}, // High current ~78 pA
};
static const size_t MODEL_SIZE = sizeof(MODEL) / sizeof(MODEL[0]);

static const char* map_current_to_kmer(float current) {
    for (size_t i = 0; i < MODEL_SIZE; i++) {
        if (current >= MODEL[i].min_pa && current <= MODEL[i].max_pa) {
            return MODEL[i].kmer;
        }
    }
    return "NNN"; // Unknown sequence
}

void process_and_basecall(const SignalData *sig, const char *output_filename) {
    if (!sig || sig->count == 0) {
        fprintf(stderr, "No signal data available for basecalling.\n");
        return;
    }

    FILE *out = fopen(output_filename, "w");
    if (!out) {
        perror("Error creating FASTA output file");
        return;
    }

    // Write FASTA header
    fprintf(out, ">NanoStream_read_001 length=%zu\n", sig->count);

    printf("\n--- Basecalling Progress ---\n");

    // Group consecutive raw readings into event plateaus (threshold: 2.0 pA)
    float current_sum = sig->readings[0];
    size_t event_len = 1;

    for (size_t i = 1; i <= sig->count; i++) {
        if (i < sig->count && fabsf(sig->readings[i] - (current_sum / event_len)) < 2.0f) {
            current_sum += sig->readings[i];
            event_len++;
        } else {
            float mean_pa = current_sum / event_len;
            const char *kmer = map_current_to_kmer(mean_pa);

            printf(" Event: mean = %5.2f pA (len = %zu) -> Base: %s\n", mean_pa, event_len, kmer);
            fprintf(out, "%s", kmer);

            if (i < sig->count) {
                current_sum = sig->readings[i];
                event_len = 1;
            }
        }
    }

    fprintf(out, "\n");
    fclose(out);
    printf("Basecalling complete. Output saved to %s\n", output_filename);
}
