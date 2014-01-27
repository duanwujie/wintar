

#include "libbb.h"



static unsigned long long ret_ERANGE(void)
{
	errno = ERANGE; /* this ain't as small as it looks (on glibc) */
	return ULLONG_MAX;
}

static unsigned long long handle_errors(unsigned long long v, char **endp)
{
	char next_ch = **endp;

	/* errno is already set to ERANGE by strtoXXX if value overflowed */
	if (next_ch) {
		/* "1234abcg" or out-of-range? */
		if (isalnum(next_ch) || errno)
			return ret_ERANGE();
		/* good number, just suspicious terminator */
		errno = EINVAL;
	}
	return v;
}


unsigned long long bb_strtoull(const char *arg, char **endp, int base)
{
	unsigned long long v;
	char *endptr;

	if (!endp) endp = &endptr;
	*endp = (char*) arg;

	/* strtoul("  -4200000000") returns 94967296, errno 0 (!) */
	/* I don't think that this is right. Preventing this... */
	if (!isalnum(arg[0])) return ret_ERANGE();

	/* not 100% correct for lib func, but convenient for the caller */
	errno = 0;
	v = port_strtoll(arg, endp, base);
	return handle_errors(v, endp);
}

long long bb_strtoll(const char *arg, char **endp, int base)
{
	unsigned long long v;
	char *endptr;
	char first;

	if (!endp) endp = &endptr;
	*endp = (char*) arg;

	/* Check for the weird "feature":
	 * a "-" string is apparently a valid "number" for strto[u]l[l]!
	 * It returns zero and errno is 0! :( */
	first = (arg[0] != '-' ? arg[0] : arg[1]);
	if (!isalnum(first)) return ret_ERANGE();

	errno = 0;
	v = port_strtoll(arg, endp, base);
	return handle_errors(v, endp);
}

#if ULONG_MAX != ULLONG_MAX
unsigned long bb_strtoul(const char *arg, char **endp, int base)
{
	unsigned long v;
	char *endptr;

	if (!endp) endp = &endptr;
	*endp = (char*) arg;

	if (!isalnum(arg[0])) return ret_ERANGE();
	errno = 0;
	v = strtoul(arg, endp, base);
	return handle_errors(v, endp);
}

long bb_strtol(const char *arg, char **endp, int base)
{
	long v;
	char *endptr;
	char first;

	if (!endp) endp = &endptr;
	*endp = (char*) arg;

	first = (arg[0] != '-' ? arg[0] : arg[1]);
	if (!isalnum(first)) return ret_ERANGE();

	errno = 0;
	v = strtol(arg, endp, base);
	return handle_errors(v, endp);
}
#endif

unsigned bb_strtou(const char *arg, char **endp, int base)
{ 
	return bb_strtoul(arg, endp, base); 
}


#if UINT_MAX != ULONG_MAX
unsigned FAST_FUNC bb_strtou(const char *arg, char **endp, int base)
{
	unsigned long v;
	char *endptr;

	if (!endp) endp = &endptr;
	*endp = (char*) arg;

	if (!isalnum(arg[0])) return ret_ERANGE();
	errno = 0;
	v = strtoul(arg, endp, base);
	if (v > UINT_MAX) return ret_ERANGE();
	return handle_errors(v, endp);
}

int FAST_FUNC bb_strtoi(const char *arg, char **endp, int base)
{
	long v;
	char *endptr;
	char first;

	if (!endp) endp = &endptr;
	*endp = (char*) arg;

	first = (arg[0] != '-' ? arg[0] : arg[1]);
	if (!isalnum(first)) return ret_ERANGE();

	errno = 0;
	v = strtol(arg, endp, base);
	if (v > INT_MAX) return ret_ERANGE();
	if (v < INT_MIN) return ret_ERANGE();
	return handle_errors(v, endp);
}
#endif
