#include "libbb.h"

ssize_t safe_write(int fd, const void *buf, size_t count)
{
	ssize_t n;

	do {
		n = port_write(fd, buf, count);
	} while (n < 0 && errno == EINTR);

	return n;
}
