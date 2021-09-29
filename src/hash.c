#include "hash.h"

// TODO: Change to more fast Hash Function? e.g) xxxHash, FarmHash

/* 
    Implementation Fowler-Noll-Vo(1a) hash under Public Domain.
*/
#define FNV_BLOCK(X) base_hash=base_hash^input[X];base_hash=base_hash*fnv_prime;


static unsigned long long fnv1a_like(char input[]) {

  unsigned long long result = 0xcbf29ce484222325;
  unsigned long long fnv_prime = 0x100000001b3;

  int i = 0;
  do {
    result = result^input[i];
    result = result*fnv_prime;
    i++;
  } while(input[i] != '\0'); 

   return result;
}

unsigned int hash(char input[], int mod) {
  return fnv1a_like(input) % mod;
}