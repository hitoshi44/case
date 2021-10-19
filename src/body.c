#include <stdlib.h>
#include <string.h>

#include "body.h"

static int  book(ctBody*);
static void lockbit(unsigned char*, int);
static void unlockbit(unsigned char*, int);
static void initBits(unsigned char*, int);

ctBody createBody(int size, int count)
  { // Assumed count is a multiple of 8.
    ctBody result;
    result.data_count = count;
    result.data_size  = size;
    result.ctData     = (char *)malloc(count * size);
    result.bits       = (unsigned char *)malloc(count/8);
    
    initBits(result.bits, count/8);
    
    return result;
  }
// ctBody methods.
char *getData(ctBody* b, int index)
  {
    return &( (b->ctData)[index * (b->data_size)] );
  }
int setData(ctBody* b, char key[], int ksize, char value[], int vsize)
  {
    int index = book(b);
    if (index > -1) {
      strncpy( &((b->ctData)[ index * b->data_size          ]),   key, ksize);
      strncpy( &((b->ctData)[ index * b->data_size + ksize+1]), value, vsize);
    }
    return index;
  }
void delData(ctBody* b, int index)
  {
    unlockbit(b->bits, index);
  }
void overWriteData(ctBody* b, int index,
                   char key[], int ksize,
                   char val[], int vsize)
  {
    strncpy( &((b->ctData)[ index * b->data_size          ]), key, ksize);
    strncpy( &((b->ctData)[ index * b->data_size + ksize+1]), val, vsize);
  }

// Bit Flags Functions.
  #define MASK_1  0x01
  #define MASK_2  0x02
  #define MASK_3  0x04
  #define MASK_4  0x08
  #define MASK_5  0x10
  #define MASK_6  0x20
  #define MASK_7  0x40
  #define MASK_8  0x80
static int book(ctBody *b)
  { // Search an index of available memory and do book, return the index.
    // If there is no available space, return -1.    
    int result = -1;
    int i;


    // First, do a loop over whole bits with bigger block size (sizeof(int)).
    int loop_limit   = (b->data_count - 1)/(sizeof(int)*8) + 1;
    int *bits_as_int = (int *)(b->bits); 

    for ( i=0; i<loop_limit; i++) {
      if( bits_as_int[i] != 0 ) { // Search for a block containing 1 bit in it.
      
    
        //Second, do a loop over the block with smaller, a byte size.
        i = i *  sizeof(int); // To use "index" on char[], change the i scale.
        loop_limit = i + (sizeof(int)/sizeof(char));
        
        for(; i<loop_limit ; i++) {
          if( (b->bits)[i] > 0 ){ // Search for a byte containing 1 bit in it.
            // Last, specify which bit is 1.
            if ( ((b->bits)[i] & MASK_1) != 0 ) {result=i*8;  break;}
            if ( ((b->bits)[i] & MASK_2) != 0 ) {result=i*8+1;break;}
            if ( ((b->bits)[i] & MASK_3) != 0 ) {result=i*8+2;break;}
            if ( ((b->bits)[i] & MASK_4) != 0 ) {result=i*8+3;break;}
            if ( ((b->bits)[i] & MASK_5) != 0 ) {result=i*8+4;break;}
            if ( ((b->bits)[i] & MASK_6) != 0 ) {result=i*8+5;break;}
            if ( ((b->bits)[i] & MASK_7) != 0 ) {result=i*8+6;break;}
            if ( ((b->bits)[i] & MASK_8) != 0 ) {result=i*8+7;break;}
        }}


      }
      // If result is updated, lock the bit and escape from whole loop.
      if(result > -1){lockbit(b->bits, result);break;}
    }

    // If no bit flag was searched, result is stil -1.
    return result;
  }
static void lockbit(unsigned char bits[], int target)
  { // Make target index "unavailable". (set bit flag to 0)
    // If -1 is passed, do nothing.
  if(target > -1){
    int chunk = (target / 8);
    switch(target % 8){
      case 0: bits[chunk] = (bits[chunk] - MASK_1);break;
      case 1: bits[chunk] = (bits[chunk] - MASK_2);break;
      case 2: bits[chunk] = (bits[chunk] - MASK_3);break;
      case 3: bits[chunk] = (bits[chunk] - MASK_4);break;
      case 4: bits[chunk] = (bits[chunk] - MASK_5);break;
      case 5: bits[chunk] = (bits[chunk] - MASK_6);break;
      case 6: bits[chunk] = (bits[chunk] - MASK_7);break;
      case 7: bits[chunk] = (bits[chunk] - MASK_8);break;
  }}}
static void unlockbit(unsigned char bits[], int target)
  { // Make target index "available" again. (set bit flag to 1)
    // If -1 is passed, do nothing.
  if(target > -1){
    int chunk = target / 8;
    switch(target % 8){
      case 0: bits[chunk] = (bits[chunk] | MASK_1);break;
      case 1: bits[chunk] = (bits[chunk] | MASK_2);break;
      case 2: bits[chunk] = (bits[chunk] | MASK_3);break;
      case 3: bits[chunk] = (bits[chunk] | MASK_4);break;
      case 4: bits[chunk] = (bits[chunk] | MASK_5);break;
      case 5: bits[chunk] = (bits[chunk] | MASK_6);break;
      case 6: bits[chunk] = (bits[chunk] | MASK_7);break;
      case 7: bits[chunk] = (bits[chunk] | MASK_8);break;
  }}}
static void initBits(unsigned char bits[], int limit)
  {
    int i;
    for(i=0; i<limit; i++) {
      bits[i] = 0xff;
    }
  }