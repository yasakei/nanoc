#include "../host/aggregate_abi.h"
#include <stdarg.h>

struct Pair local_pair(struct Pair value, long increment) {
    value.first += increment;
    value.second -= increment;
    return value;
}
struct Reals local_reals(struct Reals value, double increment) {
    value.first += increment;
    value.second -= increment;
    return value;
}
struct Mixed local_mixed_record(struct Mixed value, int increment) {
    value.integer += increment;
    value.real += increment;
    return value;
}
struct Reverse local_reverse(struct Reverse value) {
    value.real += 1.0;
    value.integer += 2;
    return value;
}
struct Large local_large(struct Large value, long increment) {
    value.first += increment;
    value.second += increment;
    value.third += increment;
    return value;
}
struct Floats local_floats_record(struct Floats value) {
    value.first += 1.0;
    value.second += 2.0;
    value.third += 3.0;
    return value;
}
struct Bytes local_bytes(struct Bytes value) {
    value.first++;
    value.second++;
    value.third++;
    return value;
}
struct Single local_single(struct Single value) {
    value.real += 1.5;
    value.integer += 2;
    return value;
}
struct Tail local_tail(struct Tail value) {
    value.real += 1.5;
    value.first++;
    value.second++;
    value.third++;
    return value;
}
struct Embedded local_embedded(struct Embedded value) {
    value.values[0] += 1.0;
    value.values[1] += 2.0;
    value.values[2] += 3.0;
    value.tag += 4;
    return value;
}
struct Nested local_nested(struct Nested value) {
    value.inner.integer += 1;
    value.inner.real += 2.0;
    return value;
}
union Word local_word(union Word value) {
    value.integer += 1;
    return value;
}
long local_exhaust(long a, long b, long c, long d, long e, struct Pair pair, long last) {
    return a + b + c + d + e + pair.first + pair.second + last;
}
long local_float_exhaust(double a, double b, double c, double d, double e, double f, double g, struct Reals pair, long last) {
    double total = a + b + c + d + e + f + g + pair.first + pair.second;
    return (long)total + last;
}
long local_named_varargs(struct Pair pair, int count, ...) {
    va_list ap;
    va_start(ap, count);
    long total = pair.first + pair.second;
    for (int i = 0; i < count; i++) { total += va_arg(ap, long); }
    va_end(ap);
    return total;
}
long local_large_varargs(struct Large value, int count, ...) {
    va_list ap;
    va_start(ap, count);
    long total = value.first + value.second + value.third;
    for (int i = 0; i < count; i++) { total += va_arg(ap, long); }
    va_end(ap);
    return total;
}
long local_aggregate_varargs(int marker, ...) {
    va_list ap;
    va_start(ap, marker);
    struct Pair pair = va_arg(ap, struct Pair);
    struct Mixed mixed = va_arg(ap, struct Mixed);
    struct Reals reals = va_arg(ap, struct Reals);
    struct Large large = va_arg(ap, struct Large);
    union Word word = va_arg(ap, union Word);
    va_end(ap);
    return marker + pair.first + pair.second + mixed.integer + (long)mixed.real
        + (long)reals.first + (long)reals.second + large.first + large.second + large.third + word.integer;
}
long local_aggregate_overflow(long a, long b, long c, long d, long e, ...) {
    va_list ap;
    va_start(ap, e);
    struct Pair pair = va_arg(ap, struct Pair);
    long last = va_arg(ap, long);
    struct Mixed mixed = va_arg(ap, struct Mixed);
    va_end(ap);
    return a + b + c + d + e + pair.first + pair.second + last + mixed.integer + (long)mixed.real;
}
struct Large local_variadic_return(int marker, ...) {
    va_list ap;
    va_start(ap, marker);
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
int main(void) {
    struct Pair pair = {17, 25};
    struct Pair changed = host_pair(pair, 3);
    if (changed.first != 20 || changed.second != 22 || pair.first != 17) { return 1; }
    changed = local_pair(pair, 3);
    if (changed.first != 20 || changed.second != 22) { return 2; }
    changed = local_pair(host_pair(pair, 3), 2);
    if (changed.first != 22 || changed.second != 20) { return 3; }
    if (host_pair(pair, 3).first != 20) { return 4; }
    struct Reals reals = {17.5, 24.5};
    struct Reals transformed = host_reals(reals, 2.5);
    if (transformed.first != 20.0 || transformed.second != 22.0) { return 5; }
    transformed = local_reals(reals, 2.5);
    if (transformed.first != 20.0 || transformed.second != 22.0) { return 6; }
    struct Mixed mixed = {17, 25.0};
    struct Mixed mixed_result = host_mixed_record(mixed, 3);
    if (mixed_result.integer != 20 || mixed_result.real != 28.0) { return 7; }
    mixed_result = local_mixed_record(mixed, 3);
    if (mixed_result.integer != 20 || mixed_result.real != 28.0) { return 8; }
    struct Reverse reverse = {2.5, 17};
    struct Reverse reversed = host_reverse(reverse);
    if (reversed.real != 3.5 || reversed.integer != 19) { return 9; }
    reversed = local_reverse(reverse);
    if (reversed.real != 3.5 || reversed.integer != 19) { return 10; }
    struct Large large = {11, 13, 18};
    struct Large large_result = host_large(large, 2);
    if (large_result.first != 13 || large_result.second != 15 || large_result.third != 20) { return 11; }
    large_result = local_large(host_large(large, 2), 3);
    if (large_result.first != 16 || large_result.second != 18 || large_result.third != 23) { return 12; }
    if (large.first != 11 || large.second != 13 || large.third != 18) { return 13; }
    struct Floats floats = {1.25, 2.5, 3.75};
    struct Floats floats_result = host_floats_record(floats);
    if (floats_result.first != 2.25 || floats_result.second != 4.5 || floats_result.third != 6.75) { return 14; }
    floats_result = local_floats_record(floats);
    if (floats_result.first != 2.25 || floats_result.second != 4.5 || floats_result.third != 6.75) { return 15; }
    struct Bytes bytes = {1, 2, 3};
    struct Bytes bytes_result = host_bytes(bytes);
    if (bytes_result.first != 2 || bytes_result.second != 3 || bytes_result.third != 4) { return 16; }
    bytes_result = local_bytes(bytes);
    if (bytes_result.first != 2 || bytes_result.second != 3 || bytes_result.third != 4) { return 17; }
    struct Nested nested = {{17, 25.0}};
    struct Nested nested_result = host_nested(nested);
    if (nested_result.inner.integer != 18 || nested_result.inner.real != 27.0) { return 18; }
    nested_result = local_nested(nested);
    if (nested_result.inner.integer != 18 || nested_result.inner.real != 27.0) { return 19; }
    union Word word = {.integer = 1099511627776L};
    union Word word_result = host_word(word);
    if (word_result.integer != 1099511627777L) { return 20; }
    word_result = local_word(word);
    if (word_result.integer != 1099511627777L) { return 21; }
    if (host_exhaust(1, 2, 3, 4, 5, pair, 6) != 63) { return 22; }
    if (local_exhaust(1, 2, 3, 4, 5, pair, 6) != 63) { return 23; }
    if (host_float_exhaust(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, reals, 6) != 76) { return 24; }
    if (local_float_exhaust(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, reals, 6) != 76) { return 25; }
    if (host_named_varargs(pair, 5, 1L, 2L, 3L, 4L, 5L) != 57) { return 26; }
    if (local_named_varargs(pair, 5, 1L, 2L, 3L, 4L, 5L) != 57) { return 27; }
    if (host_large_varargs(large, 7, 1L, 2L, 3L, 4L, 5L, 6L, 7L) != 70) { return 28; }
    if (local_large_varargs(large, 7, 1L, 2L, 3L, 4L, 5L, 6L, 7L) != 70) { return 29; }
    struct Pair (*indirect)(struct Pair, long) = host_pair;
    changed = indirect(pair, 3);
    if (changed.first != 20 || changed.second != 22) { return 30; }
    if (host_calls_aggregates() != 0) { return 31; }
    union Word small_word = {.integer = 7};
    if (local_aggregate_varargs(1, pair, mixed, reals, large, small_word) != 175) { return 32; }
    if (host_aggregate_varargs(1, pair, mixed, reals, large, small_word) != 175) { return 33; }
    if (local_aggregate_overflow(1, 2, 3, 4, 5, pair, 6L, mixed) != 105) { return 34; }
    if (host_aggregate_overflow(1, 2, 3, 4, 5, pair, 6L, mixed) != 105) { return 35; }
    struct Single single = {2.5, 17};
    struct Single single_result = host_single(single);
    if (single_result.real != 4.0 || single_result.integer != 19) { return 36; }
    single_result = local_single(single);
    if (single_result.real != 4.0 || single_result.integer != 19) { return 37; }
    struct Tail tail = {2.5, 1, 2, 3};
    struct Tail tail_result = host_tail(tail);
    if (tail_result.real != 4.0 || tail_result.first != 2 || tail_result.second != 3 || tail_result.third != 4) { return 38; }
    tail_result = local_tail(tail);
    if (tail_result.real != 4.0 || tail_result.first != 2 || tail_result.second != 3 || tail_result.third != 4) { return 39; }
    struct Large (*large_indirect)(struct Large, long) = host_large;
    large_result = large_indirect(large, 2);
    if (large_result.first != 13 || large_result.second != 15 || large_result.third != 20) { return 40; }
    struct Embedded embedded = {{1.25, 2.5, 3.75}, 17};
    struct Embedded embedded_result = host_embedded(embedded);
    if (embedded_result.values[0] != 2.25 || embedded_result.values[1] != 4.5 || embedded_result.values[2] != 6.75 || embedded_result.tag != 21) { return 41; }
    embedded_result = local_embedded(embedded);
    if (embedded_result.values[0] != 2.25 || embedded_result.values[1] != 4.5 || embedded_result.values[2] != 6.75 || embedded_result.tag != 21) { return 42; }
    large_result = local_variadic_return(1, pair, large, 2.5, 3L);
    if (large_result.first != 28 || large_result.second != 38 || large_result.third != 24) { return 43; }
    large_result = host_variadic_return(1, pair, large, 2.5, 3L);
    if (large_result.first != 28 || large_result.second != 38 || large_result.third != 24) { return 44; }
    return 0;
}
