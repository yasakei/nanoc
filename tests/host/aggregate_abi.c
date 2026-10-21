#include "aggregate_abi.h"
#include <stdarg.h>
struct Pair host_pair(struct Pair value, long increment) { value.first += increment; value.second -= increment; return value; }
struct Reals host_reals(struct Reals value, double increment) { value.first += increment; value.second -= increment; return value; }
struct Mixed host_mixed_record(struct Mixed value, int increment) { value.integer += increment; value.real += increment; return value; }
struct Reverse host_reverse(struct Reverse value) { value.real += 1; value.integer += 2; return value; }
struct Large host_large(struct Large value, long increment) { value.first += increment; value.second += increment; value.third += increment; return value; }
struct Floats host_floats_record(struct Floats value) { value.first += 1; value.second += 2; value.third += 3; return value; }
struct Bytes host_bytes(struct Bytes value) { value.first++; value.second++; value.third++; return value; }
struct Single host_single(struct Single value) { value.real += 1.5f; value.integer += 2; return value; }
struct Tail host_tail(struct Tail value) { value.real += 1.5; value.first++; value.second++; value.third++; return value; }
struct Embedded host_embedded(struct Embedded value) { value.values[0] += 1; value.values[1] += 2; value.values[2] += 3; value.tag += 4; return value; }
struct Nested host_nested(struct Nested value) { value.inner.integer += 1; value.inner.real += 2; return value; }
union Word host_word(union Word value) { value.integer++; return value; }
long host_exhaust(long a, long b, long c, long d, long e, struct Pair pair, long last) { return a + b + c + d + e + pair.first + pair.second + last; }
long host_float_exhaust(double a, double b, double c, double d, double e, double f, double g, struct Reals pair, long last) { return (long)(a + b + c + d + e + f + g + pair.first + pair.second) + last; }
long host_named_varargs(struct Pair pair, int count, ...) {
    va_list ap; va_start(ap, count); long total = pair.first + pair.second;
    for (int i = 0; i < count; i++) { total += va_arg(ap, long); }
    va_end(ap); return total;
}
long host_large_varargs(struct Large value, int count, ...) {
    va_list ap; va_start(ap, count); long total = value.first + value.second + value.third;
    for (int i = 0; i < count; i++) { total += va_arg(ap, long); }
    va_end(ap); return total;
}
long host_aggregate_varargs(int marker, ...) {
    va_list ap; va_start(ap, marker);
    struct Pair pair = va_arg(ap, struct Pair);
    struct Mixed mixed = va_arg(ap, struct Mixed);
    struct Reals reals = va_arg(ap, struct Reals);
    struct Large large = va_arg(ap, struct Large);
    union Word word = va_arg(ap, union Word);
    va_end(ap);
    return marker + pair.first + pair.second + mixed.integer + (long)mixed.real
        + (long)reals.first + (long)reals.second + large.first + large.second + large.third + word.integer;
}
long host_aggregate_overflow(long a, long b, long c, long d, long e, ...) {
    va_list ap; va_start(ap, e);
    struct Pair pair = va_arg(ap, struct Pair);
    long last = va_arg(ap, long);
    struct Mixed mixed = va_arg(ap, struct Mixed);
    va_end(ap);
    return a + b + c + d + e + pair.first + pair.second + last + mixed.integer + (long)mixed.real;
}
struct Large host_variadic_return(int marker, ...) {
    va_list ap; va_start(ap, marker);
    struct Pair pair = va_arg(ap, struct Pair);
    struct Large value = va_arg(ap, struct Large);
    double real = va_arg(ap, double);
    long last = va_arg(ap, long);
    va_end(ap);
    value.first += pair.first;
    value.second += pair.second;
    value.third += marker + (long)real + last;
    return value;
}
int host_calls_aggregates(void) {
    struct Pair pair = {17, 25}; struct Pair changed = local_pair(pair, 3);
    if (changed.first != 20 || changed.second != 22) { return 1; }
    struct Reals reals = {17.5, 24.5}; struct Reals transformed = local_reals(reals, 2.5);
    if (transformed.first != 20 || transformed.second != 22) { return 2; }
    struct Mixed mixed = {17, 25}; struct Mixed mixed_result = local_mixed_record(mixed, 3);
    if (mixed_result.integer != 20 || mixed_result.real != 28) { return 3; }
    struct Reverse reverse = {2.5, 17}; struct Reverse reversed = local_reverse(reverse);
    if (reversed.real != 3.5 || reversed.integer != 19) { return 4; }
    struct Large large = {11, 13, 18}; struct Large large_result = local_large(large, 2);
    if (large_result.first != 13 || large_result.second != 15 || large_result.third != 20) { return 5; }
    struct Floats floats = {1.25f, 2.5f, 3.75f}; struct Floats floats_result = local_floats_record(floats);
    if (floats_result.first != 2.25f || floats_result.second != 4.5f || floats_result.third != 6.75f) { return 6; }
    struct Bytes bytes = {1, 2, 3}; struct Bytes bytes_result = local_bytes(bytes);
    if (bytes_result.first != 2 || bytes_result.second != 3 || bytes_result.third != 4) { return 7; }
    struct Nested nested = {{17, 25}}; struct Nested nested_result = local_nested(nested);
    if (nested_result.inner.integer != 18 || nested_result.inner.real != 27) { return 8; }
    union Word word = {.integer = 1099511627776L}; union Word word_result = local_word(word);
    if (word_result.integer != 1099511627777L) { return 9; }
    if (local_exhaust(1, 2, 3, 4, 5, pair, 6) != 63) { return 10; }
    if (local_float_exhaust(1, 2, 3, 4, 5, 6, 7, reals, 6) != 76) { return 11; }
    if (local_named_varargs(pair, 5, 1L, 2L, 3L, 4L, 5L) != 57) { return 12; }
    if (local_large_varargs(large, 7, 1L, 2L, 3L, 4L, 5L, 6L, 7L) != 70) { return 13; }
    struct Pair (*indirect)(struct Pair, long) = local_pair; changed = indirect(pair, 3);
    if (changed.first != 20 || changed.second != 22) { return 14; }
    union Word small_word = {.integer = 7};
    if (local_aggregate_varargs(1, pair, mixed, reals, large, small_word) != 175) { return 15; }
    if (local_aggregate_overflow(1, 2, 3, 4, 5, pair, 6L, mixed) != 105) { return 16; }
    struct Single single = {2.5f, 17}; struct Single single_result = local_single(single);
    if (single_result.real != 4.0f || single_result.integer != 19) { return 17; }
    struct Tail tail = {2.5, 1, 2, 3}; struct Tail tail_result = local_tail(tail);
    if (tail_result.real != 4.0 || tail_result.first != 2 || tail_result.second != 3 || tail_result.third != 4) { return 18; }
    struct Large (*large_indirect)(struct Large, long) = local_large; large_result = large_indirect(large, 2);
    if (large_result.first != 13 || large_result.second != 15 || large_result.third != 20) { return 19; }
    struct Embedded embedded = {{1.25f, 2.5f, 3.75f}, 17}; struct Embedded embedded_result = local_embedded(embedded);
    if (embedded_result.values[0] != 2.25f || embedded_result.values[1] != 4.5f || embedded_result.values[2] != 6.75f || embedded_result.tag != 21) { return 20; }
    large_result = local_variadic_return(1, pair, large, 2.5, 3L);
    if (large_result.first != 28 || large_result.second != 38 || large_result.third != 24) { return 21; }
    return 0;
}
