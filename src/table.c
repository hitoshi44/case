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
  table.count    = 0;
  table.capacity = max_item_cap;
  table.hash_mod = table.capacity * 8;
  table.key_size = key_size;
  table.val_size = value_size;
  table.data_size = data_block_size;

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

int ctPut(CaseTable *t, char key[], char value[])
{ 
  // If table Has key, over write it.
  int idx = ctHas(t, key);
  if (idx > -1) {
    overWriteData(&(t->body), idx, key, t->key_size, value, t->val_size);
    return 0;
  }

  // Else, normally add with the open-addressing way.
  if (t->count == t->capacity) return -1;
  (t->count)++;

  unsigned int hashed = hash(key, t->hash_mod);
  while( (t->header)[hashed] > CT_IS_FULL ) {
    hashed = (hashed+1) % t->hash_mod;
  }
  (t->header)[hashed] = setData(&(t->body), key, t->key_size, value, t->val_size);

  return 0;
}

// If key exists, return the index, else, return -1.
int ctHas(CaseTable *t, char key[])
{
  unsigned int hashed = hash(key, t->hash_mod);
  char *target;
  int header = (t->header)[hashed];

  while( (t->header)[hashed] > CT_IS_DEL_FULL ) {
    if (header > CT_IS_FULL) {
      target = getData(&(t->body), header);
      if (strncmp(key, target, t->key_size) == 0){
        return header;
      }
    }
    hashed = (hashed + 1) % t->hash_mod;
    header = (t->header)[hashed];
  }

  return -1;
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


int main(void)
{
  CaseTable table, *t;
  table = createCaseTable(8,16,32);
  t = &table;

  printf("Just after inited. count:%d\n", t->count);

  printf("%d\n",ctPut(t, "key","value"));
  printf("%d\n",ctPut(t, "key2","value2"));

  printf("After put 2 items. count:%d\n", t->count);

  printf("%d\n",ctPut(t, "key","value new."));  

  printf("Over put imtes.    count:%d\n", t->count);

  ctFree(t);
}