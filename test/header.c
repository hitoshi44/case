#include "unity/unity.h"
#include "../src/header.h"

void setUp(void){}    //not need
void tearDown(void){} //not need

void test_InitHeader(void);

int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_InitHeader);
  return UNITY_END();
}


int Heap_Size_From(int item_counts)
{ /*   Will allocate < 8 > times larger than required
  ** to decrease the hash collision probs.
  **   Read comments at src/header.h .            */  
  return item_counts * (sizeof(ctHeader)*8);
}
void test_InitHeader(void)
{
  ctHeader *headers;    
  int expected_bytes, actual_bytes;


  // First, compare how much heap bytes allocated.
  // Then, confirm each Header is set to -1, meaning "Not Exist".
  expected_bytes = Heap_Size_From(16);
  actual_bytes   = initHeader(&headers, 16);
  TEST_ASSERT_EQUAL_INT(expected_bytes, actual_bytes);
  TEST_ASSERT_EQUAL_INT(CT_EMPTY, headers[0]); // First
  TEST_ASSERT_EQUAL_INT(CT_EMPTY, headers[7]);
  TEST_ASSERT_EQUAL_INT(CT_EMPTY, headers[8]);
  TEST_ASSERT_EQUAL_INT(CT_EMPTY, headers[15]);// Last

  free(headers);

  
  // Do the same with different params.
  expected_bytes = Heap_Size_From(128);
  actual_bytes   = initHeader(&headers, 128);
  TEST_ASSERT_EQUAL_INT(expected_bytes, actual_bytes);
  TEST_ASSERT_EQUAL_INT(CT_EMPTY, headers[0]);  // First
  TEST_ASSERT_EQUAL_INT(CT_EMPTY, headers[63]);
  TEST_ASSERT_EQUAL_INT(CT_EMPTY, headers[64]);
  TEST_ASSERT_EQUAL_INT(CT_EMPTY, headers[127]);// Last

  free(headers);  
}
