/*  Intro to Computer Systems: Lab 4
 *  Author: Fiona O'Connell (oconnefa)
 */
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include "lab4.h"

/*  Cache Simulator lab

- takes valgrind memory trace as input
- simulates hit/miss behavior of cache memory on this trace
- outputs total number of hits, misses, and evictions (printsummary())

- must compile without warnings
- must work correctly for arbitrary s, E, and b. (allocate storage for structures w malloc)
S = 2^s sets
E = 2^e lines per set
B = 2^b bytes per cache block
cache size = S * E * B
- ignore instruction cache accesses
- ignore request sizes in valgrind traces
*/


bool VERBOSE = false;


// print all argument options
void print_help() {
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of set index bits.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of block offset bits.\n");
    printf("  -t <file>  Trace file.\n");
    printf("\nExamples:\n");
    printf("  linux>  ./csim-ref -s 4 -E 1 -b 4 -t traces/yi.trace\n");
    printf("  linux>  ./csim-ref -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
}
// print argument format/usage
void print_usage() {
    printf("Usage: ./csim [-hv] -s <set-idx-bits> -E <lines-per-set> -b <block-bits> -t <tracefile>\n");
}


// get and parse command line arguments
void arguments(int *s, int *E, int *b, char **t, int argc, char *argv[]) {
    // ./csim [-hv] -s <s> -E <E> -b <b> -t <tracefile>

    // no arguments provided
    if (argc == 1) {
        printf("./csim: Missing required command line argument\n");
        print_usage();
        print_help();
        exit(1);
    }
    
    // get arguments
    int opt;
    while ((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
        switch (opt) {
            case 'h':               // help
                print_usage();
                print_help();
                exit(1);
            case 'v':               // verbose mode
                VERBOSE = true;
                break;
            case 's':               // set index bits
                *s = atoi(optarg);
                break;
            case 'E':               // lines per set
                *E = atoi(optarg);
                break;
            case 'b':               // number of block bits
                *b = atoi(optarg);
                break;
            case 't':               // tracefile name
                *t = optarg;
                break;
            default:
                print_usage();
                exit(1);
        }
    }
}








int main(int argc, char *argv[]) {
    int total_hits, total_misses, total_evictions;      // totals for printsummary()

    // arguments s, E, b, t
    int set_index_bits;     // s : number of set bits (S = 2^s is the number of sets)
    int lines_per_set;      // E : associativity (number of lines per set)
    int num_block_bits;     // v : number of block bits (B = 2^b is the block size)
    char *filename;         // t : name of valgrind trace to replay


    // get input arguments
    arguments(&set_index_bits, &lines_per_set, &num_block_bits, &filename, argc, argv);
    //printf("args: -s %d -E %d -b %d -t %s\n", set_index_bits, lines_per_set, num_block_bits, tracefile);


    // tracefile
    FILE *tracefile  = fopen(filename, "r");  
    char *filecontents;
    char line[500];

    // open tracefile
    if (!tracefile) {                // if file cannot be opened
        printf("%s: No such file or directory\n", filename);
        exit(1);
    }

    // get file contents
    while (fgets(line, sizeof(line), tracefile)) {
        printf("%s", line);
    }
    


    fclose(tracefile);
    
    



    //printSummary(total_hits, total_misses, total_evictions);      // total number of (hits, misses, evictions)
    return 0;
}
