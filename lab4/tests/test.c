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

    // arguments s, E, b, t
    int set_index_bits;     // s : number of set bits (S = 2^s is the number of sets)
    int lines_per_set;      // E : associativity (number of lines per set)
    int num_block_bits;     // v : number of block bits (B = 2^b is the block size)
    char *filename;         // t : name of valgrind trace to replay


    // get input arguments
    arguments(&set_index_bits, &lines_per_set, &num_block_bits, &filename, argc, argv);
    //printf("args: -s %d -E %d -b %d -t %s\n", set_index_bits, lines_per_set, num_block_bits, tracefile);





    // open tracefile
    FILE *tracefile  = fopen(filename, "r");  
    char str_line[50];          // temp str for line in file
    int nlines = 0;             // number of lines in file
    char *op, *addr, *sz;       // operation letter, address, size
    unsigned long int addr_lu;

    if (!tracefile) {                // if file cannot be opened
        printf("%s: No such file or directory\n", filename);
        exit(1);
    }

    // get number of lines in file
    while (fgets(str_line, sizeof(str_line), tracefile)) { nlines++; }      

    // allocate memory for lines array
    cache_line **lines = malloc(sizeof(cache_line)*(nlines-1));


    // parse file contents
    int idx = 0;
    tracefile = fopen(filename, "r");
    while (fgets(str_line, sizeof(str_line), tracefile)) {
        if (str_line[0] == ' ') {
            op = strtok(str_line, " ");     // get operation
            addr = strtok(NULL, ",");       // get address
            sz = strtok(NULL, "\n");        // get size

            // add to struct cache_line
            if (op != NULL && addr != NULL && sz != NULL) {
                lines[idx] = malloc(sizeof(cache_line)); 
                lines[idx]->operation = *op;
                lines[idx]->address = (int)strtol(addr, NULL, 16);
                strcpy(lines[idx]->addr_str, addr);
                lines[idx]->op_size = atoi(sz);
                idx++;
            }
        }
    } nlines = idx;
    fclose(tracefile);        // close tracefile








    // verbose output
    if (verbose == true) {
        for (int i=0; i<nlines; i++) {
            printf(" %c %s,%d {hit/miss}\n", lines[i]->operation, lines[i]->addr_str, lines[i]->op_size);
        }
    }
    
    // de-allocate memory
    for (int i=0; i<nlines; i++) { free(lines[i]); }
    free(lines);

    //printSummary(total_hits, total_misses, total_evictions);      // total number of (hits, misses, evictions)
    return 0;
}
