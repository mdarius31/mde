#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// define mde_RECOMMENDED if u want structs and functions for all the basic C data types
#define mde_RECOMMENDED
#include "mde.h"


// created by the preprocessor
mde_CREATE(mde_CharArr*, ArrOfStrings)
mde_CREATE(mde_ArrOfStrings*, ArrOfArrOfStrings)


mde_CREATE(char, String)
mde_CREATE(char*, ArrOfStrings2)

struct test {
 int a;
};

mde_CREATE(struct test, testArr);

int main(void) {

// Strings! 
mde_CharArr* ch = mde_createCharArr(10);
 
if(!mde_isCharArrSafe(ch)) {
 mde_deleteCharArr(ch);
 return 1;
}
printf("yeay it works!\n");
 

ch->data[3] = '6';
ch->data[4] = '9';

mde_CharArr* tmpCh = mde_resizeCharArr(ch, 20);

if(!mde_isCharArrSafe(tmpCh)){
 mde_deleteCharArr(ch);
 mde_deleteCharArr(tmpCh);
 return 1;
}

mde_deleteCharArr(ch);
ch = tmpCh;

printf("%c%c nice\n",ch->data[3],ch->data[4]);

mde_deleteCharArr(ch);



// Array of Strings!
mde_ArrOfStrings* arr = mde_createArrOfStrings(99);

if(!mde_isArrOfStringsSafe(arr)) {
 return 1;
}

for(int i = 0; i < arr->len; i++) {
 arr->data[i] = mde_createCharArr(100);
 mde_deleteCharArr(arr->data[i]);
}

mde_deleteArrOfStrings(arr);

// Array of Arrays of Strings!  

mde_ArrOfArrOfStrings* arrOfArr = mde_createArrOfArrOfStrings(99);

for(int i = 0; i < arrOfArr->len; i++) {
 arrOfArr->data[i] = mde_createArrOfStrings(10);
 
 for(int j = 0; j < arrOfArr->data[i]->len; j++) {
   arrOfArr->data[i]->data[j] = mde_createCharArr(100);
   mde_deleteCharArr(arrOfArr->data[i]->data[j]);
 }

 mde_deleteArrOfStrings(arrOfArr->data[i]);   
}

mde_deleteArrOfArrOfStrings(arrOfArr);


 return 0;
}
