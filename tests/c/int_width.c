int check_byte(unsigned char x) {
    if (x != 255) { return 1; }
    if ((signed char)x != -1) { return 2; }
    return 0;
}

int main() {
    unsigned char uc = 255;
    short s = -2;
    unsigned int u = 4294967295;
    short *p = &s;
    if (check_byte(uc) != 0) { return 3; }
    if (s >= -1) { return 4; }
    if (u < 1) { return 5; }
    if (u / 2 != 2147483647) { return 6; }
    if ((u >> 31) != 1) { return 7; }
    if (*p != -2) { return 8; }
    *p = 3;
    if (s != 3) { return 9; }
    return 0;
}
