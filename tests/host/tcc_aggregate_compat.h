#ifndef NANOC_TCC_AGGREGATE_COMPAT_H
#define NANOC_TCC_AGGREGATE_COMPAT_H
struct CompatPair { long first; long second; };
struct CompatReals { double first; double second; };
struct CompatLarge { long first; long second; long third; };
struct CompatBytes { char first; char second; char third; };
union CompatWord { long integer; double real; };
struct CompatPair compat_local_pair(struct CompatPair value, long increment);
struct CompatPair compat_host_pair(struct CompatPair value, long increment);
struct CompatReals compat_local_reals(struct CompatReals value, double increment);
struct CompatReals compat_host_reals(struct CompatReals value, double increment);
struct CompatLarge compat_local_large(struct CompatLarge value, long increment);
struct CompatLarge compat_host_large(struct CompatLarge value, long increment);
struct CompatBytes compat_local_bytes(struct CompatBytes value);
struct CompatBytes compat_host_bytes(struct CompatBytes value);
union CompatWord compat_local_word(union CompatWord value);
union CompatWord compat_host_word(union CompatWord value);
long compat_local_exhaust(long a, long b, long c, long d, long e, struct CompatPair pair, long last);
long compat_host_exhaust(long a, long b, long c, long d, long e, struct CompatPair pair, long last);
long compat_local_varargs(int marker, ...);
long compat_host_varargs(int marker, ...);
int compat_host_calls_local(void);
#endif
