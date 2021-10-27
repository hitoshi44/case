# Case
Case is a fixed size, no runtime-allocation Hash Table written in C.

## Features

Case employs open-addressing hashing, a little tweaking for a long period use.

### Complexity and Memory.

Complexity follows open-addressing **except for insertion**, every operations are on `O(1)`.

Insertion(`ctAdd`), to make constant terms of other oprations, is `O(n)` for searching empty K-V data index. However, the searching is just a iteration over continuous bit flags(`int[]`), it works quite fast.

Thanks for "header", average counts of access packets for rehash-searching( increment hash ) are:
| search success | count | 
| :---: | :---: |
| successed | `< 1.08` |
| failed    | `< 1.16` |

Allocated heap size of each data, with denoting
  - bytes of *int* : `I`
  - a multiple of 32 larger than *x* = `M32(x)`
  - a multiple of `I` larger than *x* = `MI[x]`
  - required key length = `k`
  - required value length = `v`
  - required item counts = `n`

| Data            | size (bytes) | Eplanations |
| -----           | :------      | ----------- |
| Item count capa | `MI[n]`      | The table capacity for item count is determined as MI[n]
| Each K-V data   | `M32[k+v+2]` | `+2` is for null terminated safety for max size key or value. |
| All K-Vs array   | `M32[k+v+2] * MI[n]`|
| All headers array| `I * MI[n]`  | "Header" is one of tweaks of Case. It contains acutal "index" to K-V data
| Bit flags of data| `MI[n]/8`   | "Bit flags" is also a tweak. It make searching empty K-V data position faster. 
| **Whole Heap**  | **`M32[k+v+2]*MI[n]` + `I*MI[n]` + `MI[n]/8`**  | |

### Tweaks

#### header : Low occupancy ratio with Little extra memory.

The table sacrifices a little memory to avoid hash-collision. Even if items count reach the max capacity, the level is as well as `12.5%` for an ordinary hash table. 

A hash(key) points to one of headers(`int[]`), then header indexes K-V data.
With allocating 8 times larger headers than actual table's capacity, tables occupancy ratio will be smaller than `12.5%` at worst with little extra memory space.

#### ctFet  : Just fetch a pointer without copying string.

CaseTable exposes you a method `ctFet`.
It just load the pointers for the key, and the value onto `CaseTable.key`, `CaseTable.value`. 
It gives you much chance to avoid over-head.

#### Good for long term period use.

Deletion(`ctDel`) does some cleaning (turn `deleted` into `empty`) if possible, the table performance will be quite stable for long term use.

## Usage

### Initialize.
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

### Put and Fet (similiar to Add and Get).

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
// So it does NOT copy string.
signal = ctFet(ct, key);

printf("%s\n", ct.key);
// => longer K
printf("%s\n", ct.value);

```

### Has and Del.

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


### Helpers.

```C
// Do the same with Fet and Del, but a little bit faster.
ctPop(ct, "target key");

ct.count; // store the actual count of items stored.

ctFree(ct);// free all alloced heap of the table.
```