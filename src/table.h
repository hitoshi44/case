#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hash.h"
#include "header.h"
#include "body.h"

typedef struct CaseTable CaseTable;
struct CaseTable {   
  ctHeader *header;
  ctBody    body;

  char *key;
  char *value;
  int   count;

  int hash_mod; // capacity * 8;
  int key_size;
  int val_size;
  int data_size;
  
  int capacity;
  int total_allocated_heap;

};
CaseTable createCaseTable(int /*key size (bytes)*/,
                          int /*value size (bytes)*/,
                          int /*items counts*/);

int ctPut(CaseTable *t, char key[], char value[]);
int ctFet(CaseTable *t, char key[]);
int ctHas(CaseTable *t, char key[]);
int ctDel(CaseTable *t, char key[]);
int ctPop(CaseTable *t, char key[]);

// Helper
void  ctFree(CaseTable *t);