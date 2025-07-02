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
 mde_INDEX_OUT_OF_BOUND,
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
