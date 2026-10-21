#include <stdarg.h>
int host_check_list(va_list ap) {
    double a = va_arg(ap, double);
    int b = va_arg(ap, int);
    double c = va_arg(ap, double);
    return !(a == 1.5 && b == 7 && c == 2.5);
}
int host_variadic(int n, ...) {
    if (n != 3) { return 1; }
    va_list ap;
    va_start(ap, n);
    int value = host_check_list(ap);
    va_end(ap);
    return value;
}

extern int sum(int n, ...);
extern int mixed(int first, double named, ...);
extern int stack_named(int, int, int, int, int, int, int, ...);
extern int float_overflow(int n, ...);
extern int forwarded_list(va_list ap);
static int host_forward(int n, ...) {
    if (n != 3) { return 1; }
    va_list ap;
    va_start(ap, n);
    int value = forwarded_list(ap);
    va_end(ap);
    return value;
}
int host_calls_variadic(void) {
    if (sum(9, 1, 2, 3, 4, 5, 6, 7, 8, 9) != 45) { return 1; }
    if (mixed(1, 2.0, 7, 3.25, 9) != 0) { return 2; }
    if (stack_named(1, 2, 3, 4, 5, 6, 7, 42) != 42) { return 3; }
    if (float_overflow(10, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0) != 0) { return 4; }
    if (host_forward(3, 1.5, 7, 2.5) != 0) { return 5; }
    return 0;
}
