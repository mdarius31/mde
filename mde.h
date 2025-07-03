#define mde_bool bool
#define mde_true true
#define mde_false false
#define mde_memcpy(ptr1, ptr2, size) memcpy(ptr1, ptr2, size)
#define mde_realloc(ptr, size) realloc(ptr, size)
#define mde_malloc(size) malloc(size)
#define mde_free(val) free(val)
#define mde_NULL NULL

#define mde_LOG_LEVEL NONE

#define mde_printErr(format, ...) fprintf(stderr, format, __VA_ARGS__)
#define mde_print(format, ...) fprintf(stdout, format, __VA_ARGS__)

#if mde_LOG_LEVEL == NONE
 #undef mde_printErr
 #define mde_printErr 
 
 #undef mde_print
 #define mde_print
#endif 


typedef enum {
 mde_NO_ERRORS,
 mde_INDEX_OUT_OF_BOUNDS,
 mde_FAILED_TO_ALLOCATE_MEMORY,
 mde_FAILED_TO_REALLOCATE_MEMORY,
 mde_POTENTIAL_DATA_LOSS
} mde_Error;

#define mde_gen(TYPE, NAME)\
typedef struct {\
 mde_Error err;\
 TYPE val;\
} mde_##NAME;\
\
int mde_##NAME##Size = sizeof(mde_##NAME);\
\
mde_##NAME* mde_new##NAME() {\
 mde_##NAME* result = malloc(mde_##NAME##Size);\
 result->err = mde_NO_ERRORS;\
 if(result == mde_NULL) result->err = mde_FAILED_TO_ALLOCATE_MEMORY;\
 return result;\
}\
\
mde_##NAME* mde_rm##NAME(mde_##NAME* val) {\
 mde_free(val);\
 val = mde_NULL;\
 return val;\
}\
\
mde_bool mde_is##NAME##Safe(mde_##NAME* val) {\
 if(val == mde_NULL) return mde_false;\
 if(val->err == mde_NO_ERRORS) return mde_true;\
 return mde_false;\
}\
\
typedef struct {\
 mde_Error err;\
 TYPE* val;\
 int len;\
} mde_##NAME##Arr;\
\
int mde_##NAME##ArrSize = sizeof(mde_##NAME##Arr);\
\
mde_##NAME##Arr* mde_new##NAME##Arr(int len) {\
 mde_##NAME##Arr* result = mde_malloc(mde_##NAME##ArrSize);\
 result->err = mde_NO_ERRORS;\
 result->len = len;\
 result->val = mde_malloc(sizeof(TYPE) * len);\
 \
 if(result->val == mde_NULL) {\
  result->err = mde_FAILED_TO_ALLOCATE_MEMORY;\
 }\
 return result;\
}\
\
mde_bool mde_is##NAME##ArrSafe(mde_##NAME##Arr* arr) {\
 if(arr == mde_NULL) return mde_false;\
 if(arr->err == mde_NO_ERRORS) return mde_true;\
 return mde_false;\
}\
\
mde_##NAME##Arr* mde_rm##NAME##Arr(mde_##NAME##Arr* arr) {\
 mde_free(arr->val);\
 arr->val = mde_NULL;\
 arr->len = 0;\
 mde_free(arr);\
 arr = mde_NULL;\
 return arr;\
}\
\
mde_##NAME##Arr* mde_resize##NAME##Arr(mde_##NAME##Arr* arr, int len) {\
 mde_##NAME##Arr* resized = mde_new##NAME##Arr(len);\
 if(!mde_is##NAME##ArrSafe(resized)) return resized;\
 for(int i = 0; i < arr->len; i++) resized->val[i] = arr->val[i];\
 if(len < arr->len) resized->err = mde_POTENTIAL_DATA_LOSS;\
 return resized;\
}\
\
mde_bool mde_isIndexValid##NAME(mde_##NAME##Arr* arr, int index) {\
 if(index >= arr->len || index < 0) return mde_false;\
 else return mde_true;\
}\
\
mde_##NAME* mde_get##NAME##At(mde_##NAME##Arr* arr, int index) {\
 mde_##NAME* result = mde_new##NAME();\
 if(!mde_isIndexValid##NAME(arr, index)) {\
   result->err = mde_INDEX_OUT_OF_BOUNDS;\
 } else {\
   result->val = arr->val[index];\
 }\
 return result;\
}\
\
mde_##NAME##Arr* mde_set##NAME##At(mde_##NAME##Arr* arr, TYPE val, int index) {\
 if(!mde_isIndexValid##NAME(arr, index)) {\
  arr->err = mde_INDEX_OUT_OF_BOUNDS;\
 } else {\
  arr->val[index] = val;\
 }\
 return arr;\
}\
\
mde_##NAME##Arr* mde_new##NAME##ArrFrom(TYPE* val, int len) {\
 mde_##NAME##Arr* result = mde_new##NAME##Arr(len);\
 for(int i = 0; i < len; i++) mde_set##NAME##At(result, val[i], i);\
 return result;\
}\
\
mde_##NAME##Arr* mde_##NAME##ArrAdd(mde_##NAME##Arr* arr, TYPE val) {\
 int i = arr->len;\
 mde_##NAME##Arr* result = mde_resize##NAME##Arr(arr, arr->len + 1);\
 result = mde_set##NAME##At(result, val, i);\
 mde_rm##NAME##Arr(arr);\
 return result;\
}\
\
mde_##NAME##Arr* mde_combine##NAME##Arr(mde_##NAME##Arr* arr1, mde_##NAME##Arr* arr2) {\
 int len = arr1->len + arr2->len;\
 mde_##NAME##Arr* result = mde_new##NAME##Arr(len);\
 for(int i = 0; i < arr1->len; i++) mde_set##NAME##At(result, arr1->val[i], i);\
 for(int i = 0; i < arr2->len; i++) mde_set##NAME##At(result, arr2->val[i], arr1->len + i);\
 return result;\
}


#ifdef mde_RECOMMENDED

 mde_gen(bool, Bool)
 
 mde_gen(char, Char)
 mde_gen(signed char, SChar)
 mde_gen(unsigned char, UChar)
 
 mde_gen(short, Short)
 mde_gen(short int , ShortInt)
 mde_gen(signed short, SShort)
 mde_gen(signed short int, SShortInt)

 mde_gen(unsigned short, UShort)
 mde_gen(unsigned short int, UShortInt)
 
 mde_gen(int, Int)
 mde_gen(signed, Signed)
 mde_gen(signed int, SInt)
 
 mde_gen(unsigned, Unsigned) 
 mde_gen(unsigned int, UInt)

 mde_gen(long, Long)
 mde_gen(long int, LongInt)
 mde_gen(signed long, SLong)
 mde_gen(signed long int, SLongInt)

 mde_gen(unsigned long, ULong)
 mde_gen(unsigned long int, ULongInt)

 mde_gen(long long, LongLong)
 mde_gen(long long, LongLongInt)
 mde_gen(signed long long, SLongLong)
 mde_gen(signed long long int, SLongLongInt)

 mde_gen(unsigned long long, uLongLong)
 mde_gen(unsigned long long int, uLongLongInt)

 mde_gen(float, Float)
 
 mde_gen(double, Double)
 
 mde_gen(long double, LongDouble)
  
#endif
