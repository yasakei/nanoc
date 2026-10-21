struct State { long count; double scale; };
int calls;
int shadowed = 99;
int static_shadow(void) {
    static int shadowed = 17;
    static int *pointer = &shadowed;
    return (*pointer)++;
}
int next(void) { static int value = 17; return value++; }
long *persistent_address(void) { static long value = 1099511627776L; return &value; }
long recurse(int n) { static long count; count++; if (n) { recurse(n - 1); } return count; }
int scopes(void) {
    static int value = 17;
    { static int value = 25; value++; if (value < 26) { return 1; } }
    value++;
    return value;
}
int aggregate_state(void) {
    static struct State state = {.count = 17, .scale = 2.5};
    state.count++;
    state.scale += 0.5;
    if (state.count == 18 && state.scale == 3.0) { return 0; }
    if (state.count == 19 && state.scale == 3.5) { return 0; }
    return 1;
}
int static_arrays(void) {
    static char text[] = "abc";
    static int values[3] = {17, 25};
    static int *pointer = values;
    pointer[2]++;
    text[0]++;
    return pointer[0] + pointer[1] + pointer[2] + text[0] - 97;
}
int static_function(void) {
    static int (*pointer)(void) = next;
    return pointer();
}
int main(void) {
    if (next() != 17 || next() != 18) { return 1; }
    if (static_function() != 19) { return 2; }
    long *first = persistent_address(), *second = persistent_address();
    if (first != second || *first != 1099511627776L) { return 3; }
    *first += 42;
    if (*persistent_address() != 1099511627818L) { return 4; }
    if (recurse(2) != 3 || recurse(0) != 4) { return 5; }
    if (scopes() != 18 || scopes() != 19) { return 6; }
    if (aggregate_state() != 0 || aggregate_state() != 0) { return 7; }
    if (static_arrays() != 44 || static_arrays() != 46) { return 8; }
    if (static_shadow() != 17 || static_shadow() != 18 || shadowed != 99) { return 9; }
    return 0;
}
