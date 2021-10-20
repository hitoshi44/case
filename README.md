# Case
Case is a fixed size, no runtime-allocation Hash Table written in C.

Case does heap allocation only once, when it's initializing, and does No extra allocations.

# Features

## Memory, Calculation Efficient with the balance.

## Fit for long period using without bothering GC.

# Usage

## Initialize.
Clone the repository into your directory, then include the header `case/src/table.h`.

```C
CaseTable createCaseTable(int key_size_bytes,
                          int value_size_bytes,
                          int table_item_capacity);
```

```C
  
CaseTable table, *ct;

// Create a table on your stack.
// Following code creates a table with,
// 100 KV pairs which is 8+64(+2) bytes long.
table = createCaseTable(8, 64, 100);
ct = &table;

printf("Total Volume of alloced heap:%d\n",
       table.total_allocated_heap);
```

## Put and Fet (similiar to Add and Get).

```C
char key[25]  = "longer KEY than required.";
char value[5] = "value";

int signal;

// Over sized key, value works after get truncated.
signal = ctPut(ct, key, value);
// Return 0 when successed, -1 when failed.
printf("%d\n", signal);

// ctFet fetches the target pointer into 
// table.key, table.value. 
signal = ctFet(ct, key);

printf("%s\n", ct.key);
// => longer K
printf("%s\n", ct.value);

```

## Has and Del.

```C
int result;

result = ctHas(ct, "some key");
if (result == -1) {
  printf("Case does NOT have the key!\n");
}
else {
  printf("Case has the key!\n");
  // result > -1.
}

result = ctDel(ct, "target key");

```


## Helpers.

```C
// Do the same with Fet and Del, but a little faster.
ctPop(ct, "target key");

ct.count; // store the actual count of items stored.

ctFree(ct);// free all alloced heap of the table.
```