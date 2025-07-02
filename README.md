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
 and you will get the following functions:
 
```
 mde_createBoolArr(int len)
 mde_isBoolSafeArr(mde_Bool* var)
 mde_deleteBoolArr(mde_Bool* var)
 mde_resizeBoolArr(mde_Bool* var, int len)
```

Define **mde_RECOMMENDED** if u want structs and functions for all the basic C data types

```
 #define mde_RECOMMENDED
 #include "mde.h"
``` 
