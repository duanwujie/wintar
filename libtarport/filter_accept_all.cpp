#include "libbb.h"

char filter_accept_all(archive_handle_t *archive_handle)
{
	if (archive_handle->file_header->name)
		return EXIT_SUCCESS;
	return EXIT_FAILURE;
}
