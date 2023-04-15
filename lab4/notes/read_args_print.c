#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>

void arguments(int *s, int *E, int *b, char **t, int argc, char *argv[]) {

    if (argc == 1) {
        printf("Usage: ./csim -s <set-idx-bits> -E <lines-per-set> -b <block-bits> -t <tracefile>\n");
        exit(1);
    }
    
    int opt;
    while ((opt = getopt(argc, argv, "s:E:b:t:")) != -1) {
        switch (opt) {
            case 's':
                *s = atoi(optarg);
                break;
            case 'E':
                *E = atoi(optarg);
                break;
            case 'b':
                *b = atoi(optarg);
                break;
            case 't':
                *t = optarg;
                break;
            default:
                printf("Usage: ./csim -s <set-idx-bits> -E <lines-per-set> -b <block-bits> -t <tracefile>\n");
                exit(1);
        }
    }
}


int main(int argc, char *argv[]) {
    int total_hits, total_misses, total_evictions;      // totals for printsummary()
    int set_index_bits;     // s : number of set bits (S = 2^s is the number of sets)
    int lines_per_set;      // E : associativity (number of lines per set)
    int num_block_bits;     // v : number of block bits (B = 2^b is the block size)
    char *filename;         // t : name of valgrind trace to replay
    FILE *tracefile;
    char *filecontents;
    long numbytes;


    // get input arguments
    arguments(&set_index_bits, &lines_per_set, &num_block_bits, &filename, argc, argv);
    
    tracefile = fopen(filename, "r");
    if (tracefile == NULL) {
        printf("cant open file\n");
        exit(1);
    }
    fseek(tracefile, 0L, SEEK_END);
    numbytes = ftell(tracefile);
    fseek(tracefile, 0L, SEEK_SET);

    filecontents = (char*)calloc(numbytes, sizeof(char));
    fread(filecontents, sizeof(char), numbytes, tracefile);

    fclose(tracefile);
    printf("%s", filecontents);
    free(filecontents);
    

    return 0;
}