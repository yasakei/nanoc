#include <stdarg.h>

int host_variadic(int n, ...);
int host_calls_variadic(void);
int host_check_list(va_list ap);

int sum(int n, ...) {
    va_list ap;
    va_start(ap, n);
    int total = 0;
    int i = 0;
    while (i < n) { total += va_arg(ap, int); i++; }
    va_end(ap);
    return total;
}
int mixed(int first, double named, ...) {
    va_list ap;
    va_list saved;
    va_start(ap, named);
    if (va_arg(ap, int) != 7) { return 1; }
    va_copy(saved, ap);
    double value = va_arg(ap, double);
    if (value != 3.25) { return 2; }
    if (va_arg(saved, double) != 3.25) { return 3; }
    if (va_arg(ap, int) != 9) { return 4; }
    if (va_arg(saved, int) != 9) { return 5; }
    va_end(saved);
    va_end(ap);
    return 0;
}
int stack_named(int a, int b, int c, int d, int e, int f, int g, ...) {
    va_list ap;
    va_start(ap, g);
    int value = va_arg(ap, int);
    va_end(ap);
    return value;
}
int float_overflow(int n, ...) {
    va_list ap;
    va_start(ap, n);
    int i = 0;
    double total = 0.0;
    while (i < n) {
        double value = va_arg(ap, double);
        total = total + value;
        i++;
    }
    va_end(ap);
    if (total != 55.0) { return 1; }
    return 0;
}
int list_to_host(int n, ...) {
    va_list ap;
    va_start(ap, n);
    int value = host_check_list(ap);
    va_end(ap);
    return value;
}
int forwarded_list(va_list ap) {
    double a = va_arg(ap, double);
    int b = va_arg(ap, int);
    double c = va_arg(ap, double);
    if (a != 1.5) { return 1; }
    if (b != 7) { return 2; }
    if (c != 2.5) { return 3; }
    return 0;
}
int forward(int n, ...) {
    va_list ap;
    va_start(ap, n);
    int value = forwarded_list(ap);
    va_end(ap);
    return value;
}
int via(int (*fn)(int, ...)) {
    return fn(3, 1.5, 7, 2.5);
}
int main(void) {
    if (sum(9, 1, 2, 3, 4, 5, 6, 7, 8, 9) != 45) { return 1; }
    if (mixed(1, 2.0, 7, 3.25, 9) != 0) { return 2; }
    if (stack_named(1, 2, 3, 4, 5, 6, 7, 42) != 42) { return 3; }
    if (float_overflow(10, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0) != 0) { return 4; }
    if (host_variadic(3, 1.5, 7, 2.5) != 0) { return 5; }
    if (via(host_variadic) != 0) { return 6; }
    if (list_to_host(3, 1.5, 7, 2.5) != 0) { return 7; }
    if (host_calls_variadic() != 0) { return 8; }
    if (forward(3, 1.5, 7, 2.5) != 0) { return 9; }
    return 0;
}
