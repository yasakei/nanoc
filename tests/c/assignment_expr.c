struct Entry { long value; short small; double real; };
int calls;
int number(void) { calls++; return 2; }

int main(void) {
    long a = 0;
    long b = 0;
    if ((a = b = 4294967296L) != 4294967296L) { return 1; }
    if (a != b) { return 2; }
    if ((a += 2) != 4294967298L) { return 3; }
    if ((a -= 1) != 4294967297L) { return 4; }
    if ((a *= 3) != 12884901891L) { return 5; }
    if ((a /= 3) != 4294967297L) { return 6; }
    if ((a %= 100) != 97) { return 7; }
    if ((a <<= 35) != 3332894621696L) { return 8; }
    if ((a >>= 35) != 97) { return 9; }
    if ((a &= 15) != 1) { return 10; }
    if ((a |= 6) != 7) { return 11; }
    if ((a ^= 3) != 4) { return 12; }
    if (a++ != 4) { return 13; }
    if (++a != 6) { return 14; }
    if (a-- != 6) { return 15; }
    if (--a != 4) { return 16; }
    unsigned char byte = 255;
    if (byte++ != 255) { return 17; }
    if (byte != 0) { return 18; }
    if (--byte != 255) { return 19; }
    short narrow = 32767;
    if ((narrow += 1) != -32768) { return 20; }
    long values[4];
    calls = 0;
    values[number()] = 1099511627776L;
    if (calls != 1) { return 21; }
    if ((values[number()] += 17) != 1099511627793L) { return 22; }
    if (calls != 2) { return 23; }
    long *cursor = values;
    if (cursor++ != values) { return 24; }
    if (cursor - values != 1) { return 25; }
    if (++cursor != &values[2]) { return 26; }
    if (*cursor != 1099511627793L) { return 27; }
    if ((*cursor += 1) != 1099511627794L) { return 28; }
    struct Entry item;
    item.value = 1099511627776L;
    item.small = -123;
    item.real = 2.5;
    if (item.value != 1099511627776L) { return 29; }
    if (item.small != -123) { return 30; }
    if (item.real != 2.5) { return 31; }
    struct Entry *address = &item;
    address->value += 9;
    if (item.value != 1099511627785L) { return 32; }
    if ((item.real *= 2) != 5.0) { return 33; }
    if (0 && (calls = 99)) { return 34; }
    if (calls != 2) { return 35; }
    if (!(1 || (calls = 99))) { return 36; }
    if (calls != 2) { return 37; }
    if ((1 ? 4294967296L : -1) != 4294967296L) { return 38; }
    if ((0 ? -1 : 4294967296L) != 4294967296L) { return 39; }
    if ((0 ? 2.5 : 7) != 7.0) { return 40; }
    if ((1 ? 2.5 : 7) != 2.5) { return 41; }
    if (sizeof(calls++) != 4) { return 42; }
    if (calls != 2) { return 43; }
    return 0;
}
