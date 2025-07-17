#ifndef MDE
#define MDE
// CONFIG
#define mdeLOGS true
#define mdeLogTimeFmt "%d-%m-%Y %H:%M:%S"
//

typedef enum {
 mdeNO_ERRORS,
 NULL_VALUE,
 mdeINDEX_OUT_OF_BOUNDS,
 mdeFAILED_TO_ALLOCATE_MEMORY,
 mdeFAILED_TO_REALLOCATE_MEMORY,
 mdePOTENTIAL_DATA_LOSS
} mdeError;
 

#define mdeLog(val) do {\
 mdeError err = *(mdeError*)val;\
 if(err ==  mdeNO_ERRORS) break;\
 \
 char* prefix = "[MDE]";\
 char* errStr = NULL;\
\
 switch(err) {\
  case mdeNO_ERRORS:\
   errStr = "NO ERRORS";\
   break;\
 \
  case NULL_VALUE:\
   errStr = "NULL VALUE";\
   break;\
\
  case mdeINDEX_OUT_OF_BOUNDS:\
   errStr = "INDEX OUT OF BOUND";\
   break;\
   \
  case mdeFAILED_TO_ALLOCATE_MEMORY:\
   errStr = "FAILED TO ALLOCATE MEMORY";\
   break;\
   \
  case mdeFAILED_TO_REALLOCATE_MEMORY:\
   errStr = "FAILED TO REALLOCATE MEMORY";\
   break;\
   \
  case mdePOTENTIAL_DATA_LOSS:\
   errStr = "POTENTIAL DATA LOSS";\
   break; \
\
  default:\
   break;\
 }\
\
 char buffer[100];\
 {\
  time_t rawtime;\
  struct tm *info;\
  \
  time(&rawtime);\
  info = localtime(&rawtime);\
 \
  strftime(buffer, sizeof(buffer), mdeLogTimeFmt, info);\
 \
 \
 }\
\
 int line = __LINE__;\
 char* time = buffer;\
 char* file = __FILE__;\
 \
 if(errStr == NULL) {\
  if(mdeLOGS) fprintf(stderr, "[MDE] [%s] UNKNOWN ERROR IN \"%s\" ON LINE %i\n", time, file, line);\
  break;\
 } \
 \
 char* template = "%s [%s] %s IN \"%s\" ON LINE %i\n";\
 \
 int size = snprintf(NULL, 0, template, prefix, time, errStr, file, line) + 1;\
\
 char* finalErrStr = malloc(size);\
 \
 if(finalErrStr == NULL) {\
  if(mdeLOGS) fprintf(stderr, "[MDE] [%s] CANT LOG ERROR IN \"%s\" ON LINE %i\n", time, file, line);\
  break;\
 }\
 snprintf(finalErrStr, size, template, prefix, time, errStr, file, line);\
 \
 if(mdeLOGS) {\
  fprintf(stderr, finalErrStr);\
  fflush(stderr);\
 }\
 else (void)(finalErrStr);\
\
 free(finalErrStr);\
\
} while(false);\



bool mdeHasErr(void* val) {
 mdeError err = *(mdeError*)val;
 return err != mdeNO_ERRORS;
}

 


#define mdeGen(TYPE, NAME)\
typedef struct {\
 mdeError err;\
 TYPE val;\
} mde##NAME;\
\
const int mde##NAME##Size = sizeof(mde##NAME);\
\
static inline mde##NAME* mdeNew##NAME() {\
 mde##NAME* result = malloc(mde##NAME##Size);\
 if(result == NULL) return result;\
 result->err = mdeNO_ERRORS;\
 return result;\
}\
\
static inline mde##NAME* mdeRm##NAME(mde##NAME* val) {\
 free(val);\
 val = NULL;\
 return val;\
}\
\
static inline mdeError mdeGet##NAME##Err(mde##NAME* val) {\
 if(val == NULL) return NULL_VALUE;\
 else return val->err;\
}\
\
static inline bool mdeIs##NAME##Safe(mde##NAME* val) {\
 if(mdeGet##NAME##Err(val) == mdeNO_ERRORS) return true;\
 return false;\
}\
\
typedef struct {\
 mdeError err;\
 TYPE* val;\
 int len;\
} mde##NAME##Arr;\
\
const int mde##NAME##ArrSize = sizeof(mde##NAME##Arr);\
\
static inline mde##NAME##Arr* mdeNew##NAME##Arr(int len) {\
 mde##NAME##Arr* result = malloc(mde##NAME##ArrSize);\
 if(result == NULL) return result;\
 result->err = mdeNO_ERRORS;\
 result->len = len;\
 result->val = malloc(sizeof(TYPE) * len);\
 \
 if(result->val == NULL) result->err = mdeFAILED_TO_ALLOCATE_MEMORY;\
 return result;\
}\
\
static inline mdeError mdeGet##NAME##ArrErr(mde##NAME##Arr* arr) {\
 if(arr == NULL) return NULL_VALUE;\
 else return arr->err;\
}\
\
static inline bool mdeIs##NAME##ArrSafe(mde##NAME##Arr* arr) {\
 if(mdeGet##NAME##ArrErr(arr) == mdeNO_ERRORS) return true;\
 return false;\
}\
\
static inline mde##NAME##Arr* mdeRm##NAME##Arr(mde##NAME##Arr* arr) {\
 free(arr->val);\
 arr->val = NULL;\
 arr->len = 0;\
 free(arr);\
 arr = NULL;\
 return arr;\
}\
\
static inline mde##NAME##Arr* mdeResize##NAME##Arr(mde##NAME##Arr* arr, int len) {\
 mde##NAME##Arr* resized = mdeNew##NAME##Arr(len);\
 if(!mdeIs##NAME##ArrSafe(resized)) {\
  if(resized != NULL) resized->err = mdeGet##NAME##ArrErr(resized);\
  return resized;\
 }\
 for(int i = 0; i < arr->len; i++) resized->val[i] = arr->val[i];\
 if(len < arr->len) resized->err = mdePOTENTIAL_DATA_LOSS;\
 return resized;\
}\
\
static inline bool mdeIsIndexValid##NAME(mde##NAME##Arr* arr, int index) {\
 if(index >= arr->len || index < 0) return false;\
 else return true;\
}\
\
static inline mde##NAME* mdeGet##NAME##At(mde##NAME##Arr* arr, int index) {\
 mde##NAME* result = mdeNew##NAME();\
 if(!mdeIs##NAME##Safe(result)) {\
  if(result != NULL) result->err = mdeGet##NAME##Err(result);\
  return result;\
 }\
 if(!mdeIsIndexValid##NAME(arr, index)) {\
   result->err = mdeINDEX_OUT_OF_BOUNDS;\
 } else {\
   result->val = arr->val[index];\
 }\
 return result;\
}\
\
static inline mde##NAME##Arr* mdeSet##NAME##At(mde##NAME##Arr* arr, TYPE val, int index) {\
 if(arr == NULL) return arr;\
 if(!mdeIsIndexValid##NAME(arr, index)) {\
  arr->err = mdeINDEX_OUT_OF_BOUNDS;\
 } else {\
  arr->val[index] = val;\
 }\
 return arr;\
}\
\
static inline mde##NAME##Arr* mdeNew##NAME##ArrFrom(TYPE* val, int len) {\
 mde##NAME##Arr* result = mdeNew##NAME##Arr(len);\
 if(!mdeIs##NAME##ArrSafe(result)) return result;\
 if(val == NULL) return result;\
 for(int i = 0; i < len; i++) mdeSet##NAME##At(result, val[i], i);\
 return result;\
}\
\
static inline mde##NAME##Arr* mde##NAME##ArrAdd(mde##NAME##Arr* arr, TYPE val) {\
 int i = arr->len;\
 mde##NAME##Arr* result = mdeResize##NAME##Arr(arr, arr->len + 1);\
 if(!mdeIs##NAME##ArrSafe(result)) {\
  if(result != NULL) {\
   result->err = mdeGet##NAME##ArrErr(result);\
  }\
  return result;\
 }\
 result = mdeSet##NAME##At(result, val, i);\
 mdeRm##NAME##Arr(arr);\
 return result;\
}\
\
static inline mde##NAME##Arr* mdeCombine##NAME##Arr(mde##NAME##Arr* arr1, mde##NAME##Arr* arr2) {\
 int len = arr1->len + arr2->len;\
 mde##NAME##Arr* result = mdeNew##NAME##Arr(len);\
 if(!mdeIs##NAME##ArrSafe(result)) {\
  if(result != NULL) {\
   result->err = mdeGet##NAME##ArrErr(result);\
  }\
  return result;\
 }\
 for(int i = 0; i < arr1->len; i++) mdeSet##NAME##At(result, arr1->val[i], i);\
 for(int i = 0; i < arr2->len; i++) mdeSet##NAME##At(result, arr2->val[i], arr1->len + i);\
 return result;\
}\
\
static inline mdeError mdeLoop##NAME##Arr(mde##NAME##Arr* arr, bool callback(mde##NAME##Arr* arr, TYPE val, int i)) {\
 if(!mdeIs##NAME##ArrSafe(arr)) return mdeGet##NAME##ArrErr(arr);\
 if(callback == NULL) return NULL_VALUE;\
 for(int i = 0; i < arr->len; i++) {\
  if(!callback(arr, arr->val[i], i)) break;\
 }\
 return mdeNO_ERRORS;\
}\
\
static inline mde##NAME##Arr* mdeSwitch##NAME(mde##NAME##Arr* arr, int i1, int i2) {\
 if(!mdeIsIndexValid##NAME(arr, i1) || !mdeIsIndexValid##NAME(arr, i2)) {\
  arr->err = mdeINDEX_OUT_OF_BOUNDS;\
  return arr;\
 }\
 \
 mde##NAME* val1 = mdeGet##NAME##At(arr, i1);\
 if(!mdeIs##NAME##Safe(val1)) {\
  arr->err = mdeGet##NAME##Err(val1);\
  mdeRm##NAME(val1);\
  return arr;\
 }\
\
 mde##NAME* val2 = mdeGet##NAME##At(arr, i2);\
 if(!mdeIs##NAME##Safe(val2)) {\
  arr->err = mdeGet##NAME##Err(val2);\
  mdeRm##NAME(val2);\
  return arr;\
 }\
 \
 arr = mdeSet##NAME##At(arr, val2->val, i1);\
 arr = mdeSet##NAME##At(arr, val1->val, i2);\
 \
 val1 = mdeRm##NAME(val1);\
 val2 = mdeRm##NAME(val2);\
 \
 return arr;\
}\

#ifdef mdeBasic

 mdeGen(bool, Bool)
 
 mdeGen(char, Char)
 mdeGen(signed char, SChar)
 mdeGen(unsigned char, UChar)
 
 mdeGen(short, Short)
 mdeGen(short int , ShortInt)
 mdeGen(signed short, SShort)
 mdeGen(signed short int, SShortInt)

 mdeGen(unsigned short, UShort)
 mdeGen(unsigned short int, UShortInt)
 
 mdeGen(int, Int)
 mdeGen(signed, Signed)
 mdeGen(signed int, SInt)
 
 mdeGen(unsigned, Unsigned) 
 mdeGen(unsigned int, UInt)

 mdeGen(long, Long)
 mdeGen(long int, LongInt)
 mdeGen(signed long, SLong)
 mdeGen(signed long int, SLongInt)

 mdeGen(unsigned long, ULong)
 mdeGen(unsigned long int, ULongInt)

 mdeGen(long long, LongLong)
 mdeGen(long long, LongLongInt)
 mdeGen(signed long long, SLongLong)
 mdeGen(signed long long int, SLongLongInt)

 mdeGen(unsigned long long, uLongLong)
 mdeGen(unsigned long long int, uLongLongInt)

 mdeGen(float, Float)
 
 mdeGen(double, Double)
 
 mdeGen(long double, LongDouble)

/* mmdeBasic */
#endif

/* MDE */
#endif 
