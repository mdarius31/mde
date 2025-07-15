#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// define mde_RECOMMENDED if u want structs and functions for all the basic C val types
// #define mde_RECOMMENDED
#include "mde.h"
mde_gen(char, Char)

typedef struct {
 char* val;
} test;

mde_gen(test, Test)


bool callbackLoopTestArr(mde_TestArr* arr, test t, int i) {
 (void)(arr);
 (void)(i);
 printf("%s\n",t.val);
 return true;
}

int switching(void) {

 int size = 2;
 test t[size];
 t[0] = (test){"darius"};
 t[1] = (test){"darius2"};
 
 mde_TestArr* t1 =  mde_newTestArrFrom(t,size);

 mde_loopTestArr(t1, callbackLoopTestArr);

 if(mde_hasErr(t1)) {
  mde_log(t1);
  t1 = mde_rmTestArr(t1);
  return 1;
 }

 
 t1 = mde_switchTest(t1, 0, 1);

 
 if(mde_hasErr(t1)) {
  mde_log(t1);
  t1 = mde_rmTestArr(t1);
 
  return 1;
 }
 
 mde_loopTestArr(t1, callbackLoopTestArr);
 
 t1 = mde_rmTestArr(t1);
 
 return 0;
}



typedef void (*function)(int i); 
mde_gen(function, Fn)

bool callbackLoopCharArr(mde_CharArr* arr, char val, int i) {
 (void)(arr);
 (void)(i);
 printf("%c",val);
 return true;
}

int charStuff() {

 mde_Char* b = mde_newChar();

 if(mde_hasErr(b)) { 
  mde_log(b);
  return 1;
 }
 
 if(mde_isCharSafe(b)) {
  b->val = 'A';
  printf("test: %c\n", b->val);
 } 




 b = mde_rmChar(b);

 // Strings! 
 mde_CharArr* ch = mde_newCharArr(10);
 
 if(mde_isCharArrSafe(ch))
  for(int i = 0; i < ch->len; i++) {
   ch->val[i] = 'B';
  
   printf("%c", ch->val[i]);
  }
  
 printf("\n");
 
 ch = mde_rmCharArr(ch);
 ch = mde_newCharArr(20);
  
 
 ch = mde_setCharAt(ch, '6', 3);
 ch = mde_setCharAt(ch, '9', 4);
 
 mde_CharArr* tmpCh = mde_resizeCharArr(ch, 20);
 
 if(!mde_isCharArrSafe(tmpCh)){
  ch = mde_rmCharArr(ch);
  tmpCh = mde_rmCharArr(tmpCh);
  return 1;
 }
 
 ch = mde_rmCharArr(ch);
 ch = tmpCh;
 
 printf("%c%c nice\n",ch->val[3],ch->val[4]);
 
 mde_Char* c = mde_getCharAt(ch, 3);

 if(mde_isCharSafe(c)) {
  printf("we got index 3 with value: %c\n", c->val);
 }


 ch = mde_rmCharArr(ch);
 c = mde_rmChar(c);

 
 ch = mde_newCharArrFrom("nice 69", 7);
 for(int i = 0; i < ch->len; i++) printf("%c", ch->val[i]);

 ch = mde_CharArrAdd(ch, '\n');
 for(int i = 0; i < ch->len; i++) printf("%c", ch->val[i]);
 
 char* str = " ohh very nice\n";

 mde_CharArr* ch2 = mde_newCharArrFrom(str, strlen(str));
 mde_CharArr* t = mde_combineCharArr(ch, ch2);

 for(int i = 0; i < t->len; i++) printf("%c", t->val[i]);
 mde_loopCharArr(t, callbackLoopCharArr);
       
 t = mde_rmCharArr(t);


 
 ch2 = mde_rmCharArr(ch2);
 ch = mde_rmCharArr(ch);
 
 printf("\nis good? yes\n");

 return 0;
}

#define tryTest(test) result = test(); if(result != 0) { printf("ERROR\n"); return result; }

int main(void) {
 int result = 0;

 tryTest(charStuff);
 tryTest(switching); 
 
 return result;
}
