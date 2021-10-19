/*
  struct ctHeader behaves like a index for the table.


  Why headers like this are needed?

  Header is needed to decrease hash-collision probability with 
  less size of heap.
  Because Case(this module) is using open-addressing, reserving larger space 
  than actually required is important.
  So, Case allocates extra heap with unit of <ctHeader = int> size, 
  instead of K-V data size.

  When you would like "get" by Key, Case does below:
  
      Hashed = hash(Key)
      Header = Header_Array[ Hashed ]
      kvData = kvData_Array[ Header.pointer ]

      IF (kvData.key == Key) 
        return kvData.data
      ELSE
        ITERATE (Header.index) to DO-SAME

  Thus, a Key chains like (Key -> Hash -> Header -> K-V-Data).
*/

typedef int ctHeader;

// empty < deleted < full...
#define CT_EMPTY -4       // "header==EMPTY" means that header is emtpy.
#define CT_DEL   -2       // "header==DEL"   means that header is deleted.
#define CT_IS_DEL_FULL -3 // "header>IS_DEL_FULL" means header is deleted or full.
#define CT_IS_FULL -1     // "header>IS_FULL"means that header is full.

int initHeader(ctHeader **, 
               int table_capacity);