#ifndef MDE
#define MDE
#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// CONFIG
#ifndef showLogWarn
 #define showLogWarn true
#endif

#ifndef showLogErr
 #define showLogErr true
#endif


#ifndef logTimeFmt
 #define logTimeFmt "%d-%m-%Y %H:%M:%S"
#endif
//

typedef enum {
 NO_ERRORS,
 NULL_VALUE,
 INDEX_OUT_OF_BOUNDS,
 FAILED_TO_ALLOCATE_MEMORY,
 FAILED_TO_REALLOCATE_MEMORY,
 POTENTIAL_DATA_LOSS,
} Error;

static inline char* formatErr(Error err) {
 if(err == POTENTIAL_DATA_LOSS)         return "POTENTIAL DATA LOSS";
 if(err == FAILED_TO_REALLOCATE_MEMORY) return "FAILED TO REALLOCATE MEMORY";
 if(err == FAILED_TO_ALLOCATE_MEMORY)   return "FAILED TO ALLOCATE MEMORY";
 if(err == INDEX_OUT_OF_BOUNDS)         return "INDEX OUT OF BOUNDS";
 if(err == NULL_VALUE)                  return "NULL VALUE";
 if(err == NO_ERRORS)                   return "NO ERRORS";
 return "UNKNOWN ERROR";
}

static inline bool hasErr(void* val) {
 Error err = *(Error*)val;
 return err != NO_ERRORS;
}

static inline char* logStr(void* val, char* file, int line) {
 Error err = val == NULL ? NULL_VALUE : *(Error*)val;
 if(err == NO_ERRORS) return NULL;
 
 char* errStr = NULL;
 errStr = formatErr(err);

 char buffer[100];
 {
  time_t rawtime;
  struct tm *info;
  
  time(&rawtime);
  info = localtime(&rawtime);
 
  strftime(buffer, sizeof(buffer), logTimeFmt, info);
 }

 char* time = buffer;
 
 char* template = "[MDE] [%s] ERROR: %s IN \"%s\" ON LINE %i";
 
 int size = snprintf(NULL, 0, template, time, errStr, file, line) + 1;

 char* finalErrStr = malloc(size);
 
 if(finalErrStr == NULL) 
  errStr = "CANT LOG ACTUAL ERROR";
  
 snprintf(finalErrStr, size, template, time, errStr, file, line);

 return finalErrStr;
}


#define logErr(val) do {\
  char* errStr = logStr(val, __FILE__, __LINE__);\
  if(errStr != NULL && showLogErr) {\
   fprintf(stderr, "%s\n", errStr);\
   fflush(stderr);\
  }\
  free(errStr);\
} while(false)\
 

#define ignVal(var) do {\
 (void)(var);\
 if(showLogWarn) {\
 printf("[MDE] WARN UNUSED VAR: \"%s\" IN FILE \"%s\" ON LINE %i\n", #var, __FILE__,  __LINE__);\
 fflush(stdout);\
 }\
} while(false)\

#define mdeGen(TYPE, NAME, SHORT_NAME, SHORT_NAME_UP)\
typedef struct {\
 Error err;\
 size_t len;\
 TYPE* val;\
} NAME;\
\
static inline NAME SHORT_NAME() {\
 NAME res = { NO_ERRORS, 0, (TYPE*){0} };\
 return res;\
}\
\
static inline NAME SHORT_NAME##OfLen(size_t len) {\
 NAME res = SHORT_NAME();\
 TYPE* val = malloc(sizeof(TYPE) * len);\
 \
 if(val == NULL) {\
  res.err = FAILED_TO_ALLOCATE_MEMORY;\
  return res;\
 }\
 for(size_t i = 0; i < len; i++) {\
  val[i] = (TYPE){0};\
 }\
 res.len = len;\
 res.val = val;\
 \
 return res;\
}\
\
static inline void rm##SHORT_NAME_UP(NAME val) {\
 free(val.val);\
 val.err = NO_ERRORS;\
 val.len = 0;\
}\
\
static inline NAME SHORT_NAME##FromOfLen(TYPE* val, size_t len) {\
 NAME res = SHORT_NAME##OfLen(len);\
 if(hasErr(&res)) {\
  rm##SHORT_NAME_UP(res);\
  return res;\
 }\
 for(size_t i = 0; i < len; i++) {\
  res.val[i] = val[i];\
 }\
 return res;\
}\
\
static inline void SHORT_NAME##Set(NAME* arr, TYPE val, size_t i) {\
 if(i > arr->len) {\
  arr->err = INDEX_OUT_OF_BOUNDS;\
  return;\
 }\
 arr->val[i] = val;\
 return;\
}\
\
static inline TYPE SHORT_NAME##GetAt(NAME arr, size_t i) {\
 return arr.val[i];\
}\
\
static inline void SHORT_NAME##Resize(NAME* arr, size_t len) {\
 if(len < arr->len){\
  arr->err = POTENTIAL_DATA_LOSS;\
  return;\
 }\
 arr->val = realloc(arr->val, sizeof(TYPE) * len);\
}\
\
static inline void SHORT_NAME##Copy(NAME* arr1, NAME arr2) {\
 SHORT_NAME##Resize(arr1, arr2.len);\
 if(hasErr(arr1)) return;\
 for(size_t i = 0; i < arr2.len; i++) {\
  arr1->val[i] = arr2.val[i];\
 }\
 arr1->len = arr2.len;\
}\
\

#define mdeGenExtraPro(TYPE, NAME, SHORT_NAME, SHORT_NAME_UP, FORMAT_TEMPLATE, FORMAT_FUNC, LEN_FUNC)\
static inline char* format##SHORT_NAME_UP(NAME val) {\
 char* res = NULL;\
 char* def = "\"(null)\"";\
 size_t defSize = strlen(def) + 1;\
 if(val.len != 0) {\
  res = FORMAT_FUNC(val.val[0], val.len, 0);\
  size_t resLen = strlen(res);\
  \
  for(size_t i = 1; i < val.len; i++) {\
   char* toAdd = NULL;\
   toAdd = FORMAT_FUNC(val.val[i], val.len, i);\
   size_t toAddLen = strlen(toAdd);\
   size_t newresLen = toAddLen + resLen;\
   \
   char* newres = realloc(res, newresLen + 1);\
   if(newres == NULL) {\
    free(toAdd);\
    break;\
   }\
   \
   res = newres;\
   res = memcpy(res+resLen, toAdd, toAddLen) - resLen;\
   resLen = newresLen;\
   \
   free(toAdd);\
  }\
  res[resLen] = '\0';\
 } else {\
  char* def = "\"(null)\"";\
  res = malloc(defSize);\
  res = strcpy(res, def);\
 }\
 \
 return res;\
}\
\
static inline char* format##SHORT_NAME_UP##Info(NAME val) {\
 char* res = NULL;\
 char* valStr = format##SHORT_NAME_UP(val);\
 size_t size = snprintf(NULL, 0, FORMAT_TEMPLATE) + 1;\
 res = malloc(size);\
 snprintf(res, size, FORMAT_TEMPLATE);\
 free(valStr);\
 return res;\
}\
\
static inline size_t fprint##SHORT_NAME_UP##Info(FILE* stream, NAME val) {\
 char* str = format##SHORT_NAME_UP##Info(val);\
 size_t res = fprintf(stream, "%s", str);\
 free(str);\
 return res;\
}\
\
static inline size_t fprint##SHORT_NAME_UP##InfoLn(FILE* stream, NAME val) {\
 fprint##SHORT_NAME_UP##Info(stream, val);\
 return fprintf(stream, "\n");\
}\
\
static inline size_t print##SHORT_NAME_UP##Info(NAME val) {\
 return fprint##SHORT_NAME_UP##Info(stdout, val);\
}\
\
static inline size_t print##SHORT_NAME_UP##InfoLn(NAME val) {\
 print##SHORT_NAME_UP##Info(val);\
 return fprintf(stdout, "\n");\
}\
\
static inline size_t fprint##SHORT_NAME_UP(FILE* stream, NAME val) {\
 char* str = format##SHORT_NAME_UP(val);\
 size_t res = fprintf(stream, "%s", str);\
 free(str);\
 return res;\
}\
\
static inline size_t fprint##SHORT_NAME_UP##Ln(FILE* stream, NAME val) {\
 fprint##SHORT_NAME_UP(stream, val);\
 return fprintf(stream, "\n");\
}\
\
static inline size_t print##SHORT_NAME_UP(NAME val) {\
 return fprint##SHORT_NAME_UP(stdout, val);\
}\
\
static inline size_t print##SHORT_NAME_UP##Ln(NAME val) {\
 print##SHORT_NAME_UP(val);\
 return fprintf(stdout, "\n");\
}\
static inline NAME SHORT_NAME##From(TYPE* val) {\
 return SHORT_NAME##FromOfLen(val, LEN_FUNC(val));\
}\
\

#define JSON_FORMAT(...) "{\n"\
                          "\"type\": \"%s\",\n"\
                          "\"err\":  \"%s\",\n"\
                          "\"len\":  %lu,\n"\
                          "\"val\":  %s\n}", __VA_ARGS__

#define mdeGenExtra(TYPE, NAME, SHORT_NAME, SHORT_NAME_UP, FORMAT_FUNC, LEN_FUNC)\
 mdeGenExtraPro(TYPE, NAME, SHORT_NAME, SHORT_NAME_UP, JSON_FORMAT(#NAME, formatErr(val.err), val.len, valStr), FORMAT_FUNC, LEN_FUNC)

mdeGen(char, String, str, Str);

static inline char* _formatStr(char val, size_t len, size_t i) {
 size_t resLen = 1;
 bool isFirst = i == 0;
 bool isLast = (len - 1) == i;
 
 if(isFirst || isLast) resLen += 1;
  
 size_t resSize = resLen + 1;

 char* res = malloc(resSize);
 if(isFirst) {
  res[0] = '"';
  res[1] = val;
 }else if (isLast) {
    res[0] = val;
    res[1] = '"';
 } else res[0] = val;
 
 res[resLen] = '\0';
 
 return res;
}

static inline size_t _lenStr(char* val) {
 return strlen(val);
}

mdeGenExtra(char, String, str, Str, _formatStr, _lenStr)

/* MDE */
#endif 
