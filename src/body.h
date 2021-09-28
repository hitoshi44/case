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

// Initialize ctBody with byte length of K-V data, and max counts of data,
// where counts must be a multiple of (sizeof(int)*8).
//     initBody(bytes, counts)
ctBody initBody(  int, int);

// Get a pointer to the data on the heap.
//    getData(ctBody*,index);
char* getData(ctBody*,  int);

// Set Key, Value into the heap, return the index tha data got set.
int   setData(ctBody*,
              char*, int,     //   key[], length_of_key
              char*, int);    // value[], length_of_value

// Delete by index. Actually, it just let bitflag be 1.
//    delData(ctBody*,index);
void  delData(ctBody*,  int);
