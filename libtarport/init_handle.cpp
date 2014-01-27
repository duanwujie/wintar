#include "libbb.h"

archive_handle_t* init_handle(void)
{
	archive_handle_t *archive_handle;

	/* Initialize default values */
	archive_handle = (archive_handle_t *)xzalloc(sizeof(archive_handle_t));
	archive_handle->file_header = (file_header_t *)xzalloc(sizeof(file_header_t));
	archive_handle->action_header = header_skip;
	archive_handle->action_data = data_skip;
	archive_handle->filter = filter_accept_all;
	archive_handle->seek = seek_by_jump;

	return archive_handle;
}
