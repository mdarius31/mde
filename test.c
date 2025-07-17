#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// define mdeRecommended if u want structs and functions for all the basic C val types
#define mdeRecommended
#include "mde.h"

typedef struct {
 char* val;
} test;

mdeGen(test, Test)


bool callbackLoopTestArr(mdeTestArr* arr, test t, int i) {
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
 
 mdeTestArr* t1 =  mdeNewTestArrFrom(t,size);

 mdeLoopTestArr(t1, callbackLoopTestArr);

 mdeLog(t1);
 if(mdeHasErr(t1)) {
  t1 = mdeRmTestArr(t1);
  return 1;
 }

 
 t1 = mdeSwitchTest(t1, 0, 1);

 
 if(mdeHasErr(t1)) {
  mdeLog(t1);
  t1 = mdeRmTestArr(t1);
 
  return 1;
 }
 
 mdeLoopTestArr(t1, callbackLoopTestArr);
 
 t1 = mdeRmTestArr(t1);
 
 return 0;
}



typedef void (*function)(int i); 
mdeGen(function, Fn)


bool callbackLoopCharArr(mdeCharArr* arr, char val, int i) {
 (void)(arr);
 (void)(i);
 printf("%c",val);
 return true;
}

int charStuff() {

 mdeChar* b = mdeNewChar();

 if(mdeHasErr(b)) { 
  mdeLog(b);
  return 1;
 }
 
 if(mdeIsCharSafe(b)) {
  b->val = 'A';
  printf("test: %c\n", b->val);
 } 




 b = mdeRmChar(b);

 // Strings! 
 mdeCharArr* ch = mdeNewCharArr(10);
 
 if(mdeIsCharArrSafe(ch))
  for(int i = 0; i < ch->len; i++) {
   ch->val[i] = 'B';
  
   printf("%c", ch->val[i]);
  }
  
 printf("\n");
 
 ch = mdeRmCharArr(ch);
 ch = mdeNewCharArr(20);
  
 
 ch = mdeSetCharAt(ch, '6', 3);
 ch = mdeSetCharAt(ch, '9', 4);
 
 mdeCharArr* tmpCh = mdeResizeCharArr(ch, 20);
 
 if(!mdeIsCharArrSafe(tmpCh)){
  ch = mdeRmCharArr(ch);
  tmpCh = mdeRmCharArr(tmpCh);
  return 1;
 }
 
 ch = mdeRmCharArr(ch);
 ch = tmpCh;
 
 printf("%c%c nice\n",ch->val[3],ch->val[4]);
 
 mdeChar* c = mdeGetCharAt(ch, 3);

 if(mdeIsCharSafe(c)) {
  printf("we got index 3 with value: %c\n", c->val);
 }


 ch = mdeRmCharArr(ch);
 c = mdeRmChar(c);

 
 ch = mdeNewCharArrFrom("nice 69", 7);
 for(int i = 0; i < ch->len; i++) printf("%c", ch->val[i]);

 ch = mdeCharArrAdd(ch, '\n');
 for(int i = 0; i < ch->len; i++) printf("%c", ch->val[i]);
 
 char* str = " ohh very nice\n";

 mdeCharArr* ch2 = mdeNewCharArrFrom(str, strlen(str));
 mdeCharArr* t = mdeCombineCharArr(ch, ch2);

 for(int i = 0; i < t->len; i++) printf("%c", t->val[i]);
 mdeLoopCharArr(t, callbackLoopCharArr);
       
 t = mdeRmCharArr(t);


 
 ch2 = mdeRmCharArr(ch2);
 ch = mdeRmCharArr(ch);
 
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
