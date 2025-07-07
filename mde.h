#ifndef MDE
#define MDE
// CONFIG
#define mde_LOGS mde_true
#define mde_logTimeFmt "%d-%m-%Y %H:%M:%S"
//


#define mde_strftime(...) strftime(__VA_ARGS__)
#define mde_localtime(...) localtime(__VA_ARGS__)
#define mde_time(...) time(__VA_ARGS__) 
#define mde_tm struct tm
#define mde_time_t time_t
#define mde_bool bool
#define mde_true true
#define mde_false false
#define mde_memcpy(ptr1, ptr2, size) memcpy(ptr1, ptr2, size)
#define mde_realloc(ptr, size) realloc(ptr, size)
#define mde_malloc(size) malloc(size)
#define mde_free(val) free(val)
#define mde_NULL NULL


#define mde_snprintf(ptr, n, format, ...) snprintf(ptr, n, format, __VA_ARGS__)


// streams
#define mde_errst stderr
#define mde_outst stdout
#define mde_inst stdin

#define mde_fprintf(...) fprintf(__VA_ARGS__)
#define mde_printErr(...) fprintf(__VA_ARGS__) 



typedef enum {
 mde_NO_ERRORS,
 mde_NULL_VALUE,
 mde_INDEX_OUT_OF_BOUNDS,
 mde_FAILED_TO_ALLOCATE_MEMORY,
 mde_FAILED_TO_REALLOCATE_MEMORY,
 mde_POTENTIAL_DATA_LOSS
} mde_Error;
 

#define mde_log(err) do {\
 if(err ==  mde_NO_ERRORS) break;\
 \
 char* prefix = "[MDE]";\
 char* errStr = mde_NULL;\
\
 switch(err) {\
  case mde_NO_ERRORS:\
   errStr = "NO ERRORS";\
   break;\
 \
  case mde_NULL_VALUE:\
   errStr = "NULL VALUE";\
   break;\
\
  case mde_INDEX_OUT_OF_BOUNDS:\
   errStr = "INDEX OUT OF BOUND";\
   break;\
   \
  case mde_FAILED_TO_ALLOCATE_MEMORY:\
   errStr = "FAILED TO ALLOCATE MEMORY";\
   break;\
   \
  case mde_FAILED_TO_REALLOCATE_MEMORY:\
   errStr = "FAILED TO REALLOCATE MEMORY";\
   break;\
   \
  case mde_POTENTIAL_DATA_LOSS:\
   errStr = "POTENTIAL DATA LOSS";\
   break; \
\
  default:\
   break;\
 }\
\
 char buffer[100];\
 {\
  mde_time_t rawtime;\
  mde_tm *info;\
  \
  mde_time(&rawtime);\
  info = mde_localtime(&rawtime);\
 \
  mde_strftime(buffer, sizeof(buffer), mde_logTimeFmt, info);\
 \
 \
 }\
\
 int line = __LINE__;\
 char* time = buffer;\
 char* file = __FILE__;\
 \
 if(errStr == mde_NULL) {\
  if(mde_LOGS) mde_fprintf(mde_errst, "[MDE] [%s] UNKNOWN ERROR IN \"%s\" ON LINE %i\n", time, file, line);\
  break;\
 } \
 \
 char* template = "%s [%s] %s IN \"%s\" ON LINE %i\n";\
 \
 int size = mde_snprintf(mde_NULL, 0, template, prefix, time, errStr, file, line) + 1;\
\
 char* finalErrStr = mde_malloc(size);\
 \
 if(finalErrStr == mde_NULL) {\
  if(mde_LOGS) mde_fprintf(mde_errst, "[MDE] [%s] CANT LOG ERROR IN \"%s\" ON LINE %i\n", time, file, line);\
  break;\
 }\
 mde_snprintf(finalErrStr, size, template, prefix, time, errStr, file, line);\
 \
 if(mde_LOGS) mde_fprintf(mde_errst, finalErrStr);\
 else (void)(finalErrStr);\
\
 free(finalErrStr);\
\
} while(mde_false);\


#define mde_gen(TYPE, NAME)\
typedef struct {\
 mde_Error err;\
 TYPE val;\
} mde_##NAME;\
\
const int mde_##NAME##Size = sizeof(mde_##NAME);\
\
static inline mde_##NAME* mde_new##NAME() {\
 mde_##NAME* result = mde_malloc(mde_##NAME##Size);\
 if(result == mde_NULL) return result;\
 result->err = mde_NO_ERRORS;\
 return result;\
}\
\
static inline mde_##NAME* mde_rm##NAME(mde_##NAME* val) {\
 mde_free(val);\
 val = mde_NULL;\
 return val;\
}\
\
static inline mde_Error mde_get##NAME##Err(mde_##NAME* val) {\
 if(val == mde_NULL) return mde_NULL_VALUE;\
 else return val->err;\
}\
\
static inline mde_bool mde_is##NAME##Safe(mde_##NAME* val) {\
 if(mde_get##NAME##Err(val) == mde_NO_ERRORS) return mde_true;\
 return mde_false;\
}\
\
typedef struct {\
 mde_Error err;\
 TYPE* val;\
 int len;\
} mde_##NAME##Arr;\
\
const int mde_##NAME##ArrSize = sizeof(mde_##NAME##Arr);\
\
static inline mde_##NAME##Arr* mde_new##NAME##Arr(int len) {\
 mde_##NAME##Arr* result = mde_malloc(mde_##NAME##ArrSize);\
 if(result == mde_NULL) return result;\
 result->err = mde_NO_ERRORS;\
 result->len = len;\
 result->val = mde_malloc(sizeof(TYPE) * len);\
 \
 if(result->val == mde_NULL) result->err = mde_FAILED_TO_ALLOCATE_MEMORY;\
 return result;\
}\
\
static inline mde_Error mde_get##NAME##ArrErr(mde_##NAME##Arr* arr) {\
 if(arr == mde_NULL) return mde_NULL_VALUE;\
 else return arr->err;\
}\
\
static inline mde_bool mde_is##NAME##ArrSafe(mde_##NAME##Arr* arr) {\
 if(mde_get##NAME##ArrErr(arr) == mde_NO_ERRORS) return mde_true;\
 return mde_false;\
}\
\
static inline mde_##NAME##Arr* mde_rm##NAME##Arr(mde_##NAME##Arr* arr) {\
 mde_free(arr->val);\
 arr->val = mde_NULL;\
 arr->len = 0;\
 mde_free(arr);\
 arr = mde_NULL;\
 return arr;\
}\
\
static inline mde_##NAME##Arr* mde_resize##NAME##Arr(mde_##NAME##Arr* arr, int len) {\
 mde_##NAME##Arr* resized = mde_new##NAME##Arr(len);\
 if(!mde_is##NAME##ArrSafe(resized)) {\
  if(resized != mde_NULL) resized->err = mde_get##NAME##ArrErr(resized);\
  return resized;\
 }\
 for(int i = 0; i < arr->len; i++) resized->val[i] = arr->val[i];\
 if(len < arr->len) resized->err = mde_POTENTIAL_DATA_LOSS;\
 return resized;\
}\
\
static inline mde_bool mde_isIndexValid##NAME(mde_##NAME##Arr* arr, int index) {\
 if(index >= arr->len || index < 0) return mde_false;\
 else return mde_true;\
}\
\
static inline mde_##NAME* mde_get##NAME##At(mde_##NAME##Arr* arr, int index) {\
 mde_##NAME* result = mde_new##NAME();\
 if(!mde_is##NAME##Safe(result)) {\
  if(result != mde_NULL) result->err = mde_get##NAME##Err(result);\
  return result;\
 }\
 if(!mde_isIndexValid##NAME(arr, index)) {\
   result->err = mde_INDEX_OUT_OF_BOUNDS;\
 } else {\
   result->val = arr->val[index];\
 }\
 return result;\
}\
\
static inline mde_##NAME##Arr* mde_set##NAME##At(mde_##NAME##Arr* arr, TYPE val, int index) {\
 if(arr == mde_NULL) return arr;\
 if(!mde_isIndexValid##NAME(arr, index)) {\
  arr->err = mde_INDEX_OUT_OF_BOUNDS;\
 } else {\
  arr->val[index] = val;\
 }\
 return arr;\
}\
\
static inline mde_##NAME##Arr* mde_new##NAME##ArrFrom(TYPE* val, int len) {\
 mde_##NAME##Arr* result = mde_new##NAME##Arr(len);\
 if(!mde_is##NAME##ArrSafe(result)) return result;\
 if(val == mde_NULL) return result;\
 for(int i = 0; i < len; i++) mde_set##NAME##At(result, val[i], i);\
 return result;\
}\
\
static inline mde_##NAME##Arr* mde_##NAME##ArrAdd(mde_##NAME##Arr* arr, TYPE val) {\
 int i = arr->len;\
 mde_##NAME##Arr* result = mde_resize##NAME##Arr(arr, arr->len + 1);\
 if(!mde_is##NAME##ArrSafe(result)) {\
  if(result != mde_NULL) {\
   result->err = mde_get##NAME##ArrErr(result);\
  }\
  return result;\
 }\
 result = mde_set##NAME##At(result, val, i);\
 mde_rm##NAME##Arr(arr);\
 return result;\
}\
\
static inline mde_##NAME##Arr* mde_combine##NAME##Arr(mde_##NAME##Arr* arr1, mde_##NAME##Arr* arr2) {\
 int len = arr1->len + arr2->len;\
 mde_##NAME##Arr* result = mde_new##NAME##Arr(len);\
 if(!mde_is##NAME##ArrSafe(result)) {\
  if(result != mde_NULL) {\
   result->err = mde_get##NAME##ArrErr(result);\
  }\
  return result;\
 }\
 for(int i = 0; i < arr1->len; i++) mde_set##NAME##At(result, arr1->val[i], i);\
 for(int i = 0; i < arr2->len; i++) mde_set##NAME##At(result, arr2->val[i], arr1->len + i);\
 return result;\
}\
\
static inline mde_Error mde_loop##NAME##Arr(mde_##NAME##Arr* arr, bool callback(mde_##NAME##Arr* arr, TYPE val, int i)) {\
 if(!mde_is##NAME##ArrSafe(arr)) return mde_get##NAME##ArrErr(arr);\
 if(callback == mde_NULL) return mde_NULL_VALUE;\
 for(int i = 0; i < arr->len; i++) {\
  if(!callback(arr, arr->val[i], i)) break;\
 }\
 return mde_NO_ERRORS;\
}\
\
static inline mde_bool mde_logIf##NAME##ArrErr(mde_##NAME##Arr* arr) {\
 mde_Error err = mde_get##NAME##ArrErr(arr);\
 mde_log(err);\
 return mde_is##NAME##ArrSafe(arr);\
}\
\
static inline mde_bool mde_logIf##NAME##Err(mde_##NAME* val) {\
 mde_Error err = mde_get##NAME##Err(val);\
 mde_log(err);\
 return !mde_is##NAME##Safe(val);\
}\


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

/* mde_RECOMMENDED */
#endif

/* MDE */
#endif 
