#include "libbb.h"

void seek_by_read(int fd, off_t amount)
{
	if (amount)
		bb_copyfd_exact_size(fd, -1, amount);
}
