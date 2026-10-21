#include <stdint.h>

long global_large = 1099511627776L;
unsigned long global_mask = 0x8000000000000000UL;
long identity(long value) { return value; }
unsigned long unsigned_identity(unsigned long value) { return value; }

int main(void) {
    long big = 1099511627776L;
    if (big + 17 != 1099511627793L) { return 1; }
    if (17 + big != 1099511627793L) { return 2; }
    if (big - 17 != 1099511627759L) { return 3; }
    if (big * 3 != 3298534883328L) { return 4; }
    if (big / 3 != 366503875925L) { return 5; }
    if (big % 3 != 1) { return 6; }
    if (-big / 3 != -366503875925L) { return 7; }
    if (-big % 3 != -1) { return 8; }
    if ((big << 3) != 8796093022208L) { return 9; }
    if ((-big >> 3) != -137438953472L) { return 10; }
    unsigned long top = 0x8000000000000000UL;
    if ((top >> 63) != 1) { return 11; }
    if (top / 2 != 0x4000000000000000UL) { return 12; }
    if (top % 3 != 2) { return 13; }
    if (top <= 4294967296L) { return 14; }
    if ((top | 17) != 0x8000000000000011UL) { return 15; }
    if ((top ^ 17) != 0x8000000000000011UL) { return 16; }
    if ((~top & 255) != 255) { return 17; }
    if (global_large != big) { return 18; }
    if (global_mask != top) { return 19; }
    if (identity(big + 42) != 1099511627818L) { return 20; }
    if (unsigned_identity(top + 17) != 0x8000000000000011UL) { return 21; }
    unsigned int small = 4294967295U;
    if (small + 1 != 0) { return 22; }
    if ((long)small != 4294967295L) { return 23; }
    int negative = -1;
    if ((long)negative != -1L) { return 24; }
    if ((unsigned long)negative != ~0UL) { return 25; }
    if (negative < small) { return 26; }
    if (small < negative) { return 27; }
    if ((long)negative >= small) { return 28; }
    if ((unsigned long)small != 4294967295UL) { return 29; }
    long *pointer = &big;
    *pointer = 0x123456789abcdefL;
    if (big != 0x123456789abcdefL) { return 30; }
    long values[4];
    values[0] = 4294967296L;
    values[1] = 1099511627776L;
    values[2] = -1099511627776L;
    values[3] = 0x123456789abcdefL;
    pointer = &values[2];
    if (pointer[-1] != 1099511627776L) { return 31; }
    if (pointer - values != 2) { return 32; }
    if (values - pointer != -2) { return 33; }
    if (*(1 + values) != 1099511627776L) { return 34; }
    if ((values + 3) - (values + 1) != 2) { return 35; }
    if ((void *)pointer != (void *)&values[2]) { return 36; }
    if (sizeof(big) != 8) { return 37; }
    if (sizeof(values) != 32) { return 38; }
    if (sizeof(long) != 8) { return 39; }
    if (_Alignof(long) != 8) { return 40; }
    if (0x80000000 != 2147483648U) { return 41; }
    if (4294967295 != 4294967295L) { return 42; }
    return 0;
}
