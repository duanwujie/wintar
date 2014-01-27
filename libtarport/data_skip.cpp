#include "libbb.h"

void data_skip(archive_handle_t *archive_handle)
{
	archive_handle->seek(archive_handle->src_fd, archive_handle->file_header->size);
}
