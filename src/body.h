
typedef struct ctBody ctBody;
struct ctBody {
  unsigned char *bits; // Flags for insertion.
  char  *ctData;        // actual K-V data array
  int   data_size;      // byte length of KV-data;
  int   data_count;     // count of data.
};

ctBody initBody(int, int);    // Initialize ctBody with count, size of KV-data;
char* getData(ctBody*, int);  // Get a pointer to the data by index.
int   setData(ctBody*,        // Set Key, Value into array.      
              char*, int,     //   char* key[],  int keysize
              char*, int);    //   char* value[],int valuesize
void  delData(ctBody*, int);  // Delete by index. Just free bitflags.
