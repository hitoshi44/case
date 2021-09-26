#include "unity/unity.h"
#include "../src/hash.h"

void setUp(void){}    //not need
void tearDown(void){} //not need

void test_HashDeterministic(void);

int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_HashDeterministic);
  return UNITY_END();
}

void test_HashDeterministic(void)
{
  char ascii[64]    = "abcdXYZ@01230123()";
  char nonascii[64] = "カラビ・ヤウ多様体🐶";
  int modulo = 3141592653;

  unsigned int prehash;

  prehash = hash(ascii, modulo);
  TEST_ASSERT_EQUAL_UINT(prehash, hash(ascii, modulo));

  prehash = hash(nonascii, modulo);
  TEST_ASSERT_EQUAL_UINT(prehash, hash(nonascii, modulo));  

  TEST_ASSERT_NOT_EQUAL_UINT(hash("Hash diff: A", modulo), hash("Hash diff: B", modulo));
}