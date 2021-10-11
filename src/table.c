#include <stdio.h>
#include <stdlib.h>

#include "hash.h"
#include "header.h"
#include "body.h"

#include "table.h"

static int calculateDataSize(int, int); // Calculate heap size for ctData
static int calculateCapacity(int); // Calculate heap size for actual capacity.

CaseTable createCaseTable(int key_size,
                          int value_size,
                          int item_counts)
{
  CaseTable table;

  int max_item_cap    = calculateCapacity(item_counts);
  int data_block_size = calculateDataSize(key_size, value_size);

  // Set props.
  table.capacity = max_item_cap;
  table.key_size = key_size;
  table.val_size = value_size;
  table.data_size = data_block_size;
  table.hash_mod = table.capacity * 8;

  // Alloc header.
  int header_size = initHeader(&(table.header), table.capacity);
  if (header_size == -1) /*TODO:error*/;

  // Alloc body.
  table.body = createBody(table.data_size, table.capacity);

  // Set allocated heap size and return.
  table.total_allocated_heap = header_size + 
                               (table.body.data_size * table.body.data_count);
  return table; 
}

// Helpers.
void ctFree(CaseTable *t)
{ // Free all alloced heap.
  free(t->body.bits);
  free(t->body.ctData);
  free(t->header);
}


int calculateDataSize(int ksize, int vsize) {
  // Make a multiple of 4 bytes where larger than (ksize + vsize + 2).
  int sum = ksize + vsize;
  return ((sum + 2 - 1)/4 + 1) * 4;
}
int calculateCapacity(int required) {
  return ((required - 1)/sizeof(int) + 1)*sizeof(int);
}