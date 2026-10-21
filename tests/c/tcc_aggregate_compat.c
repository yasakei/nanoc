#include "../host/tcc_aggregate_compat.h"
#include <stdarg.h>

struct CompatPair compat_local_pair(struct CompatPair value, long increment) {
    value.first += increment;
    value.second -= increment;
    return value;
}
struct CompatReals compat_local_reals(struct CompatReals value, double increment) {
    value.first += increment;
    value.second -= increment;
    return value;
}
struct CompatLarge compat_local_large(struct CompatLarge value, long increment) {
    value.first += increment;
    value.second += increment;
    value.third += increment;
    return value;
}
struct CompatBytes compat_local_bytes(struct CompatBytes value) {
    value.first++;
    value.second++;
    value.third++;
    return value;
}
union CompatWord compat_local_word(union CompatWord value) {
    value.integer++;
    return value;
}
long compat_local_exhaust(long a, long b, long c, long d, long e, struct CompatPair pair, long last) {
    return a + b + c + d + e + pair.first + pair.second + last;
}
long compat_local_varargs(int marker, ...) {
    va_list ap;
    va_start(ap, marker);
    struct CompatPair pair = va_arg(ap, struct CompatPair);
    struct CompatReals reals = va_arg(ap, struct CompatReals);
    struct CompatLarge large = va_arg(ap, struct CompatLarge);
    union CompatWord word = va_arg(ap, union CompatWord);
    va_end(ap);
    return marker + pair.first + pair.second + (long)reals.first + (long)reals.second
        + large.first + large.second + large.third + word.integer;
}
int main(void) {
    struct CompatPair pair = {17, 25};
    struct CompatPair changed = compat_host_pair(pair, 3);
    if (changed.first != 20 || changed.second != 22) { return 1; }
    changed = compat_local_pair(pair, 3);
    if (changed.first != 20 || changed.second != 22) { return 2; }
    changed = compat_local_pair(compat_host_pair(pair, 3), 2);
    if (changed.first != 22 || changed.second != 20 || pair.first != 17) { return 3; }
    struct CompatReals reals = {17.5, 24.5};
    struct CompatReals transformed = compat_host_reals(reals, 2.5);
    if (transformed.first != 20 || transformed.second != 22) { return 4; }
    transformed = compat_local_reals(reals, 2.5);
    if (transformed.first != 20 || transformed.second != 22) { return 5; }
    struct CompatLarge large = {11, 13, 18};
    struct CompatLarge expanded = compat_host_large(large, 2);
    if (expanded.first != 13 || expanded.second != 15 || expanded.third != 20) { return 6; }
    expanded = compat_local_large(compat_host_large(large, 2), 3);
    if (expanded.first != 16 || expanded.second != 18 || expanded.third != 23 || large.first != 11) { return 7; }
    struct CompatBytes bytes = {1, 2, 3};
    struct CompatBytes incremented = compat_host_bytes(bytes);
    if (incremented.first != 2 || incremented.second != 3 || incremented.third != 4) { return 8; }
    incremented = compat_local_bytes(bytes);
    if (incremented.first != 2 || incremented.second != 3 || incremented.third != 4) { return 9; }
    union CompatWord word = {.integer = 7};
    union CompatWord next = compat_host_word(word);
    if (next.integer != 8) { return 10; }
    next = compat_local_word(word);
    if (next.integer != 8) { return 11; }
    if (compat_host_exhaust(1, 2, 3, 4, 5, pair, 6) != 63) { return 12; }
    if (compat_local_exhaust(1, 2, 3, 4, 5, pair, 6) != 63) { return 13; }
    if (compat_host_varargs(1, pair, reals, large, word) != 133) { return 14; }
    if (compat_local_varargs(1, pair, reals, large, word) != 133) { return 15; }
    struct CompatPair (*indirect)(struct CompatPair, long) = compat_host_pair;
    changed = indirect(pair, 3);
    if (changed.first != 20 || changed.second != 22) { return 16; }
    if (compat_host_calls_local() != 0) { return 17; }
    return 0;
}
