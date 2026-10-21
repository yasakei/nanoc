extern int got_value;
extern int got_bss[8];
extern int got_answer(void);
extern int puts(const char *);

static int local_counter;
static int local_identity(void) { return ++local_counter; }

int got_host(void) {
    got_value += 1;
    got_bss[7] = got_answer();
    return got_value + got_bss[7];
}
int *got_value_address(void) { return &got_value; }
int *got_bss_address(void) { return got_bss; }
int (*got_answer_address(void))(void) { return got_answer; }
int (*got_puts_address(void))(const char *) { return puts; }
int (*got_local_address(void))(void) { return local_identity; }
int got_print(void) { return puts("GOT calls work"); }
