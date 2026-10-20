#include <float.h>
#include <stdalign.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdnoreturn.h>
#include <tgmath.h>

int main() {
    return (int)(FLT_RADIX != 2 || sizeof(size_t) < 4);
}
