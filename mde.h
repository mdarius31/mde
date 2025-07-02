#define mde_bool bool
#define mde_true true
#define mde_false false
#define mde_memcpy(ptr1, ptr2, size) memcpy(ptr1, ptr2, size)
#define mde_realloc(ptr, size) realloc(ptr, size)
#define mde_malloc(size) malloc(size)
#define mde_free(var) free(var)
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
 TYPE* data;\
 int len;\
} mde_##NAME;\
mde_##NAME* mde_create##NAME(int len) {\
 mde_##NAME* result = mde_malloc(sizeof(mde_##NAME));\
 result->err = mde_NO_ERRORS;\
 result->len = len;\
 result->data = mde_malloc(sizeof(TYPE) * len);\
 \
 if(result->data == mde_NULL) {\
  result->err = mde_FAILED_TO_ALLOCATE_MEMORY;\
 }\
 return result;\
}\
mde_bool mde_is##NAME##Safe(mde_##NAME* var) {\
 if(var->err == mde_NO_ERRORS) return mde_true;\
 return mde_false;\
}\
void mde_delete##NAME(mde_##NAME* var) {\
 mde_free(var->data);\
 var->len = 0;\
 mde_free(var);\
}\
mde_##NAME* mde_resize##NAME(mde_##NAME* var, int len) {\
 mde_##NAME* resized = mde_create##NAME(len);\
 if(!mde_is##NAME##Safe(resized)) return resized;\
 for(int i = 0; i < var->len; i++) resized->data[i] = var->data[i];\
 if(len < var->len) resized->err = mde_POTENTIAL_DATA_LOSS;\
 return resized;\
}\
mde_##NAME* mde_get##NAME##AtIndex(mde_##NAME* var, int index) {\
 mde_##NAME* result = mde_create##NAME(1);\
 if(index >= var->len || index < 0) {\
   result->err = mde_INDEX_OUT_OF_BOUNDS;\
 } else {\
   result->data[0] = var->data[index];\
 }\
 return result;\
}

#ifdef mde_RECOMMENDED

 mde_CREATE(bool, BoolArr)
 
 mde_CREATE(char, CharArr)
 mde_CREATE(signed char, SCharArr)
 mde_CREATE(unsigned char, UCharArr)
 
 mde_CREATE(short, ShortArr)
 mde_CREATE(short int , ShortIntArr)
 mde_CREATE(signed short, SShortArr)
 mde_CREATE(signed short int, SShortIntArr)

 mde_CREATE(unsigned short, UShortArr)
 mde_CREATE(unsigned short int, UShortIntArr)
 
 mde_CREATE(int, IntArr)
 mde_CREATE(signed, SignedArr)
 mde_CREATE(signed int, SIntArr)
 
 mde_CREATE(unsigned, UnsignedArr) 
 mde_CREATE(unsigned int, UIntArr)

 mde_CREATE(long, LongArr)
 mde_CREATE(long int, LongIntArr)
 mde_CREATE(signed long, SLongArr)
 mde_CREATE(signed long int, SLongIntArr)

 mde_CREATE(unsigned long, ULongArr)
 mde_CREATE(unsigned long int, ULongIntArr)

 mde_CREATE(long long, LongLongArr)
 mde_CREATE(long long, LongLongIntArr)
 mde_CREATE(signed long long, SLongLongArr)
 mde_CREATE(signed long long int, SLongLongIntArr)

 mde_CREATE(unsigned long long, uLongLongArr)
 mde_CREATE(unsigned long long int, uLongLongIntArr)

 mde_CREATE(float, FloatArr)
 
 mde_CREATE(double, DoubleArr)
 
 mde_CREATE(long double, LongDoubleArr)
  
#endif
