#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// define mdeBasic if u want structs and functions for all the basic C val types
// #define mdeBasic
#define mdeLogWarn false
#define mdeLogErr false

#include "mde.h"

int main(void) {
 int result = 0;

 
 String test = strFromOfLen("test", 4);
 printStr(test);
 rmStr(test);
 
 return result;
}
