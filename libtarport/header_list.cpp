
#include "libbb.h"


void header_list(const file_header_t *file_header)
{
//TODO: cpio -vp DIR should output "DIR/NAME", not just "NAME" */
	puts(file_header->name);
}
