#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// define mde_RECOMMENDED if u want structs and functions for all the basic C val types
#define mde_RECOMMENDED
#include "mde.h"

int main(void) {

 mde_Char* b = mde_newChar();

 if(mde_isCharSafe(b)) {
  b->val = 'A';
  printf("test: %c\n", b->val);
 }

 b = mde_deleteChar(b);

 // Strings! 
 mde_CharArr* ch = mde_newCharArr(10);
 
 if(mde_isCharArrSafe(ch))
  for(int i = 0; i < ch->len; i++) {
   ch->val[i] = 'B';
  
   printf("%c", ch->val[i]);
  }
  
 printf("\n");
 
 ch = mde_deleteCharArr(ch);
 ch = mde_newCharArr(20);
  
 
 ch = mde_setCharAtIndex(ch, '6', 3);
 ch = mde_setCharAtIndex(ch, '9', 4);
 
 mde_CharArr* tmpCh = mde_resizeCharArr(ch, 20);
 
 if(!mde_isCharArrSafe(tmpCh)){
  ch = mde_deleteCharArr(ch);
  tmpCh = mde_deleteCharArr(tmpCh);
  return 1;
 }
 
 ch = mde_deleteCharArr(ch);
 ch = tmpCh;
 
 printf("%c%c nice\n",ch->val[3],ch->val[4]);
 
 mde_Char* c = mde_getCharArrAtIndex(ch, 3);


 if(mde_isCharSafe(c)) {
  printf("we got index 3 with value: %c\n", c->val);
 }


 ch = mde_deleteCharArr(ch);
 c = mde_deleteChar(c);

 
 ch = mde_newCharArrFrom("nice 69", 7);
 for(int i = 0; i < ch->len; i++) printf("%c", ch->val[i]);
 
 
 printf("\n");
 ch = mde_deleteCharArr(ch);

 return 0;
}
