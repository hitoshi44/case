#include "table.h"

static int calculateDataSize(int, int); // Calculate heap size for ctData
static int calculateCapacity(int); // Calculate heap size for actual capacity.
static void clean_DEL_flags_into_EMPTY(CaseTable *t, unsigned int start_pos);

CaseTable createCaseTable(int key_size,
                          int value_size,
                          int item_counts)
{
  CaseTable table;

  int max_item_cap    = calculateCapacity(item_counts);
  int data_block_size = calculateDataSize(key_size, value_size);

  // Set props.
  table.count     = 0;
  table.capacity  = max_item_cap;
  table.hash_mod  = table.capacity * 8;
  table.key_size  = key_size;
  table.val_size  = value_size;
  table.data_size = data_block_size;

  // Alloc header.
  int header_size = initHeader(&(table.header), table.capacity);
  if (header_size == -1) /*TODO:error*/;

  // Alloc body.
  table.body = createBody(table.data_size, table.capacity);

  // Set allocated heap size and return.
  table.total_allocated_heap = header_size + 
                               (table.body.data_size * table.body.data_count) +
                               (table.capacity)/8 + 1 /*Body bit flags size*/;
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

// Fetch pointers for key and value, if NOT exist return -1.
int ctFet(CaseTable *t, char key[])
{
  unsigned int hashed = hash(key, t->hash_mod);
  int header = (t->header)[hashed];

  while( header > CT_IS_DEL_FULL ) {
    if ( header > CT_IS_FULL ) {
      t->key = getData(&(t->body), header);
      if (strncmp(key, t->key, t->key_size) == 0){
        (t->value) = (t->key + t->key_size+1);
        return 0;
      }
    }
    hashed = (hashed + 1) % t->hash_mod;
    header = (t->header)[hashed];
  }

  return -1;  
}

// If key exists, return the header > 0(index for Table.body), else, return -1.
int ctHas(CaseTable *t, char key[])
{
  unsigned int hashed = hash(key, t->hash_mod);
  char *target;
  int header = (t->header)[hashed];

  while( header > CT_IS_DEL_FULL ) {
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

// Return 0 when deleted, -1 when key not found.
int ctDel(CaseTable *t, char key[])
{ // Almost same for ctHas, except for deepest if-stmt.
  unsigned int hashed = hash(key, t->hash_mod);
  char *target;
  int header = (t->header)[hashed];

  while( header > CT_IS_DEL_FULL ) {
    if (header > CT_IS_FULL) {
      target = getData(&(t->body), header);
      if (strncmp(key, target, t->key_size) == 0){
        // Deletes
        (t->count)--; (t->header)[hashed] = CT_DEL;
        delData(&(t->body), header);
        // Clean flags if possible.
        clean_DEL_flags_into_EMPTY(t, hashed);
        return 0;
      }
    }
    hashed = (hashed + 1) % t->hash_mod;
    header = (t->header)[hashed];
  }

  return -1;
}

// Fetch and delete it at the same time.
int ctPop(CaseTable *t, char key[])
{ 
  unsigned int hashed = hash(key, t->hash_mod);
  int header = (t->header)[hashed];

  while( header > CT_IS_DEL_FULL ) {
    if ( header > CT_IS_FULL ) {
      t->key = getData(&(t->body), header);
      if (strncmp(key, t->key, t->key_size) == 0){
        // Fetch
        (t->value) = (t->key + t->key_size+1);
        // Delete
        (t->count)--; (t->header)[hashed] = CT_DEL;
        delData(&(t->body), header);
        clean_DEL_flags_into_EMPTY(t, hashed);

        return 0;
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
  // Allocate one more byte for each to make null terminated.
  int sum = ksize + 1 + vsize + 1;
  return ((sum - 1)/4 + 1) * 4;
}
int calculateCapacity(int required) {
  return ((required - 1)/sizeof(int) + 1)*sizeof(int);
}
void clean_DEL_flags_into_EMPTY(CaseTable *t, unsigned int start_pos) {
  int current_pos = start_pos + 1;
  // while current header is DEL or EMPTY.
  while ((t->header)[current_pos] < 0) {

    // If there is EMPTY after continous DELs,
    // then that DELs are all EMPTY.
    if ((t->header)[current_pos] == CT_EMPTY){
      while (start_pos < current_pos) {
        (t->header)[start_pos] = CT_EMPTY;
        start_pos++;

      }
      return;
    }

    current_pos = (current_pos + 1) % t->hash_mod;
  }

  // When FULL header found, do nothing.
}