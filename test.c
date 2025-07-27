#define showLogWarn true
#define showLogErr true
#include "mde.h"


typedef struct Enemy{
 char* name;
 int health;
} Enemy;

mdeGen(Enemy*, Enemies, enems, Enems);

static inline char* _formatEnemy(Enemy* val, size_t len, size_t i) {
 
 bool isFirst = i == 0;
 bool isLast = (len - 1) == i;

 char* prefix = "[ ";
 char* infix = ", ";
 char* postfix = " ]";
 
 if(isFirst && isLast) { 
  infix = "";
 } else
 if(isFirst) {
  infix = "";
  postfix = "";
 } else
 if(isLast) {
  prefix = ""; 
 } else {
  prefix = "";
  postfix = "";
 }

 if(val == NULL) {
  char* nullTemplate = "%s%s\"(null)\"%s";
  size_t size = snprintf(NULL, 0, nullTemplate, prefix, infix, postfix) + 1;
  char* res = malloc(size);
  snprintf(res, size, nullTemplate, prefix, infix, postfix);
  return res;
 }
 
 char* template = "%s%s{ \"name\": \"%s\", \"health\": %i }%s";
 size_t size = snprintf(NULL, 0, template, prefix, infix, val->name, val->health, postfix) + 1;
 char* res = malloc(size);
 snprintf(res, size, template, prefix, infix, val->name, val->health, postfix);
 return res;
}

static inline size_t _lenEnemies(Enemy** val) {
 size_t res = 0;

 for(size_t i = 0; val[i] != NULL; i++) res++;
 
 return res;
}

mdeGenExtra(Enemy*, Enemies, enems, Enems, _formatEnemy, _lenEnemies)

int main(void) {
 FILE *fptr;
 fptr = fopen("test.json", "w"); 
 
 String t = strFrom("");
 printStrInfoLn(t);
 rmStr(t);
 
 String test = strFrom("test");
 logErr(&test);
 if(hasErr(&test)) {
  rmStr(test);
  return -1;
 }
 
 printStrInfoLn(test);
 rmStr(test);
 
 Enemies enemies = enemsOfLen(10);
 for(size_t i = 0; i < enemies.len; i++) {
   if(i % 2 == 0) enemsSet(&enemies, &(Enemy){"Marvin", 100}, i);
   else enemsSet(&enemies, &(Enemy){"John", 100}, i);
 }
 fprintEnemsInfoLn(fptr, enemies);
 rmEnems(enemies);


 Enemies nullEnemies = enemsOfLen(12);
 
 

 
 
 rmEnems(nullEnemies);

 
 
 Enemies enemies2 = enemsFrom((Enemy*[]){ &(Enemy){"John", 100}, &(Enemy){"test here asd s", 100}, NULL });
 printEnemsInfoLn(enemies2);

 fclose(fptr);
 
 rmEnems(enemies2);


 return 0;
}
