#include <stdarg.h>

int sum(int n, ...);

int sum(int n, ...) {
    va_list ap;
    va_start(ap, n);
    int total = 0;
    int i = 0;
    while (i < n) {
        total += va_arg(ap, int);
        i++;
    }
    va_end(ap);
    return total;
}

int via(int (*fn)(int, ...)) {
    return fn(2, 4, 5) - 9;
}

int main() {
    return sum(3, 10, 20, 12) - 42 + via(sum);
}
