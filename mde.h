#ifndef MDE
#define MDE
// CONFIG

#ifndef mdeLogWarn
 #define mdeLogWarn true
#endif

#ifndef mdeLogErr
 #define mdeLogErr true
#endif

#ifndef mdeLogTimeFmt
 #define mdeLogTimeFmt "%d-%m-%Y %H:%M:%S"
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

char* formatErr(Error err) {
 if(err == POTENTIAL_DATA_LOSS)         return "POTENTIAL DATA LOSS";
 if(err == FAILED_TO_REALLOCATE_MEMORY) return "FAILED TO REALLOCATE MEMORY";
 if(err == FAILED_TO_ALLOCATE_MEMORY)   return "FAILED TO ALLOCATE MEMORY";
 if(err == INDEX_OUT_OF_BOUNDS)         return "INDEX OUT OF BOUNDS";
 if(err == NULL_VALUE)                  return "NULL VALUE";
 if(err == NO_ERRORS)                   return "NO ERRORS";
 return "NO FORMAT FOR THIS ERROR";
}

bool hasErr(void* val) {
 Error err = *(Error*)val;
 return err != NO_ERRORS;
}

#define mdeIgn(var) do {\
 (void)(var);\
 if(mdeLogWarn) {\
 printf("[MDE] WARN UNUSED VAR: \"%s\" IN FILE \"%s\" ON LINE %i\n", #var, __FILE__,  __LINE__);\
 fflush(stdout);\
 }\
} while(false)\

#define mdeGen(TYPE, NAME, SHORT_NAME, SHORT_NAME_UP)\
typedef struct {\
 Error err;\
 unsigned int len;\
 TYPE* val;\
} NAME;\
\
NAME SHORT_NAME() {\
 NAME res = { NO_ERRORS, 0, NULL };\
 return res;\
}\
\
NAME SHORT_NAME##OfLen(unsigned int len) {\
 NAME res = SHORT_NAME();\
 TYPE* val = malloc(sizeof(TYPE) * len);\
 \
 if(val == NULL) {\
  res.err = FAILED_TO_ALLOCATE_MEMORY;\
  return res;\
 }\
 \
 res.len = len;\
 res.val = val;\
 \
 return res;\
}\
\
void rm##SHORT_NAME_UP(NAME val) {\
 free(val.val);\
 val.err = NO_ERRORS;\
 val.len = 0;\
}\
\
NAME SHORT_NAME##FromOfLen(TYPE* val, unsigned int len) {\
 NAME res = SHORT_NAME##OfLen(len);\
 if(hasErr(&res)) {\
  rm##SHORT_NAME_UP(res);\
  return res;\
 }\
 for(unsigned int i = 0; i < len; i++) {\
  res.val[i] = val[i];\
 }\
 return res;\
}\
\

#define mdeGenExtra(TYPE, NAME, SHORT_NAME, SHORT_NAME_UP, FORMAT_FUNC)\
char* format##SHORT_NAME_UP(NAME val) {\
 char* res = NULL;\
 char* template = "%s {\n"\
                " err: \"%s\"\n"\
                " len: %u\n"\
                " %s*: %s\n"\
                "}";\
 if(val.len == 0) {\
  int size = snprintf(NULL, 0, template, #NAME, formatErr(val.err), val.len, #TYPE, "") + 1;\
  res = malloc(size);\
  snprintf(res, size, template, #NAME, formatErr(val.err), val.len, #TYPE, "");\
  return res;\
 }\
 \
 char* valStr = FORMAT_FUNC(val.val[0]);\
 unsigned int valStrLen = strlen(valStr);\
 \
 for(unsigned int i = 1; i < val.len; i++) {\
  char* toAdd = FORMAT_FUNC(val.val[i]);\
  unsigned int toAddLen = strlen(toAdd);\
  unsigned int newValStrLen = toAddLen + valStrLen;\
  \
  char* newValStr = realloc(valStr, newValStrLen + 1);\
  if(newValStr == NULL) {\
   free(toAdd);\
   break;\
  }\
  \
  valStr = newValStr;\
  valStr = memcpy(valStr+valStrLen, toAdd, toAddLen);\
  valStrLen = newValStrLen;\
  \
  free(toAdd);\
 }\
 valStr[valStrLen] = '\0';\
 int size = snprintf(NULL, 0, template, #NAME, formatErr(val.err), val.len, #TYPE, valStr) + 1;\
 res = malloc(size);\
 snprintf(res, size, template, #NAME, formatErr(val.err), val.len, #TYPE, valStr);\
 free(valStr);\
 return res;\
}\
\
int fprint##SHORT_NAME_UP(FILE* stream, NAME val) {\
 char* str = format##SHORT_NAME_UP(val);\
 int res = fprintf(stream, "%s\n", str);\
 free(str);\
 return res;\
}\
\
int print##SHORT_NAME_UP(NAME val) {\
 return fprint##SHORT_NAME_UP(stdout, val);\
}\


mdeGen(char, String, str, Str);

char* _formatStr(char val) {
 int len = 1;
 int size = len + 1;

 char* res = malloc(size);
 
 res[0] = val;
 res[1] = '\0';
 
 return res;
}

mdeGenExtra(char, String, str, Str, _formatStr)

// int fprintStr(FILE* stream, String str) {
//  int res = 0;
//  for(int i = 0; i < str.len; i++) {
//   res = fprintf(stream, "%c", str.val[i]);
//   if(res < 0) break;
//  }
//  return res;
// }
// 
// int printStr(String str) {
//  return fprintStr(stdout, str);
// }

/* MDE */
#endif 
