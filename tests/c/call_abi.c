int aligned(void);
int host_calls_local(void);
int host_mixed(int a, double b, int c, float d, int e, double f,
               int g, int h, int i, int j, double k);
int host_floats(double a, double b, double c, double d, double e,
                double f, double g, double h, double i, double j);
float host_float(float a, float b);

int id(int x) { return x; }
int sum8(int a, int b, int c, int d, int e, int f, int g, int h) {
    return a + b + c + d + e + f + g + h;
}
int local_mixed(int a, double b, int c, float d, int e, double f,
                int g, int h, int i, int j, double k) {
    if (a != 1) { return 1; }
    if (b != 2.5) { return 2; }
    if (c != 3) { return 3; }
    if (d != 4.5) { return 4; }
    if (e != 5) { return 5; }
    if (f != 6.5) { return 6; }
    if (g != 7) { return 7; }
    if (h != 8) { return 8; }
    if (i != 9) { return 9; }
    if (j != 10) { return 10; }
    if (k != 11.5) { return 11; }
    return 0;
}
double local_floats(double a, double b, double c, double d, double e,
                    double f, double g, double h, double i, double j) {
    return a + b + c + d + e + f + g + h + i + j;
}
float local_float(float a, float b) { return a + b; }
int fallthrough(int condition) {
    if (condition) { return 7; }
}
int large_frame(void) {
    int values[200];
    values[0] = 17;
    values[199] = 25;
    return values[0] + values[199];
}
int main(void) {
    if (sum8(1, id(2), 3, 4, id(5), 6, 7, 8) != 36) { return 1; }
    if (id(1) + sum8(1, 2, 3, 4, 5, 6, 7, 8) != 37) { return 2; }
    if (aligned() != 1) { return 3; }
    if (id(4) + aligned() != 5) { return 4; }
    if (sum8(aligned(), 2, aligned(), 4, aligned(), 6, aligned(), 8) != 24) { return 5; }
    if (host_mixed(1, 2.5, 3, 4.5, 5, 6.5, 7, 8, 9, 10, 11.5) != 0) { return 6; }
    if (local_mixed(1, 2.5, 3, 4.5, 5, 6.5, 7, 8, 9, 10, 11.5) != 0) { return 7; }
    if (host_floats(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0) != 0) { return 8; }
    double total = local_floats(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0);
    if (total != 55.0) { return 9; }
    float native = host_float(1.25, 2.5);
    if (native != 3.75) { return 10; }
    float local = local_float(1.25, 2.5);
    if (local != 3.75) { return 11; }
    if (large_frame() != 42) { return 12; }
    int (*indirect)(int, int, int, int, int, int, int, int) = sum8;
    if (indirect(1, 2, 3, 4, 5, 6, 7, 8) != 36) { return 13; }
    if (host_calls_local() != 0) { return 14; }
    if (fallthrough(0) != 0) { return 15; }
    float suffixed = 1.25f;
    if (suffixed != 1.25) { return 16; }
    return 0;
}
