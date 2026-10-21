double double_identity(double value) { return value; }
float float_identity(float value) { return value; }
int main(void) {
    double value = 2 + 0.5;
    if (value != 2.5) { return 1; }
    if (3 + value != 5.5) { return 2; }
    if (value + 3 != 5.5) { return 3; }
    if (7 - value != 4.5) { return 4; }
    if (3 * value != 7.5) { return 5; }
    if (10 / value != 4.0) { return 6; }
    if (!(value > 2)) { return 7; }
    if (!(2 < value)) { return 8; }
    if ((value > 2) + (value < 3) != 2) { return 9; }
    if (!value) { return 10; }
    if (!(value && 7)) { return 11; }
    if (0.0 || 0) { return 12; }
    long big = 1099511627776L;
    if ((double)big != 1099511627776.0) { return 13; }
    if ((long)1099511627776.75 != big) { return 14; }
    unsigned long top = 0x8000000000000000UL;
    if ((double)top != 9223372036854775808.0) { return 15; }
    if ((unsigned long)9223372036854775808.0 != top) { return 16; }
    if (double_identity(big) != 1099511627776.0) { return 17; }
    float rounded = 16777217;
    if (rounded != 16777216.0) { return 18; }
    if (float_identity(16777217) != 16777216.0) { return 19; }
    value += 1.5;
    if (value != 4.0) { return 20; }
    if (value++ != 4.0) { return 21; }
    if (++value != 6.0) { return 22; }
    double *pointer = &value;
    *pointer = 3.25;
    if (*pointer != 3.25) { return 23; }
    double zero = 0.0;
    double nan = zero / zero;
    if (nan == nan) { return 24; }
    if (!(nan != nan)) { return 25; }
    if (nan < zero) { return 26; }
    if (nan <= zero) { return 27; }
    if (nan > zero) { return 28; }
    if (nan >= zero) { return 29; }
    if (!nan) { return 30; }
    if (!(nan && 1)) { return 31; }
    if (sizeof(value) != 8) { return 32; }
    if (_Alignof(double) != 8) { return 33; }
    if (_Alignof(float) != 4) { return 34; }
    return 0;
}
