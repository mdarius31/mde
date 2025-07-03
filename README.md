# mde.h
mde.h contains macros that generate array structs
 and useful functions for these structs from your specified data type.

The purpose of this library is to make C programming safer


Lets say you want functions for the bool data type:
 
```
 #include "mde.h"
 mde_gen(bool, Bool)
```
 
**bool** is the type that its inside and **Bool** will be the string used to compose the function names
 and you will get the following functions and structs:

 
**This works with any data type** 
```
 typedef struct {
   mde_Error err;
   _Bool val;
 } mde_Bool;
 int mde_BoolSize = sizeof(mde_Bool);
 mde_Bool *mde_newBool()
 mde_Bool *mde_rmBool(mde_Bool *val)
 _Bool mde_isBoolSafe(mde_Bool *val)
 
 typedef struct {
   mde_Error err;
   _Bool *val;
   int len;
 } mde_BoolArr;
 
 int mde_BoolArrSize = sizeof(mde_BoolArr);
 mde_BoolArr *mde_newBoolArr(int len)
 _Bool mde_isBoolArrSafe(mde_BoolArr *arr)
 mde_BoolArr *mde_rmBoolArr(mde_BoolArr *arr)
 mde_BoolArr *mde_resizeBoolArr(mde_BoolArr *arr, int len)
 _Bool mde_isIndexValidBool(mde_BoolArr *arr, int index)
 mde_Bool *mde_getBoolAt(mde_BoolArr *arr, int index)
 mde_BoolArr *mde_setBoolAt(mde_BoolArr *arr, _Bool val, int index)
 mde_BoolArr *mde_newBoolArrFrom(_Bool *val, int len)
 mde_BoolArr *mde_BoolArrAdd(mde_BoolArr *arr, _Bool val)
 mde_BoolArr *mde_combineBoolArr(mde_BoolArr *arr1, mde_BoolArr *arr2)
```

Define **mde_RECOMMENDED** if u want structs and functions for all the basic C data types

```
 #define mde_RECOMMENDED
 #include "mde.h"
``` 
