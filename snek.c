#define showLogErr false
#define showLogWarn false

#include "mde.h"
#include <pthread.h>
#include <unistd.h>
#include <termios.h>



typedef enum {
 NORTH,
 SOUTH,
 EAST,
 WEST
} facing;


struct snek {
 int x;
 int y;
};


mdeGen(struct snek*, Snek, snek, Snek)
char* _formatSnek(struct snek* val, size_t len, size_t i) {
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

 char* template = "%s%s{ \"x\": \"%i\", \"y\": %i }%s";
 size_t size = snprintf(NULL, 0, template, prefix, infix, val->x, val->y, postfix) + 1;
 char* res = malloc(size);
 snprintf(res, size, template, prefix, infix, val->x, val->y, postfix);
 return res;
 
}

size_t _lenSnek(struct snek** s) {
  int res = 0;
  for(int i = 0; s[i] != NULL; i++) res++;
  return res;
}

mdeGenExtra(struct snek*, Snek, snek, Snek, _formatSnek, _lenSnek)

mdeGen(String*, Arena, arena, Arena)


char* _formatArena(String* val, size_t len, size_t i) {
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

 char* template = "%s%s{ \"row\": \"%s\" }%s";
 size_t size = snprintf(NULL, 0, template, prefix, infix, val->val, postfix) + 1;
 char* res = malloc(size);
 snprintf(res, size, template, prefix, infix, val->val, postfix);
 return res; 
}


size_t _lenArena(String** s) {
  int res = 0;
  for(int i = 0; s[i] != NULL; i++) res++;
  return res;
}

mdeGenExtra(String*, Arena, arena, Arena, _formatArena, _lenArena)

typedef struct {
 char input[1];
 bool shouldClose;
 facing dir;
 Snek snek;
 Arena arena;
} State;
 


void *takeInput(void *ptr) {
 State* state = (State *)ptr;

 struct termios oldTio, newTio;
 tcgetattr(STDIN_FILENO, &oldTio);
 newTio = oldTio;
 newTio.c_lflag &= (~ICANON & ~ECHO);
 
 tcsetattr(STDIN_FILENO, TCSANOW, &newTio);

 
 ignVal(ptr);

 while(!state->shouldClose) { 
  read(0, state->input, 1);
  if(state->input[0] == 'q') state->shouldClose = true;
 } 
 
 tcsetattr(STDIN_FILENO, TCSANOW, &oldTio);
 
 return NULL;
}

Arena initArena(State* state, Arena layout) {
 ignVal(state);
 ignVal(layout);
 // state
 return state->arena;
}

int game(State* state) { 
 printSnekInfoLn(state->snek);
 printArenaInfoLn(state->arena);
 return 0;
}

#define tryToRun(func) if((result = func(&state)) != 0) { pthread_join(inputThread, NULL); return result; }

int main(void) {
 int result = 0;
 
 State state = (State){"", false, EAST, snekOfLen(1), arena()};

 if(hasErr(&state.snek)) {
  logErr(&state.snek);
  return -1;
 }
 
 pthread_t inputThread;
 int inputThreadRet;
 inputThreadRet = pthread_create(&inputThread, NULL, takeInput, (void *) &state);
 
 ignVal(inputThreadRet);

 tryToRun(game);


 pthread_join(inputThread, NULL);

 rmSnek(state.snek);
 
 return result;
}
