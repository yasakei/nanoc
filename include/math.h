#ifndef _NANOC_MATH_H
#define _NANOC_MATH_H

typedef float float_t;
typedef double double_t;

#define M_E 2.7182818284590452354
#define M_LOG2E 1.4426950408889634074
#define M_LOG10E 0.43429448190325182765
#define M_LN2 0.69314718055994530942
#define M_LN10 2.30258509299404568402
#define M_PI 3.14159265358979323846
#define M_PI_2 1.57079632679489661923
#define M_PI_4 0.78539816339744830962
#define M_1_PI 0.31830988618379067154
#define M_2_PI 0.63661977236758134308
#define M_2_SQRTPI 1.12837916709551257390
#define M_SQRT2 1.41421356237309504880
#define M_SQRT1_2 0.70710678118654752440

double acos(double x);
double asin(double x);
double atan(double x);
double atan2(double y, double x);
double cos(double x);
double sin(double x);
double tan(double x);
double cosh(double x);
double sinh(double x);
double tanh(double x);
double acosh(double x);
double asinh(double x);
double atanh(double x);

double exp(double x);
double exp2(double x);
double expm1(double x);
double frexp(double x, int *exp);
double ldexp(double x, int exp);
double log(double x);
double log10(double x);
double log1p(double x);
double log2(double x);
double logb(double x);
double modf(double x, double *ipart);

double pow(double x, double y);
double sqrt(double x);
double cbrt(double x);
double hypot(double x, double y);

double ceil(double x);
double floor(double x);
double fmod(double x, double y);
double remainder(double x, double y);
double remquo(double x, double y, int *quo);
double copysign(double x, double y);
double nan(const char *tag);
double fabs(double x);
double fdim(double x, double y);
double fmax(double x, double y);
double fmin(double x, double y);

double erf(double x);
double erfc(double x);
double tgamma(double x);
double lgamma(double x);
double j0(double x);
double j1(double x);
double jn(int n, double x);

double nearbyint(double x);
double rint(double x);
double lrint(double x);
long long llrint(double x);
double round(double x);
long lround(double x);
long long llround(double x);
double trunc(double x);

int isfinite(double x);
int isinf(double x);
int isnan(double x);
int signbit(double x);

float acosf(float x);
float asinf(float x);
float atanf(float x);
float cosf(float x);
float sinf(float x);
float tanf(float x);
float expf(float x);
float logf(float x);
float powf(float x, float y);
float sqrtf(float x);
float fabsf(float x);

#endif
