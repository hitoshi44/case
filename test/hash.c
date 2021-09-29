#include "unity/unity.h"
#include "../src/hash.h"

void setUp(void){}    //not need
void tearDown(void){} //not need

void test_HashDeterministic(void);
void test_Vectors(void);

int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_HashDeterministic);
  return UNITY_END();
}

void test_HashDeterministic(void)
{
  // No meaning. A Random modulo for hash.
  int modulo = 3141592653;

  unsigned int hash_1;
  unsigned int hash_2;


  // Same inputs must be return same value.
  hash_1 = hash("abcdXYZ@01230123()", modulo);
  hash_2 = hash("abcdXYZ@01230123()", modulo);
  TEST_ASSERT_EQUAL_UINT(hash_1,hash_2);

  hash_1 = hash("カラビ・ヤウ多様体🐶", modulo);
  hash_2 = hash("カラビ・ヤウ多様体🐶", modulo);
  TEST_ASSERT_EQUAL_UINT(hash_1,hash_2);


  // Different inputs should return differents.
  hash_1 = hash("abcdXYZ@01230123()",   modulo);
  hash_2 = hash("カラビ・ヤウ多様体🐶", modulo);
  TEST_ASSERT_NOT_EQUAL_UINT(hash_1, hash_2);
}