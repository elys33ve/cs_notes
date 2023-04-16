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

bool debug = false;
bool verbose = false;


int E;      // lines per set
int total_hits, total_misses, total_evictions;      // totals for printsummary()


// struct for cache info
typedef struct {
    int valid;              // valid bit
    unsigned long int tag;  // tag
    int lru;                // least recently used
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
    printf("  linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n");
    printf("  linux>  ./csim -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
}
// print argument format/usage
void print_usage() {
    printf("Usage: ./csim [-hv] -s <num> -E <num> -b <num> -t <file>\n");
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


// check cache ( look for hit, miss, eviction and update)
void check_cache(int tag, int set, int idx, cache_line **cache) {
    /*  valid bit = 1 && tag matches        : hit
        tag doesn't match                   : miss
        tag doesn't match && all valid      : miss & eviction
    *//*   
        look for valid line in set to compare tag
        if matches (hit)
        if no match found, get first invalid and replace tag (miss)
        if no match found and all lines valid, get LRU line to evict (miss & eviction)
   */
    // look for valid line w matching tag in set given by address (check for hit)
    for (int i=0; i<E; i++) {
        
        if (cache[set][i].valid == 1 && cache[set][i].tag == tag) {
            cache[set][i].lru = idx;        // update age
            total_hits++;
            if (verbose) { printf(" hit"); }
            return;
        }
    } // (no matches)
    total_misses++;

    // get first invalid and replace
    int lru = 0;        // to compare lru
    for (int i=0; i<E; i++) {
        if (cache[set][i].valid == 0) {     // look for invalid
            // replace
            cache[set][i].valid = 1;
            cache[set][i].tag = tag;
            cache[set][i].lru = idx;
            if (verbose) { printf(" miss"); }
            return;
        } else {
            // if update for least recently used
            if (cache[set][lru].lru > cache[set][i].lru) { lru = i; }
        }
    } // (no lines invalid)
    total_evictions++;

    // evict and replace
    if (verbose) { printf(" miss eviction"); }
    cache[set][lru].valid = 1;
    cache[set][lru].tag = tag;
    cache[set][lru].lru = idx;
    return;    
}





int main(int argc, char *argv[]) {
    // argument variables  ( s, E, b, t )
    int set_idx_bits;           // s : number of set index bits
    int num_sets = 1;           // S : number of sets (2^s)
    int lines_per_set;          // E : number of lines per set (associativity)
    int block_bits;             // b : number of block bits (block size = 2^b)
    char *filename;             // t : name of valgrind trace to replay

    // ------- get input arguments -------
    arguments(&set_idx_bits, &lines_per_set, &block_bits, &filename, argc, argv);
    if (debug) {
        printf("args: -s %d -E %d -b %d -t %s\n", set_idx_bits, lines_per_set, block_bits, filename);
    }
    E = lines_per_set;
    for (int i=0; i<set_idx_bits; i++) { num_sets *= 2; }   // get number of sets (2^s)
    // (it was complaining abt pow() so this was easier)





    // cache variables
    char operation;
    long unsigned int addr;
    unsigned int set_idx, tag;
    int size;

    // ------- allocate memory for cache struct -------
    cache_line **cache_array = malloc(sizeof(cache_line) * num_sets); 
    // check for allocation error
    if (cache_array == NULL) {
        printf("memory allocation error\n");
        exit(1);
    }
    // allocate space for lines in each set
    for (int i=0; i<num_sets; i++) {
        cache_array[i] = malloc(sizeof(cache_line) * lines_per_set);    // allocate memory for cache line
        // check for error
        if (cache_array == NULL) {
            printf("memory allocation error\n");
            exit(1);
        }
    }




    // tracefile variables
    FILE *tracefile  = fopen(filename, "r");  
    char str_line[64];          // temp str for line in file
    int addr_len = 64;          // length of addr in bits
    
    // if file cannot be opened
    if (!tracefile) {                
        printf("%s: No such file or directory\n", filename);
        exit(1);
    }   

    

    // ------- parse file contents and check cache -------
    int idx = 0;
    int s = set_idx_bits;
    int b = block_bits;
    while (fgets(str_line, sizeof(str_line), tracefile) != NULL) {
        idx++;

        // ignore if first char in line is not space (ignore 'I')
        if (str_line[0] == ' ') {
            // get line contents
            sscanf(str_line, " %c %lx,%d", &operation, &addr, &size);           
            tag = (addr >> s) >> b;                                         // parse tag
            set_idx = ((addr << (addr_len - (s + b))) >> (addr_len - s));   // parse set idx

            if (verbose) { printf("%c %lx,%d", operation, addr, size); }    // print for verbose

            // L : load
            if (operation == 'L') {
                check_cache(tag, set_idx, idx, cache_array);    // load
            } 
            // S : store
            else if (operation == 'S') {
                check_cache(tag, set_idx, idx, cache_array);    // store
            } 
            // M : modify (load + store)
            else if (operation == 'M') {
                check_cache(tag, set_idx, idx, cache_array);    // load (modify)
                check_cache(tag, set_idx, idx, cache_array);    // store (modify)
            }
            if (verbose) { printf("\n"); }  // print for verbose

            
            if (debug) {
                printf("line: %c %lx,%d\n", operation, addr, size);
                printf("\ttag: %x, set idx: %x\n", tag, set_idx);
            }
        }
    }
    fclose(tracefile);        // close tracefile

    
    


    // ------- de-allocate memory -------
    for (int i=0; i<num_sets; i++) { free(cache_array[i]); }
    free(cache_array);


    printSummary(total_hits, total_misses, total_evictions);      // total number of (hits, misses, evictions)
    return 0;
}
