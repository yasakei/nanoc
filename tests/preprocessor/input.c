// Preprocessor regression input.
#include "include.h"

#define EMPTY
#define ANSWER 42
#define DOUBLE(x) ((x) + (x))
#define STR(x) #x
#define CAT(a, b) a ## b
#define F(a, b) a + b
#define REP 3
#define REC(a) REP + a
#define LOOP(x) LOOP(x)
#define VA(...) __VA_ARGS__
#define VA2(a, ...) a, __VA_ARGS__
#define GLUE(x) x ## 1

int answer = ANSWER;
int twice = DOUBLE(5 + 1);
char *s = STR(hello world);
int glued = CAT(foo, bar);
int glue2 = GLUE(foo);
int sum = F(1, 2);
int rec = REC(7);
int lv = LOOP(1);
int va1 = VA(1, 2, 3);
int va2 = VA2(1, 2, 3);
int line_no = __LINE__;

#if ANSWER == 42
int yes = 1;
#elif ANSWER == 43
int no = 1;
#else
int also_no = 1;
#endif

#ifdef EMPTY
int defd = 1;
#endif

#ifndef MISSING
int not_missing = 1;
#endif

#if defined(MISSING) || (ANSWER > 40 && ANSWER < 44)
int logic = 1;
#endif

#if 0
int dead = ANSWER;
#ifdef FOO
int nested_dead = 1;
#endif
#elif 1
int alive = 1;
#if defined(EMPTY)
int nested_alive = 1;
#endif
#else
int dead2 = 1;
#endif

#undef ANSWER
#ifdef ANSWER
int should_not_appear = 1;
#endif

#if 1 ? 2 : 0
int ternary = 1;
#endif

#if (1 << 4) == 16 && (7 % 3) == 1 && (10 / 2) == 5
int arith = 1;
#endif
