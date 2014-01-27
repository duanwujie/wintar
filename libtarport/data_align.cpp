#include "libbb.h"


void data_align(archive_handle_t *archive_handle, unsigned boundary)
{
	unsigned skip_amount = (boundary - (archive_handle->offset % boundary)) % boundary;

	archive_handle->seek(archive_handle->src_fd, skip_amount);
	archive_handle->offset += skip_amount;
}
