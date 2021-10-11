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

int ctAdd(CaseTable *t, char key[], char value[]);
int ctFet(CaseTable *t, char key[]);
int ctHas(CaseTable *t, char key[]);
int ctDel(CaseTable *t, char key[]);
int ctPop(CaseTable *t, char key[]);

// Helper
char* ctGet(CaseTable *t, char key[]);
void  ctFree(CaseTable *t);