#include "tcc_aggregate_compat.h"
#include <stdarg.h>

struct CompatPair compat_host_pair(struct CompatPair value, long increment) {
    value.first += increment;
    value.second -= increment;
    return value;
}
struct CompatReals compat_host_reals(struct CompatReals value, double increment) {
    value.first += increment;
    value.second -= increment;
    return value;
}
struct CompatLarge compat_host_large(struct CompatLarge value, long increment) {
    value.first += increment;
    value.second += increment;
    value.third += increment;
    return value;
}
struct CompatBytes compat_host_bytes(struct CompatBytes value) {
    value.first++;
    value.second++;
    value.third++;
    return value;
}
union CompatWord compat_host_word(union CompatWord value) {
    value.integer++;
    return value;
}
long compat_host_exhaust(long a, long b, long c, long d, long e, struct CompatPair pair, long last) {
    return a + b + c + d + e + pair.first + pair.second + last;
}
long compat_host_varargs(int marker, ...) {
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
int compat_host_calls_local(void) {
    struct CompatPair pair = {17, 25};
    struct CompatPair changed = compat_local_pair(pair, 3);
    if (changed.first != 20 || changed.second != 22) { return 1; }
    struct CompatReals reals = {17.5, 24.5};
    struct CompatReals transformed = compat_local_reals(reals, 2.5);
    if (transformed.first != 20 || transformed.second != 22) { return 2; }
    struct CompatLarge large = {11, 13, 18};
    struct CompatLarge expanded = compat_local_large(large, 2);
    if (expanded.first != 13 || expanded.second != 15 || expanded.third != 20) { return 3; }
    struct CompatBytes bytes = {1, 2, 3};
    struct CompatBytes incremented = compat_local_bytes(bytes);
    if (incremented.first != 2 || incremented.second != 3 || incremented.third != 4) { return 4; }
    union CompatWord word = {.integer = 7};
    union CompatWord next = compat_local_word(word);
    if (next.integer != 8) { return 5; }
    if (compat_local_exhaust(1, 2, 3, 4, 5, pair, 6) != 63) { return 6; }
    if (compat_local_varargs(1, pair, reals, large, word) != 133) { return 7; }
    struct CompatPair (*indirect)(struct CompatPair, long) = compat_local_pair;
    changed = indirect(pair, 3);
    if (changed.first != 20 || changed.second != 22) { return 8; }
    return 0;
}
