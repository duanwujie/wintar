#include "libbb.h"

void seek_by_jump(int fd, off_t amount)
{
	if (amount && port_lseek(fd, amount, SEEK_CUR) == (off_t) -1) {
		if (errno == ESPIPE)
			seek_by_read(fd, amount);
		else{
				//bb_perror_msg_and_die("seek failure");
		}
	}
}
