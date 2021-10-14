#include "unity/unity.h"
#include "../src/table.h"

#include <string.h>

void setUp(){}
void tearDown(){}

void test_InitTable(void);
void test_Add_and_Get(void);
void test_Add_and_Has(void);
void test_Del_and_Pop(void);
void test_OverAdding(void);

int main(void)
{
  UNITY_BEGIN();

  RUN_TEST(test_InitTable);
  RUN_TEST(test_Add_and_Get);
  RUN_TEST(test_Add_and_Has);
  RUN_TEST(test_Del_and_Pop);
  RUN_TEST(test_OverAdding);

  return UNITY_END();
}

void test_InitTable(void)
{
  CaseTable table, *t;


  // Create Table with info of
  // Key_Size, Value_Size, Item_Counts
  table = createCaseTable(32, 100, 20);
  
  // Allocated heap volume is calculated as below:
  // ( (sizeof(KV Data)+sizeof(int)) * Item_Counts) + sizeof(BitFlags)
  // 
  // sizeof(KV Data) and Item_Counts are multiples of sizeof(int)
  // Sizeof BitFlags, which is tiny bits array for searching "empty" space for data,
  // is (Item_Counts)/8.
  int heap_volume;
  switch(sizeof(int)){
    case 32: heap_volume = (160 + 32) * 32 + (32/8);break;
    case 64: heap_volume = (192 + 64) * 64 + (64/8);break;
    case 16: heap_volume = (144 + 16) * 32 + (32/8);break;
    default: heap_volume = -1;// Unknown.
  }
  TEST_ASSERT_EQUAL_INT(heap_volume, table.total_allocated_heap);

  // Free the memory allocated.
  // For ordinary use case, this is not necessary.
  ctFree(&table);


  // Do the same with another Init param.
  // CaseTable allow valuesize:0, this leads key only table.
  table = createCaseTable(253, 0, 500);
  switch(sizeof(int)){
    case 32: heap_volume = (256 + 32) * 512 + (512/8);break;
    case 64: heap_volume = (256 + 64) * 512 + (512/8);break;
    case 16: heap_volume = (256 + 16) * 512 + (512/8);break;
    default: heap_volume = -1;// Unknown.
  }
  TEST_ASSERT_EQUAL_INT(heap_volume, table.total_allocated_heap);  

  ctFree(&table);
}
void test_Add_and_Fetch(void)
{
  CaseTable table, *t;
  table = createCaseTable(64, 192, 500);
  t = &table;

  // Add and Fetch string literal.
  // Fetch retrieve the pointer to the value.

  TEST_ASSERT_EQUAL_INT( 0, ctPut(t, "key", "value"));
  TEST_ASSERT_EQUAL_INT( 0, ctFetch(t, "key"));
  TEST_ASSERT_EQUAL_STRING("value", t->value); 

  TEST_ASSERT_EQUAL_INT( 0, ctPut(t, "key2", "value2"));
  TEST_ASSERT_EQUAL_INT( 0, ctFetch(t, "key2"));
  TEST_ASSERT_EQUAL_STRING("value2", t->value);

  // Fetch NOT-exist key returns -1.
  TEST_ASSERT_EQUAL_INT( -1, ctFetch(t, "No Key"));

  // Get returns the value:string.
  //     does NOT write to table.value.
  TEST_ASSERT_EQUAL_STRING("value",  ctGet(t, "key"));
  TEST_ASSERT_EQUAL_STRING("value2", ctGet(t, "key2"));
  // For Not-exist key, returns "\0"
  TEST_ASSERT_EQUAL_STRING(   "\0",  ctGet(t, "No Key"));


  ctFree(t);
}
void test_Add_and_Has(void)
{
  CaseTable table, *t;
  table = createCaseTable(32, 32, 64);
  t = &table;

  ctPut(t, "key:1", "value:1");
  ctPut(t, "key:2", "value:2");

  // Exist
  TEST_ASSERT_EQUAL_INT( 1, ctHas(t, "key:1"));
  TEST_ASSERT_EQUAL_INT( 1, ctHas(t, "key:2"));

  // Not Exist key
  TEST_ASSERT_EQUAL_INT( 0, ctHas(t, "Noot"));

  ctFree(t);
}
void test_Del_and_Pop(void)
{
  CaseTable table, *t;
  table = createCaseTable(32, 0, 64);
  t = &table;

  // Test Del
  ctPut(t, "test_delete", "");
  TEST_ASSERT_EQUAL_INT( 1, ctHas(t, "test_delete"));
  ctDel(t, "test_delete");
  TEST_ASSERT_EQUAL_INT( 0, ctHas(t, "test_delete"));

  // Test Pop
  // Pop return int but NOT string, it just fetch and delete.
  ctPut(t, "test_pop", "");
  TEST_ASSERT_EQUAL_INT( 1, ctHas(t, "test_pop")); // Has
  TEST_ASSERT_EQUAL_INT( 1, ctPop(t, "test_pop")); // Pop
  // After Pop.
  TEST_ASSERT_EQUAL_STRING("test_pop", t->key);
  TEST_ASSERT_EQUAL_INT( 0, ctHas(t, "test_pop")); // should return 0:false.


  ctFree(t);
}
void test_OverAdding(void)
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

  // Exceed Adding.
  for( ; i<100; i++)
  {
    sprintf(key, "key<%d>", i);
    TEST_ASSERT_EQUAL_INT(-1, ctPut(t, key, ""));
  }


  ctFree(t);
}