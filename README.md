# mde.h
mde.h contains macros that generate array structs
 and useful functions for these structs from your specified data type.

The purpose of this library is to make C programming safer


Lets say you want functions for the bool data type:
 
```
 #include "mde.h"
 mde_CREATE(bool, Bool)
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

mde_Bool *mde_createBool();
mde_Bool *mde_deleteBool(mde_Bool *val);
_Bool mde_isBoolSafe(mde_Bool *val);

typedef struct {
  mde_Error err;
  _Bool *val;
  int len;
} mde_BoolArr;

int mde_BoolArrSize = sizeof(mde_BoolArr);

mde_BoolArr *mde_createBoolArr(int len);
_Bool mde_isBoolArrSafe(mde_BoolArr *val);
mde_BoolArr *mde_deleteBoolArr(mde_BoolArr *val);
mde_BoolArr *mde_resizeBoolArr(mde_BoolArr *val, int len);
mde_Bool *mde_getBoolArrAtIndex(mde_BoolArr *val, int index);
```

Define **mde_RECOMMENDED** if u want structs and functions for all the basic C data types

```
 #define mde_RECOMMENDED
 #include "mde.h"
``` 
