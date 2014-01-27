#include "libbb.h"

static void
call_arg_warn (char const *call, char const *name)
{
  int e = getError();
  //WARN ((0, e, _("%s: Warning: Cannot %s"), quotearg_colon (name), call));
}

void
readlink(char const *name)
{
  call_arg_warn ("readlink", name);
}

#if 0
char* xmalloc_readlink(const char *path)
{
	enum { GROWBY = 80 }; /* how large we will grow strings by */

	char *buf = NULL;
	int bufsize = 0, readsize = 0;

	do {
		bufsize += GROWBY;
		buf = (char *)xrealloc(buf, bufsize);
		readsize = readlink(path, buf, bufsize);
		if (readsize == -1) {
			free(buf);
			return NULL;
		}
	} while (bufsize < readsize + 1);

	buf[readsize] = '\0';

	return buf;
}

char* xmalloc_readlink_or_warn(const char *path)
{
	char *buf = xmalloc_readlink(path);
	if (!buf) {
		/* EINVAL => "file: Invalid argument" => puzzled user */
		const char *errmsg = "not a symlink";
		int err = errno;
		if (err != EINVAL)
			errmsg = strerror(err);
		bb_error_msg("%s: cannot read link: %s", path, errmsg);
	}
	return buf;
}
#endif




void* xrealloc(void *ptr, size_t size)
{
	ptr = realloc(ptr, size);
	if (ptr == NULL && size != 0){
		//bb_error_msg_and_die(bb_msg_memory_exhausted);
	}
	return ptr;
}

// Die if we can't open a file and return a fd.
int xopen3(const char *pathname, int flags, int mode)
{
	int ret;
#if _WIN32
	int pfh=0;
	//ret = port_open(&pfh, pathname, _O_RDONLY, _SH_DENYNO, _S_IREAD);
	ret = port_open(pathname, flags);
#else
	ret = port_open(pathname, flags, mode);
#endif
	if (ret < 0) {
		return -1;
	}
	return ret;
}

// Die if we can't open a file and return a fd.
int xopen(const char *pathname, int flags)
{
	return xopen3(pathname, flags, 0666);
}

void*  xmalloc(size_t size)
{
	void *ptr = malloc(size);
	if (ptr == NULL && size != 0)
		return NULL;
	return ptr;
}


void* xzalloc(size_t size)
{
	void *ptr = xmalloc(size);
	memset(ptr, 0, size);
	return ptr;
}

void  xfstat(int fd, struct port_stat *stat_buf, const char *errmsg)
{
	/* errmsg is usually a file name, but not always:
	 * xfstat may be called in a spot where file name is no longer
	 * available, and caller may give e.g. "can't stat input file" string.
	 */
	if (port_fstat(fd, stat_buf))
		bb_simple_perror_msg_and_die(errmsg);
}

void xwrite(int fd, const void *buf, size_t count)
{
	if (count) {
		ssize_t size = full_write(fd, buf, count);
		if ((size_t)size != count)
			bb_error_msg_and_die("short write");
	}
}



int vasprintf(char **string_ptr, const char *format, va_list p)
{
	int r;
	va_list p2;
	char buf[128];

	va_copy(p2, p);
	r = port_vsnprintf(buf, 128, format, p);
	va_end(p);

	/* Note: can't use xstrdup/xmalloc, they call vasprintf (us) on failure! */

	if (r < 128) {
		va_end(p2);
		*string_ptr = port_strdup(buf);
		return (*string_ptr ? r : -1);
	}

	*string_ptr =(char *) malloc(r+1);
	r = (*string_ptr ? vsnprintf(*string_ptr, r+1, format, p2) : -1);
	va_end(p2);

	return r;
}

char*  xasprintf(const char *format, ...)
{
	va_list p;
	int r;
	char *string_ptr;

	va_start(p, format);
	r = vasprintf(&string_ptr, format, p);
	va_end(p);

	if (r < 0){
		//bb_error_msg_and_die(bb_msg_memory_exhausted);
	}
	return string_ptr;
}

// Warn if we can't open a file and return a fd.
int open3_or_warn(const char *pathname, int flags, int mode)
{
	int ret;

#if _WIN32
	int pfh=0;
	//ret = port_open(&pfh, pathname,flags,_S_IREAD | _S_IWRITE,_SH_DENYNO);
	ret = port_open(pathname, flags, mode);
#else
	ret = port_open(pathname, flags, mode);
#endif
	if (ret < 0) {
		bb_perror_msg("can't open '%s'", pathname);
	}
	return ret;
}

// Warn if we can't open a file and return a fd.
int open_or_warn(const char *pathname, int flags)
{
	return open3_or_warn(pathname, flags, 0666);
}


ssize_t full_read(int fd, void *buf, size_t len)
{
	ssize_t cc;
	ssize_t total;

	total = 0;

	while (len) {
		cc = safe_read(fd, buf, len);

		if (cc < 0) {
			if (total) {
				/* we already have some! */
				/* user can do another read to know the error code */
				return total;
			}
			return cc; /* read() returns -1 on failure. */
		}
		if (cc == 0)
			break;
		buf = ((char *)buf) + cc;
		total += cc;
		len -= cc;
	}

	return total;
}

/* Die with an error message if we can't read the entire buffer. */
void xread(int fd, void *buf, size_t count)
{
	if (count) {
		ssize_t size = full_read(fd, buf, count);
		if ((size_t)size != count)
			bb_error_msg_and_die("short read");
	}
}

char* xstrndup(const char *s, int n)
{
	int m;
	char *t;

	if (ENABLE_DEBUG && s == NULL)
		bb_error_msg_and_die("xstrndup bug");

	/* We can just xmalloc(n+1) and strncpy into it, */
	/* but think about xstrndup("abc", 10000) wastage! */
	m = n;
	t = (char*) s;
	while (m) {
		if (!*t) break;
		m--;
		t++;
	}
	n -= m;
	t = (char *)xmalloc(n + 1);
	t[n] = '\0';

	return (char *)memcpy(t, s, n);
}

char* xstrdup(const char *s)
{
	char *t;

	if (s == NULL)
		return NULL;

	t = strdup(s);

	if (t == NULL){
		//bb_error_msg_and_die(bb_msg_memory_exhausted);
	}

	return t;
}
