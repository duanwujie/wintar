#ifndef __ARCHIVE_H_
#define __ARCHIVE_H_

#include "libbb.h"

typedef struct file_header_t {
	char *name;
	char *link_target;
#if ENABLE_FEATURE_TAR_UNAME_GNAME
	char *tar__uname;
	char *tar__gname;
#endif
	off_t size;
	uid_t uid;
	gid_t gid;
	mode_t mode;
	time_t mtime;
	dev_t device;
} file_header_t;

typedef struct archive_handle_t {
	unsigned ah_flags;/* Flags. 1st since it is most used member */
	int src_fd;/* The raw stream as read from disk or stdin */
	/* Define if the header and data component should be processed */
	char (*filter)(struct archive_handle_t *);
	llist_t *accept;/* List of files that have been accepted */
	llist_t *reject;/* List of files that have been rejected */
	llist_t *passed;/* List of files that have successfully been worked on */

	file_header_t *file_header;/* Currently processed file's header */

	
	/* Process the header component, e.g. tar -t */
	void (*action_header)(const file_header_t *);

	/* Process the data component, e.g. extract to filesystem */
	void (*action_data)(struct archive_handle_t *);

	void (*seek)(int fd, off_t amount);/* Function that skips data */
	off_t offset;/* Count processed bytes */

	/* Archiver specific. Can make it a union if it ever gets big */
#define PAX_NEXT_FILE 0
#define PAX_GLOBAL    1

	smallint tar__end;

	char* tar__longname;
	char* tar__linkname;


	char* tar__to_command;
	const char* tar__to_command_shell;

} archive_handle_t;

#define ARCHIVE_RESTORE_DATE        (1 << 0)
#define ARCHIVE_CREATE_LEADING_DIRS (1 << 1)
#define ARCHIVE_UNLINK_OLD          (1 << 2)
#define ARCHIVE_EXTRACT_QUIET       (1 << 3)
#define ARCHIVE_EXTRACT_NEWER       (1 << 4)
#define ARCHIVE_DONT_RESTORE_OWNER  (1 << 5)
#define ARCHIVE_DONT_RESTORE_PERM   (1 << 6)
#define ARCHIVE_NUMERIC_OWNER       (1 << 7)
#define ARCHIVE_O_TRUNC             (1 << 8)
#define ARCHIVE_REMEMBER_NAMES      (1 << 9)



#define TAR_BLOCK_SIZE 512
#define NAME_SIZE      100
#define NAME_SIZE_STR "100"
/**
 * @brief Tar Format
 */
typedef struct tar_header_t {     /* byte offset */
	char name[NAME_SIZE];     /*   0-99 */
	char mode[8];             /* 100-107 */
	char uid[8];              /* 108-115 */
	char gid[8];              /* 116-123 */
	char size[12];            /* 124-135 */
	char mtime[12];           /* 136-147 */
	char chksum[8];           /* 148-155 */
	char typeflag;            /* 156-156 */
	char linkname[NAME_SIZE]; /* 157-256 */
	/* POSIX:   "ustar" NUL "00" */
	/* GNU tar: "ustar  " NUL */
	/* Normally it's defined as magic[6] followed by
	 * version[2], but we put them together to save code.
	 */
	char magic[8];            /* 257-264 */
	char uname[32];           /* 265-296 */
	char gname[32];           /* 297-328 */
	char devmajor[8];         /* 329-336 */
	char devminor[8];         /* 337-344 */
	char prefix[155];         /* 345-499 */
	char padding[12];         /* 500-512 (pad to exactly TAR_BLOCK_SIZE) */
} tar_header_t;

#endif