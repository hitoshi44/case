#include "unity/unity.h"
#include "../src/table.h"

#include <string.h>

void setUp(){}
void tearDown(){}

void test_InitTable(void);
void test_Put_and_Fet(void);
void test_Put_and_Has(void);
void test_Del_and_Pop(void);
void test_OverPuting(void);

int main(void)
{
  UNITY_BEGIN();

  RUN_TEST(test_InitTable);
  RUN_TEST(test_Put_and_Fet);
  RUN_TEST(test_Put_and_Has);
  RUN_TEST(test_Del_and_Pop);
  RUN_TEST(test_OverPuting);

  return UNITY_END();
}

// Capacity is multiple of sizeof(int), Size(bytes) is multiple of 4.
int calcCap(int req) {return ((req - 1)/sizeof(int) + 1)*sizeof(int);}
int calcSize(int req){return ((req + 2 - 1)/4 + 1) * 4; }
void test_InitTable(void)
{
  CaseTable table, *t;
  int heap_volume;
  int cap, size;

  // Create Table with info of
  // Key_Size, Value_Size, Item_Counts
  // Allocated heap volume is calculated as below:
  // ( (sizeof(KV Data)+sizeof(int)) * Item_Counts) + sizeof(BitFlags)
  // 
  // sizeof(KV Data) and Item_Counts are multiples of sizeof(int)
  // Sizeof BitFlags, which is tiny bits array for searching "empty" space for data,
  // is (Item_Counts)/8.
  table = createCaseTable(32, 100, 20);
  cap = calcCap(20);
  size= calcSize(132);
  
  TEST_ASSERT_EQUAL_INT(cap,  table.capacity);
  TEST_ASSERT_EQUAL_INT(size, table.data_size);

  // HeaderVolume + BodyDataVolume + Body-bit-flagsVolume
  heap_volume = sizeof(int)*cap*8 + size*cap + ( cap/8 + 1 );
  TEST_ASSERT_EQUAL_INT(heap_volume, table.total_allocated_heap);

  // Free the memory allocated.
  // For ordinary use case, this is not necessary.
  ctFree(&table);


  // Do the same with another Init param.
  // CaseTable allow valuesize:0, this leads key only table.
  table = createCaseTable(253, 0, 500);
  cap = calcCap(500);
  size= calcSize(253);

  TEST_ASSERT_EQUAL_INT(cap,  table.capacity);
  TEST_ASSERT_EQUAL_INT(size, table.data_size);

  heap_volume = sizeof(int)*cap*8 + size*cap + ( cap/8 + 1);
  TEST_ASSERT_EQUAL_INT(heap_volume, table.total_allocated_heap);  

  ctFree(&table);
}
void test_Put_and_Fet(void)
{
  CaseTable table, *t;
  table = createCaseTable(64, 192, 500);
  t = &table;

  // Put and Fetch string literal.
  // Fetch retrieve the pointer to the value.

  TEST_ASSERT_EQUAL_INT( 0, ctPut(t, "key", "value"));
  TEST_ASSERT_EQUAL_INT( 0, ctFet(t, "key"));
  TEST_ASSERT_EQUAL_STRING("value", t->value); 

  TEST_ASSERT_EQUAL_INT( 0, ctPut(t, "key2", "value2"));
  TEST_ASSERT_EQUAL_INT( 0, ctFet(t, "key2"));
  TEST_ASSERT_EQUAL_STRING("value2", t->value);

  // Fetch NOT-exist key returns -1.
  TEST_ASSERT_EQUAL_INT( -1, ctFet(t, "No Key"));

  ctFree(t);
}
void test_Put_and_Has(void)
{
  CaseTable table, *t;
  table = createCaseTable(32, 32, 64);
  t = &table;

  ctPut(t, "key:1", "value:1");
  ctPut(t, "key:2", "value:2");

  // Exist
  TEST_ASSERT_EQUAL_INT( 0, ctHas(t, "key:1"));
  TEST_ASSERT_EQUAL_INT( 1, ctHas(t, "key:2"));

  // Not Exist key
  TEST_ASSERT_EQUAL_INT( -1, ctHas(t, "Noot"));

  ctFree(t);
}
void test_Del_and_Pop(void)
{
  CaseTable table, *t;
  table = createCaseTable(32, 0, 64);
  t = &table;

  // Test Del
  ctPut(t, "test_delete", "");
  TEST_ASSERT_EQUAL_INT( 0, ctHas(t, "test_delete"));
  ctDel(t, "test_delete");
  TEST_ASSERT_EQUAL_INT( -1, ctHas(t, "test_delete"));

  // Test Pop
  // Pop return int but NOT string, it just fetch and delete.
  ctPut(t, "test_pop", "");
  TEST_ASSERT_EQUAL_INT( 0, ctHas(t, "test_pop")); // Has
  TEST_ASSERT_EQUAL_INT( 0, ctPop(t, "test_pop")); // Pop
  // After Pop.
  TEST_ASSERT_EQUAL_STRING("test_pop", t->key);
  TEST_ASSERT_EQUAL_INT( -1, ctHas(t, "test_pop")); // should return 0:false.


  ctFree(t);
}
void test_OverPuting(void)
{
  CaseTable table, *t;
  table = createCaseTable(32, 0, 64);
  t = &table;

  int i;

  // Fill Up a table.
  char key[32];
  for(i=0; i<64; i++)
  {
    sprintf(key, "key<%d>", i);
    ctPut(t, key, "");
  }

  // Exceed Puting.
  for( ; i<100; i++)
  {
    sprintf(key, "key<%d>", i);
    TEST_ASSERT_EQUAL_INT(-1, ctPut(t, key, ""));
  }


  ctFree(t);
}
void test_OverSized(void)
{
  CaseTable table, *t;
  table = createCaseTable(8,16,32);
  t = &table;

  char key_long[10] = "Key Len=10";
  char val[17] = "Value Length = 17";
  int result;
  
  // Put over sized. It should work.
  result = ctPut(t, key_long, val);
  TEST_ASSERT_EQUAL_INT(0, result);

  // Fetch it.
  result = ctFet(t, key_long);
  TEST_ASSERT_EQUAL_INT(0, result);
  TEST_ASSERT_EQUAL_STRING("Key Len=", t->key);
  TEST_ASSERT_EQUAL_STRING("Value Length = 1", t->value);

  // Fetch with truncated into 8 bytes long. It should work.
  char key_ok[8] = "Key Len=";
  result = ctFet(t, key_ok);
  TEST_ASSERT_EQUAL_INT(0, result);
  TEST_ASSERT_EQUAL_STRING(key_ok, t->key);
}