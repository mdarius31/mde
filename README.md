# mde.h
mde.h contains macros that generate array structs
 and useful functions for these structs from your specified data type.

The purpose of this library is to make C programming safer


Lets say you want functions for the bool data type:
 
```
 #include "mde.h"
 mdeGen(bool, Bool)
```
 
**bool** is the type that its inside and **Bool** will be the string used to compose the function names
 and you will get the following functions and structs:

 
**This works with any data type** 
```
 typedef enum {
   mdeNO_ERRORS,
   mdeNULL_VALUE,
   mdeINDEX_OUT_OF_BOUNDS,
   mdeFAILED_TO_ALLOCATE_MEMORY,
   mdeFAILED_TO_REALLOCATE_MEMORY,
   mdePOTENTIAL_DATA_LOSS
 } mdeError;

 mdeLog(val)
 _Bool mdeHasErr(void *val)
 
 typedef struct {
   mdeError err;
   char val;
 } mdeChar;
 const int mdeCharSize = sizeof(mdeChar);
 mdeChar *mdeNewChar()
 mdeChar *mdeRmChar(mdeChar *val)
 mdeError mdeGetCharErr(mdeChar *val)
 _Bool mdeIsCharSafe(mdeChar *val)
 
 typedef struct {
   mdeError err;
   char *val;
   int len;
 } mdeCharArr;
 const int mdeCharArrSize = sizeof(mdeCharArr);
 mdeCharArr *mdeNewCharArr(int len)
 mdeError mdeGetCharArrErr(mdeCharArr *arr)
 _Bool mdeIsCharArrSafe(mdeCharArr *arr)
 mdeCharArr *mdeRmCharArr(mdeCharArr *arr)
 mdeCharArr *mdeResizeCharArr(mdeCharArr *arr, int len)
 _Bool mdeIsIndexValidChar(mdeCharArr *arr, int index)
 mdeChar *mdeGetCharAt(mdeCharArr *arr, int index)
 mdeCharArr *mdeSetCharAt(mdeCharArr *arr, char val, int index)
 mdeCharArr *mdeNewCharArrFrom(char *val, int len)
 mdeCharArr *mdeCharArrAdd(mdeCharArr *arr, char val)
 mdeCharArr *mdeCombineCharArr(mdeCharArr *arr1, mdeCharArr *arr2)
 mdeError mdeLoopCharArr(mdeCharArr *arr, _Bool callback(mdeCharArr *arr, char val, int i))
 mdeCharArr *mdeSwitchChar(mdeCharArr *arr, int i1, int i2)
```

Define **mdeBasic** if u want structs and functions for all the basic C data types

```
 #define mdeBasic
 #include "mde.h"
``` 
