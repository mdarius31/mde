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

#define mde_CREATE(TYPE, NAME)\
typedef struct {\
 mde_Error err;\
 TYPE val;\
} mde_##NAME;\
\
int mde_##NAME##Size = sizeof(mde_##NAME);\
\
mde_##NAME* mde_create##NAME() {\
 mde_##NAME* result = malloc(mde_##NAME##Size);\
 result->err = mde_NO_ERRORS;\
 if(result == mde_NULL) result->err = mde_FAILED_TO_ALLOCATE_MEMORY;\
 return result;\
}\
\
mde_##NAME* mde_delete##NAME(mde_##NAME* val) {\
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
\
\
/* ARRAY STUFF */ \
typedef struct {\
 mde_Error err;\
 TYPE* val;\
 int len;\
} mde_##NAME##Arr;\
\
int mde_##NAME##ArrSize = sizeof(mde_##NAME##Arr);\
\
mde_##NAME##Arr* mde_create##NAME##Arr(int len) {\
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
mde_bool mde_is##NAME##ArrSafe(mde_##NAME##Arr* val) {\
 if(val == mde_NULL) return mde_false;\
 if(val->err == mde_NO_ERRORS) return mde_true;\
 return mde_false;\
}\
\
mde_##NAME##Arr* mde_delete##NAME##Arr(mde_##NAME##Arr* val) {\
 mde_free(val->val);\
 val->val = mde_NULL;\
 val->len = 0;\
 mde_free(val);\
 val = mde_NULL;\
 return val;\
}\
\
mde_##NAME##Arr* mde_resize##NAME##Arr(mde_##NAME##Arr* val, int len) {\
 mde_##NAME##Arr* resized = mde_create##NAME##Arr(len);\
 if(!mde_is##NAME##ArrSafe(resized)) return resized;\
 for(int i = 0; i < val->len; i++) resized->val[i] = val->val[i];\
 if(len < val->len) resized->err = mde_POTENTIAL_DATA_LOSS;\
 return resized;\
}\
\
mde_##NAME* mde_get##NAME##ArrAtIndex(mde_##NAME##Arr* val, int index) {\
 mde_##NAME* result = mde_create##NAME();\
 if(index >= val->len || index < 0) {\
   result->err = mde_INDEX_OUT_OF_BOUNDS;\
 } else {\
   result->val = val->val[index];\
 }\
 return result;\
}

#ifdef mde_RECOMMENDED

 mde_CREATE(bool, Bool)
 
 mde_CREATE(char, Char)
 mde_CREATE(signed char, SChar)
 mde_CREATE(unsigned char, UChar)
 
 mde_CREATE(short, Short)
 mde_CREATE(short int , ShortInt)
 mde_CREATE(signed short, SShort)
 mde_CREATE(signed short int, SShortInt)

 mde_CREATE(unsigned short, UShort)
 mde_CREATE(unsigned short int, UShortInt)
 
 mde_CREATE(int, Int)
 mde_CREATE(signed, Signed)
 mde_CREATE(signed int, SInt)
 
 mde_CREATE(unsigned, Unsigned) 
 mde_CREATE(unsigned int, UInt)

 mde_CREATE(long, Long)
 mde_CREATE(long int, LongInt)
 mde_CREATE(signed long, SLong)
 mde_CREATE(signed long int, SLongInt)

 mde_CREATE(unsigned long, ULong)
 mde_CREATE(unsigned long int, ULongInt)

 mde_CREATE(long long, LongLong)
 mde_CREATE(long long, LongLongInt)
 mde_CREATE(signed long long, SLongLong)
 mde_CREATE(signed long long int, SLongLongInt)

 mde_CREATE(unsigned long long, uLongLong)
 mde_CREATE(unsigned long long int, uLongLongInt)

 mde_CREATE(float, Float)
 
 mde_CREATE(double, Double)
 
 mde_CREATE(long double, LongDouble)
  
#endif
