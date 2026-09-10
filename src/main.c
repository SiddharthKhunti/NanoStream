#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "signal.h"
#include "basecall.h"

void print_usage(const char *prog_name) {
    printf("Usage: %s -i <input_signals.txt> [-o <output.fasta>]\n", prog_name);
    printf("Options:\n");
    printf("  -i <file>    Path to raw signal text file (required)\n");
    printf("  -o <file>    Path to output FASTA file (default: output.fasta)\n");
    printf("  -h           Display this help menu\n");
}

int main(int argc, char *argv[]) {
    const char *input_file = NULL;
    const char *output_file = "output.fasta";
    int opt;

    // Parse command-line flags
    while ((opt = getopt(argc, argv, "i:o:h")) != -1) {
        switch (opt) {
            case 'i':
                input_file = optarg;
                break;
            case 'o':
                output_file = optarg;
                break;
            case 'h':
                print_usage(argv[0]);
                return 0;
            default:
                print_usage(argv[0]);
                return 1;
        }
    }

    // Fall back to positional argument if -i wasn't provided
    if (!input_file && optind < argc) {
        input_file = argv[optind];
    }

    // Ensure an input file was specified
    if (!input_file) {
        fprintf(stderr, "Error: Missing required input signal file.\n");
        print_usage(argv[0]);
        return 1;
    }

    printf("--- NanoStream Engine --- \n");
    printf("Loading signals from: %s\n", input_file);

    SignalData *sig = load_signals(input_file);
    if (!sig) {
        fprintf(stderr, "Failed to load signals.\n");
        return 1;
    }

    printf("Successfully loaded %zu signal readings:\n", sig->count);
    for (size_t i = 0; i < sig->count; i++) {
        printf(" Signal [%zu]: %.2f pA\n", i, sig->readings[i]);
    }

    // Call basecaller
    process_and_basecall(sig, output_file);

    free_signals(sig);
    printf("Memory freed successfully.\n");

    return 0;
}
