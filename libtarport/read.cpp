#include "libbb.h"


ssize_t safe_read(int fd, void *buf, size_t count)
{
	ssize_t n;

	do {
		n = port_read(fd, buf, count);
	} while (n < 0 && errno == EINTR);

	return n;
}