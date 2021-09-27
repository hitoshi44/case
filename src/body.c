#include <stdlib.h>
#include <string.h>

#include "body.h"

static int  book(ctBody*);
static void lockbit(unsigned char*, int);
static void unlockbit(unsigned char*, int);
static void initBits(unsigned char*, int);

ctBody initBody(int size, int count)
  { // Assumed count is a multiple of sizeof(int)*8.
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
      strncpy( &((b->ctData)[ index * b->data_size        ]),   key, ksize);
      strncpy( &((b->ctData)[ index * b->data_size + ksize]), value, vsize);
    }
    return index;
  }
void delData(ctBody* b, int index)
  {
    unlockbit(b->bits, index);
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
static int  book(ctBody *b)
  { // Search available index and book, return the index.
    // If there is no space, return -1
    int result = -1;
    int limit = (b->data_count - 1)/(sizeof(int)*8) + 1;
    int *bigs = (int *)(b->bits); // Iterate with bigger size over bits.
    int i;
    for (i=0; i<limit; i++) {
    if( bigs[i] != 0 ) {
      
      limit = i*sizeof(int) + 8;
      for(i=(limit-8); i<limit ; i++) {
      if( (b->bits)[i] > 0 ){
        if ( ((b->bits)[i] & MASK_1) != 0 ) {result=i*8;  break;}
        if ( ((b->bits)[i] & MASK_2) != 0 ) {result=i*8+1;break;}
        if ( ((b->bits)[i] & MASK_3) != 0 ) {result=i*8+2;break;}
        if ( ((b->bits)[i] & MASK_4) != 0 ) {result=i*8+3;break;}
        if ( ((b->bits)[i] & MASK_5) != 0 ) {result=i*8+4;break;}
        if ( ((b->bits)[i] & MASK_6) != 0 ) {result=i*8+5;break;}
        if ( ((b->bits)[i] & MASK_7) != 0 ) {result=i*8+6;break;}
        if ( ((b->bits)[i] & MASK_8) != 0 ) {result=i*8+7;break;}
      }}

    }if(result > -1)break;}

    lockbit(b->bits, result);
    return result;
  }
static void lockbit(unsigned char bits[], int target)
  { // Make target index "unavailable". (set bit flag to 0)
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