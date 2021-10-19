/*
  struct ctBody behaves like actual K-V data for the table.

  ctBody.ctData
    represents an array of K-V data. Actually, it is continuous heap.

  ctBody.data_size
    is a number of byte length of K-V data.
  
  ctBody.data_count
    is a number how many K-V data the table can holds.

  ctBody.bits
      is an array of bit-flags which helps efficiently searching for "available" place 
    to store K-V data on the heap.
    
*/
typedef struct ctBody ctBody;
struct ctBody {
  unsigned char *bits;  // Bit-Flags for insertion.
  char  *ctData;        // actual K-V data array
  int   data_size;      // byte length of KV-data;
  int   data_count;     // count of data.
};

// Methods
ctBody createBody(int bytes_of_KV_Data,
                  int capacity_of_data_count);

// Get a pointer to the data on the heap.
char* getData(ctBody*,
              int index);

// Set Key, Value on a heap
// Return the index of the data.
int setData(ctBody*,
            char* key,
            int   bytes_of_key,
            char* value,
            int   bytes_of_value);    

// Delete by index. Actually, it just let bitflag be 1.
void delData(ctBody*,
             int index);

void overWriteData(ctBody*,
                   int index,
                   char* key,
                   int   bytes_of_key,
                   char* value,
                   int   bytes_of_value);