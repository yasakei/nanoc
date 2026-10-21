int host_mixed(int a, double b, int c, float d, int e, double f,
               int g, int h, int i, int j, double k) {
    return !(a == 1 && b == 2.5 && c == 3 && d == 4.5f && e == 5 && f == 6.5
             && g == 7 && h == 8 && i == 9 && j == 10 && k == 11.5);
}
int host_floats(double a, double b, double c, double d, double e,
                double f, double g, double h, double i, double j) {
    return !(a == 1 && b == 2 && c == 3 && d == 4 && e == 5 && f == 6
             && g == 7 && h == 8 && i == 9 && j == 10);
}
float host_float(float a, float b) { return a + b; }

extern int sum8(int, int, int, int, int, int, int, int);
extern int local_mixed(int, double, int, float, int, double, int, int, int, int, double);
extern double local_floats(double, double, double, double, double, double, double, double, double, double);
extern float local_float(float, float);
int host_calls_local(void) {
    if (sum8(1, 2, 3, 4, 5, 6, 7, 8) != 36) { return 1; }
    if (local_mixed(1, 2.5, 3, 4.5f, 5, 6.5, 7, 8, 9, 10, 11.5) != 0) { return 2; }
    if (local_floats(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) != 55) { return 3; }
    if (local_float(1.25f, 2.5f) != 3.75f) { return 4; }
    return 0;
}
