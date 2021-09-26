#include "hash.h"

/* 
    Implementation Fowler-Noll-Vo(1a) hash under Public Domain.
    
     - To avoid IF evaluations, this function always do 64 times loop.
     - Thus, it does NOT match the test vectors.
     - Assumed an input is on char[64] initialized with '\0' filled.
*/
#define FNV_BLOCK(X) base_hash=base_hash^input[X];base_hash=base_hash*fnv_prime;


static unsigned long long fnv1a_like(char input[]) {

  unsigned long long base_hash = 0xcbf29ce484222325;
  unsigned long long fnv_prime = 0x100000001b3;
  
  // avoid IF evaluation for each loop.
  FNV_BLOCK(0); FNV_BLOCK(1); FNV_BLOCK(2); FNV_BLOCK(3);
  FNV_BLOCK(4); FNV_BLOCK(5); FNV_BLOCK(6); FNV_BLOCK(7);
  FNV_BLOCK(8); FNV_BLOCK(9); FNV_BLOCK(10);FNV_BLOCK(11);
  FNV_BLOCK(12);FNV_BLOCK(13);FNV_BLOCK(14);FNV_BLOCK(15);
  FNV_BLOCK(16);FNV_BLOCK(17);FNV_BLOCK(18);FNV_BLOCK(19);
  FNV_BLOCK(20);FNV_BLOCK(21);FNV_BLOCK(22);FNV_BLOCK(23);
  FNV_BLOCK(24);FNV_BLOCK(25);FNV_BLOCK(26);FNV_BLOCK(27);
  FNV_BLOCK(28);FNV_BLOCK(29);FNV_BLOCK(30);FNV_BLOCK(31);
  FNV_BLOCK(32);FNV_BLOCK(33);FNV_BLOCK(34);FNV_BLOCK(35);
  FNV_BLOCK(36);FNV_BLOCK(37);FNV_BLOCK(38);FNV_BLOCK(39);
  FNV_BLOCK(40);FNV_BLOCK(41);FNV_BLOCK(42);FNV_BLOCK(43);
  FNV_BLOCK(44);FNV_BLOCK(45);FNV_BLOCK(46);FNV_BLOCK(47);
  FNV_BLOCK(48);FNV_BLOCK(49);FNV_BLOCK(50);FNV_BLOCK(51);
  FNV_BLOCK(52);FNV_BLOCK(53);FNV_BLOCK(54);FNV_BLOCK(55);
  FNV_BLOCK(56);FNV_BLOCK(57);FNV_BLOCK(58);FNV_BLOCK(59);
  FNV_BLOCK(60);FNV_BLOCK(61);FNV_BLOCK(62);FNV_BLOCK(63);
  
  return base_hash;
}

unsigned int hash(char input[], int mod) {
  return fnv1a_like(input) % mod;
}