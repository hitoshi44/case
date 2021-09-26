#include <stdlib.h>
#include "header.h"

int initHeader(ctHeader **pp, int size)
{
  // Allocate heap.
  int count  = size * 8; // 8 times larger than required to avoid collision.
  int volume = sizeof(ctHeader) * count;
  *pp = (ctHeader *)malloc(volume);

  if (*pp == NULL)
    return -1;

  // Initialization with -1 which means "Not Exists yet".
  int i;
  for(i=0; i<count; i++) {(*pp)[i] = -1;}

  return volume;
}