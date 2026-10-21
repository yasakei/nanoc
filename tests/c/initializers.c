struct Record { long wide; short narrow; double real; };
union Variant { long whole; double real; };
extern int inferred_external[];
int inferred_external[] = {17, 25};

int first = 17, second = 25;
long wide_global = (1L << 40) + 7;
unsigned char global_bytes[5] = {1, 255, 17};
short global_shorts[] = {-123, 32767, 0};
long global_wide[] = {1099511627776L, -1099511627776L};
double global_reals[3] = {1.25, -2.5, 0.0};
int global_grid[2][3] = {{1, 2, 3}, {4, 5, 6}};
int global_designated[] = {[3] = 17, [0] = 25};
char global_text[] = "A\n" "B\x43\101";
char global_exact[3] = "abc";
char global_embedded[] = "a\0b";
char *message = "hello";
struct Record global_record = {.real = 2.5, .wide = 1099511627776L, .narrow = -123};
union Variant global_variant = {.whole = 1099511627776L};

int sum_grid(int grid[2][3]) {
    return grid[0][0] + grid[0][1] + grid[0][2] + grid[1][0] + grid[1][1] + grid[1][2];
}
int identity(int value) { return value; }
int main(void) {
    if (first + second != 42) { return 1; }
    if (wide_global != 1099511627783L) { return 2; }
    if (global_bytes[0] != 1 || global_bytes[1] != 255 || global_bytes[2] != 17) { return 3; }
    if (global_bytes[3] != 0 || global_bytes[4] != 0) { return 4; }
    if (sizeof(global_shorts) != 6 || global_shorts[0] != -123 || global_shorts[1] != 32767) { return 5; }
    if (global_wide[0] != 1099511627776L || global_wide[1] != -1099511627776L) { return 6; }
    if (global_reals[0] != 1.25 || global_reals[1] != -2.5) { return 7; }
    if (sum_grid(global_grid) != 21 || sizeof(global_grid) != 24 || sizeof(global_grid[0]) != 12) { return 8; }
    if (sizeof(global_designated) != 16 || global_designated[0] != 25 || global_designated[1] != 0 || global_designated[3] != 17) { return 9; }
    if (sizeof(global_text) != 6 || global_text[0] != 65 || global_text[1] != 10 || global_text[3] != 67 || global_text[4] != 65 || global_text[5] != 0) { return 10; }
    if (sizeof(global_exact) != 3 || global_exact[2] != 99) { return 11; }
    if (sizeof(global_embedded) != 4 || global_embedded[1] != 0 || global_embedded[2] != 98) { return 12; }
    if (message[0] != 104) { return 13; }
    if (global_record.wide != 1099511627776L || global_record.narrow != -123 || global_record.real != 2.5) { return 14; }
    if (global_variant.whole != 1099511627776L) { return 15; }

    int a = 17, b = 25, *p = &a;
    if (a + b != 42 || *p != 17) { return 16; }
    unsigned char bytes[5] = {identity(1), 255, 17};
    if (bytes[0] != 1 || bytes[1] != 255 || bytes[2] != 17 || bytes[3] != 0 || bytes[4] != 0) { return 17; }
    short shorts[] = {-123, 32767, 0};
    if (sizeof(shorts) != 6 || shorts[0] != -123 || shorts[1] != 32767) { return 18; }
    long wide[] = {1099511627776L, -1099511627776L};
    if (wide[0] != 1099511627776L || wide[1] != -1099511627776L) { return 19; }
    double reals[3] = {1.25, -2.5};
    if (reals[0] != 1.25 || reals[1] != -2.5 || reals[2] != 0.0) { return 20; }
    int grid[][3] = {{1, 2, 3}, {4, 5, 6}};
    if (sizeof(grid) != 24 || sum_grid(grid) != 21) { return 21; }
    int (*row)[3] = grid;
    if (row[1][2] != 6 || row + 1 != &grid[1]) { return 22; }
    int designated[] = {[3] = 17, [0] = 25};
    if (sizeof(designated) != 16 || designated[0] != 25 || designated[1] != 0 || designated[3] != 17) { return 23; }
    char text[] = "A\n" "B\x43\101";
    if (sizeof(text) != 6 || text[1] != 10 || text[3] != 67 || text[4] != 65 || text[5] != 0) { return 24; }
    char exact[3] = "abc", embedded[] = "a\0b";
    if (sizeof(exact) != 3 || exact[2] != 99 || sizeof(embedded) != 4 || embedded[2] != 98) { return 25; }
    struct Record record = {.real = 2.5, .wide = 1099511627776L, .narrow = -123};
    if (record.wide != 1099511627776L || record.narrow != -123 || record.real != 2.5) { return 26; }
    struct Record copied = record;
    if (copied.wide != record.wide || copied.narrow != record.narrow || copied.real != record.real) { return 27; }
    copied = global_record;
    copied.wide += 17;
    if (copied.wide != 1099511627793L || global_record.wide != 1099511627776L) { return 28; }
    union Variant variant = {.whole = 1099511627776L};
    if (variant.whole != 1099511627776L) { return 29; }
    typedef long Wide;
    Wide local_alias = 1099511627776L;
    if (local_alias != 1099511627776L) { return 30; }
    { int a = 25; if (a != 25) { return 31; } }
    if (a != 17) { return 32; }
    if (sizeof("a\0b") != 4 || sizeof("ab" "cd") != 5) { return 33; }
    if (sizeof(int[2][3]) != 24 || sizeof(grid + 1) != sizeof(void *)) { return 34; }
    if (sizeof(a + 1099511627776L) != 8) { return 35; }
    if ((a = 1, b = 2, a + b) != 3) { return 36; }
    typedef long Alias;
    { typedef short Alias; Alias narrow_alias = -123; if (sizeof(narrow_alias) != 2) { return 37; } }
    Alias wide_alias = 1099511627776L;
    if (sizeof(wide_alias) != 8 || wide_alias != 1099511627776L) { return 38; }
    int i = 25;
    for (int i = 0; i < 3; i++) { a += i; }
    if (i != 25 || a != 4) { return 39; }
    if (inferred_external[0] + inferred_external[1] != 42) { return 40; }
    int braced = {42};
    if (braced != 42) { return 41; }
    return 0;
}
