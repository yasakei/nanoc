#include <stdarg.h>
#include <float.h>

long double global_value = 1.0000000000000000001L;
long double local_identity(long double value) { return value; }
long double local_add(long double a, long double b) { return a + b; }
long double local_variadic(int count, ...) {
    va_list ap; va_start(ap, count);
    long double total = 0.0L;
    for (int i = 0; i < count; i++) { total += va_arg(ap, long double); }
    va_end(ap); return total;
}
int main(void) {
    if (sizeof(long double) != 16 || _Alignof(long double) != 16) { return 1; }
    long double one = 1.0L, precise = 1.0000000000000000001L;
    if (!(precise > one) || precise != global_value) { return 2; }
    if ((double)precise != 1.0) { return 3; }
    long double difference = precise - one;
    if (!(difference > 0.0L) || !(difference < 0.000000000000000001L)) { return 4; }
    long double value = 2.5L;
    if (value + 2 != 4.5L || 2 + value != 4.5L || 7 - value != 4.5L) { return 5; }
    if (value * 3 != 7.5L || 10 / value != 4.0L || value / 2 != 1.25L) { return 6; }
    if (-value != -2.5L || !value || (0.0L && 1)) { return 7; }
    if ((long)value != 2 || (long)-value != -2) { return 8; }
    unsigned long top = 0x8000000000000000UL;
    if ((long double)top != 9223372036854775808.0L || (unsigned long)9223372036854775808.0L != top) { return 9; }
    if ((long double)4294967295U != 4294967295.0L) { return 10; }
    value += 1.5L;
    if (value != 4.0L || value++ != 4.0L || ++value != 6.0L || value-- != 6.0L || --value != 4.0L) { return 11; }
    long double *p = &value; *p = precise;
    if (*p != precise || local_identity(precise) != precise || local_add(one, difference) != precise) { return 12; }
    if (local_variadic(3, 1.0L, 2.5L, 3.25L) != 6.75L) { return 13; }
    long double values[2] = {one, precise};
    if (values[1] != precise || sizeof(values) != 32) { return 14; }
    long double (*indirect)(long double) = local_identity;
    if (indirect(precise) != precise) { return 15; }
    long double big = 1e4000L, small = 1e-4000L;
    if (!(big > 1e300L) || !(small > 0.0L) || !(small < 1e-300L)) { return 16; }
    long double zero = 0.0L, nan = zero / zero;
    if (nan == nan || !(nan != nan) || nan <= zero || nan >= zero || !nan) { return 17; }
    if ((1 ? precise : 1.0) != precise || (0 ? precise : 2.0) != 2.0L) { return 18; }
    for (int i = 0; i < 100; i++) { value = local_identity(precise); if (value != precise) { return 19; } }
    return 0;
}
