/*
  struct tHeader pretends a index to actual K-V data.

  
  When you would like "get" by Key, CaseTable does below:
  
      Hashed = hash(Key)
      Header = Header_Array[ Hashed ]
      kvData = kvData_Array[ Header.pointer ]

      IF (kvData.key == Key) 
        return kvData.data
      ELSE
        ITERATE (Header.index) to DO-SAME

  It means, a Key chains to (Key -> Hash -> Header -> K-V-Data).


  Header is needed to decrease hash-collision probability with less memory.
  Because Case is using open-addressing, reserving larger space 
  than actually required is important.
  So, Case allocates extra memory space with unit of <ctHeader = int> size, 
  instead of K-V data size.
*/

typedef int ctHeader;

int initHeader(ctHeader **, int);
