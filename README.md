# mde.h
mde.h contains macros that generate array structs
 and useful functions for these structs from your specified data type

the purpose of this library is to make C programming safer


 lets say you want functions for the bool data type:
 
```
 #include "mde.h"
 mde_create(bool, Bool)
```
 
**bool** is the type that its inside and **Bool** will be the string used to compose the function names
 and you will get the following functions:
 
```
 mde_createBool(int len)
 mde_isBoolSafe(mde_Bool* var)
 mde_deleteBool(mde_Bool* var)
 mde_resizeBool(mde_Bool* var, int len)
```
