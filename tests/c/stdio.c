#include <stdio.h>

int main() {
    char buffer[32];
    int n = sprintf(buffer, "value=%d", 42);
    puts(buffer);
    printf("count=%d\n", n);
    return n != 8;
}
