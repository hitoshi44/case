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

void test_InitHeader(void)
{
  ctHeader *h;
  int i;
  
  TEST_ASSERT_EQUAL_INT(16*8*sizeof(ctHeader), initHeader(&h, 16));
  for(i=0; i<16; i++)
  {
    TEST_ASSERT_EQUAL_INT(-1, h[i]);
  }
  free(h);

  TEST_ASSERT_EQUAL_INT(128*8*sizeof(ctHeader), initHeader(&h, 128));
  for(i=0; i<128; i++)
  {
    TEST_ASSERT_EQUAL_INT(-1, h[i]);
  }
  free(h);  
}
