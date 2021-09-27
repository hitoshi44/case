#include "unity/unity.h"
#include "../src/body.h"

#include <string.h>

void setUp(){}
void tearDown(){}

void test_InitBody(void);
void test_Standard_Set_Get_Body(void);
void test_Exceed_Size_Set_Body(void);

int main(void)
{
  UNITY_BEGIN();

  RUN_TEST(test_InitBody);
  RUN_TEST(test_Standard_Set_Get_Body);
  RUN_TEST(test_Exceed_Size_Set_Body);

  return UNITY_END();
}

void test_InitBody(void)
{
  ctBody b;

  b = initBody(32, 64);

  TEST_ASSERT_EQUAL_INT(32, b.data_size);
  TEST_ASSERT_EQUAL_INT(64,  b.data_count);

  int i;
  for(i=0; i<8; i++)
  {
    TEST_ASSERT_EQUAL_HEX8(0xff, (b.bits)[i]); 
  }

  free(b.bits);
  free(b.ctData);
}
void test_Standard_Set_Get_Body(void)
{
  ctBody b;
  b = initBody(32, 64);  

  // set
  char key[16];
  char val[16];
  int i;
  for(i=0; i<8; i++)
  {
    sprintf(key, "key:%d", i);
    sprintf(val, "value:%d",i*256);
    TEST_ASSERT_EQUAL_INT(i, setData(&b, key, 16, val, 16));
  }

  // get
  char *result;
  result = getData(&b, 4);
  TEST_ASSERT_EQUAL_CHAR('k', result[0]);
  TEST_ASSERT_EQUAL_CHAR('e', result[1]);
  TEST_ASSERT_EQUAL_CHAR('y', result[2]);
  TEST_ASSERT_EQUAL_CHAR(':', result[3]);
  TEST_ASSERT_EQUAL_CHAR('4', result[4]);
  TEST_ASSERT_EQUAL_CHAR('v', result[16]);
  TEST_ASSERT_EQUAL_CHAR('a', result[17]);
  TEST_ASSERT_EQUAL_CHAR('l', result[18]);
  TEST_ASSERT_EQUAL_CHAR('u', result[19]);
  TEST_ASSERT_EQUAL_CHAR('e', result[20]);
  TEST_ASSERT_EQUAL_CHAR(':', result[21]);
  TEST_ASSERT_EQUAL_CHAR('1', result[22]);
  TEST_ASSERT_EQUAL_CHAR('0', result[23]);
  TEST_ASSERT_EQUAL_CHAR('2', result[24]);
  TEST_ASSERT_EQUAL_CHAR('4', result[25]);

  TEST_ASSERT_EQUAL_HEX8(0x00,result[26]);

  free(b.ctData);
  free(b.bits);
}
void test_Exceed_Size_Set_Body(void)
{
  ctBody b;
  b = initBody(8, 128);

  char key[4] = "key";
  char val[4] = "val";
  int i;

  // fill up data array.
  for(i=0; i<128; i++)
  {
    TEST_ASSERT_EQUAL_INT(i, setData(&b, key, 4, val, 4));
  }

  // exceed setting.
  for(; i<256; i++)
  {
    TEST_ASSERT_EQUAL_INT(-1, setData(&b, key, 4, val, 4));
  }

  // make a few space.
  delData(&b, 13);     
  delData(&b, 17);
  delData(&b, 19);
  delData(&b, 23);
  delData(&b, 29);

  TEST_ASSERT_EQUAL_INT(13, setData(&b, key, 4, val, 4));
  TEST_ASSERT_EQUAL_INT(17, setData(&b, key, 4, val, 4));
  TEST_ASSERT_EQUAL_INT(19, setData(&b, key, 4, val, 4));
  TEST_ASSERT_EQUAL_INT(23, setData(&b, key, 4, val, 4));
  TEST_ASSERT_EQUAL_INT(29, setData(&b, key, 4, val, 4));

  free(b.bits);
  free(b.ctData);
}
















