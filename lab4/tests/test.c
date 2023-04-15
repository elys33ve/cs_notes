#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>


bool verbose = false;


// struct for each tracefile line
typedef struct {
    char operation;             // operation letter
    unsigned long int address;  // 
    char addr_str[16];
    int op_size;
} cache_line;



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
                verbose = true;
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

    int set_idx_bits;           // s : number of set index bits
    int num_sets = 1;           // S : number of sets (2^s)
    int lines_per_set;          // E : number of lines per set (associativity)
    int block_bits;             // b : number of block bits (block size = 2^b)
    char *filename;             // t : name of valgrind trace to replay

    // ------- get input arguments -------
    arguments(&set_idx_bits, &lines_per_set, &block_bits, &filename, argc, argv);
    //printf("args: -s %d -E %d -b %d -t %s\n", set_index_bits, lines_per_set, num_block_bits, tracefile);
    
    for (int i=0; i<set_idx_bits; i++) { num_sets *= 2; }
    printf("%d^%d = %d\n", 2, set_idx_bits, num_sets);




    // open tracefile
    FILE *tracefile;
    char str_line[50];          // temp str for line in file
    int nlines = 0;             // number of lines in file
    char *op, *addr, *sz;       // operation letter, address, size
    unsigned long int addr_lu;

    
   




    //printSummary(total_hits, total_misses, total_evictions);      // total number of (hits, misses, evictions)
    return 0;
}
