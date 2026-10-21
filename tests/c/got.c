int got_value = 1;
int got_bss[8];
int got_answer(void) { return 40; }
int puts(const char *);
typedef int (*Answer)(void);
typedef int (*Printer)(const char *);
int got_host(void);
int *got_value_address(void);
int *got_bss_address(void);
Answer got_answer_address(void);
Printer got_puts_address(void);
Answer got_local_address(void);
int got_print(void);

// A non-GOT absolute relocation to the same imported function.
Printer direct_puts = puts;
int main(void) {
    if (got_host() != 42 || got_value != 2 || got_bss[7] != 40) { return 1; }
    if (got_value_address() != &got_value) { return 2; }
    if (got_bss_address() != got_bss) { return 3; }
    if (got_answer_address() != got_answer) { return 4; }
    Answer answer = got_answer_address();
    if (answer() != 40) { return 5; }
    Printer printer = got_puts_address();
    if (printer != direct_puts || printer != puts) { return 6; }
    if (printer("GOT pointer works") < 0 || got_print() < 0) { return 7; }
    Answer local = got_local_address();
    if (local() != 1 || local() != 2) { return 8; }
    return 0;
}
