#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "cachelab.h"

/*
 * name: Vrushank Agrawal
 * userid: vrushank.agrawal
 */

/* Globals set by command line args */
int verbosity = 0; /* print trace if set */
int s = 0;         /* set index bits */
int b = 0;         /* block offset bits */
int E = 0;         /* associativity */
char *trace_file = NULL;

/*
 * printUsage - Print usage info
 */
void printUsage(char *argv[])
{
  printf("Usage: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n", argv[0]);
  printf("Options:\n");
  printf("  -h         Print this help message.\n");
  printf("  -v         Optional verbose flag.\n");
  printf("  -s <num>   Number of set index bits.\n");
  printf("  -E <num>   Number of lines per set.\n");
  printf("  -b <num>   Number of block offset bits.\n");
  printf("  -t <file>  Trace file.\n");
  printf("\nExamples:\n");
  printf("  linux>  %s -s 4 -E 1 -b 4 -t traces/yi.trace\n", argv[0]);
  printf("  linux>  %s -v -s 8 -E 2 -b 4 -t traces/yi.trace\n", argv[0]);
  exit(0);
}

// struct that contains info for every cache_line
typedef struct cache_line
{
  char valid;                  // set to 0x00 if not valid
  unsigned long int cache_tag; // tag address of the cache
  int order;                   // order in which it was used
} cache_line;

/*
 * main - Main routine
 */
int main(int argc, char *argv[])
{
  char c;

  while ((c = getopt(argc, argv, "s:E:b:t:vh")) != -1)
  {
    switch (c)
    {
    case 's':
      s = atoi(optarg);
      break;
    case 'E':
      E = atoi(optarg);
      break;
    case 'b':
      b = atoi(optarg);
      break;
    case 't':
      trace_file = optarg;
      break;
    case 'v':
      verbosity = 1;
      break;
    case 'h':
      printUsage(argv);
      exit(0);
    default:
      printUsage(argv);
      exit(1);
    }
  }

  /* Make sure that all required command line args were specified */
  if (s == 0 || E == 0 || b == 0 || trace_file == NULL)
  {
    printf("%s: Missing required command line argument\n", argv[0]);
    printUsage(argv);
    exit(1);
  }

  // open file
  FILE *tfile;
  tfile = fopen(trace_file, "r");
  if (tfile == NULL)
  {
    fprintf(stderr, "Error opening trace file: %s\t%d", strerror(errno), errno);
    exit(EXIT_FAILURE);
  }

  // set number of sets
  int i, nbsets = 1 << s;

  // assign cache array as a pointer to an array of
  // pointers pointing to every cache set in the array
  cache_line **cache_array = (cache_line **)malloc(nbsets * sizeof(cache_line *));
  if (cache_array == NULL)
  {
    fprintf(stderr, "Malloc error: %s\t%d", strerror(errno), errno);
    exit(EXIT_FAILURE);
  }

  // assign number of lines E for every cache set with mem=0
  for (i = 0; i < nbsets; i++)
  {
    cache_array[i] = (cache_line *)calloc(E, sizeof(cache_line));
    if (cache_array[i] == NULL)
    {
      fprintf(stderr, "Calloc error: %s\t%d", strerror(errno), errno);
      exit(EXIT_FAILURE);
    }
  }

  char operation;
  unsigned long int address;
  int size;
  // global number of hits, misses, and evicts
  int hits = 0, misses = 0, evicts = 0;
  // sets the order number when the cache line was last used
  int cache_order = 0;

  // start simulating the cache for every line
  while (fscanf(tfile, " %c %lx,%d", &operation, &address, &size) > 0)
  {

    // ignore all Instruction Load
    if (operation == 'I')
      continue;

    // get the set index and tag from the address
    unsigned long int set_index, tag;
    set_index = ((address >> b) & (nbsets - 1));
    tag = (address >> (b + s));
    char found = 0x00, inserted = 0x00, verbose[20];

    // now check every line of the set in cache_array
    for (i = 0; i < E; i++)
    {
      if (cache_array[set_index][i].valid)
      {
        if (cache_array[set_index][i].cache_tag == tag)
        {
          cache_array[set_index][i].order = cache_order++;
          found = 0x01;
          break;
        }
      }
      else
      { // if we found a non-valid, we can insert here
        cache_array[set_index][i].valid = 0x01;
        cache_array[set_index][i].cache_tag = tag;
        cache_array[set_index][i].order = cache_order++;
        inserted = 0x01;
        break;
      }
    } // end of for loop to check cache entry

    // if data is not found, and neither inserted
    // we search for the smallest order and insert there
    if (!found && !inserted)
    {
      int j = 0;
      for (i = 1; i < E; i++)
        if (cache_array[set_index][j].order > cache_array[set_index][i].order)
          j = i;
      cache_array[set_index][j].valid = 0x01;
      cache_array[set_index][j].cache_tag = tag;
      cache_array[set_index][j].order = cache_order++;
    }

    // based on the operation, we switch between
    // the corresponding output to be displayed
    if (operation == 'L')
    {
      if (found)
      {
        strncpy(verbose, "hit\n", 5);
        hits++;
      }
      else if (inserted)
      {
        strncpy(verbose, "miss\n", 6);
        misses++;
      }
      else
      {
        strncpy(verbose, "miss eviction\n", 15);
        misses++;
        evicts++;
      }
    }
    else if (operation == 'S')
    {
      if (found)
      {
        strncpy(verbose, "hit\n", 5);
        hits++;
      }
      else if (inserted)
      {
        strncpy(verbose, "miss\n", 6);
        misses++;
      }
      else
      {
        strncpy(verbose, "miss eviction\n", 15);
        misses++;
        evicts++;
      }
    }
    else
    {
      if (found)
      {
        strncpy(verbose, "hit hit\n", 9);
        hits++;
        hits++;
      }
      else if (inserted)
      {
        strncpy(verbose, "miss hit\n", 10);
        misses++;
        hits++;
      }
      else
      {
        strncpy(verbose, "miss eviction hit\n", 19);
        misses++;
        evicts++;
        hits++;
      }
    }

    if (verbosity)
      printf("%c %lx,%d %s\n", operation, address, size, verbose);
  }

  // deallocate all memory and close file
  fclose(tfile);
  for (i = 0; i < nbsets; i++)
    free(cache_array[i]);
  free(cache_array);

  printSummary(hits, misses, evicts);
  return 0;
}
