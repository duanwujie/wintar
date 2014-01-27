#ifndef __LIBBB_H_
#define __LIBBB_H_

#include <iostream>
using namespace std;

#include "platform.h"

#include "xfuncs.h"
#include "llist.h"
#include "archive.h"

#if _WIN32
#include "dirent.h"
#endif

#define LONE_DASH(s)     ((s)[0] == '-' && !(s)[1])
#define SKIP	((int) 2)
#define DOT_OR_DOTDOT(s) ((s)[0] == '.' && (!(s)[1] || ((s)[1] == '.' && !(s)[2])))
#define LONE_CHAR(s,c)     ((s)[0] == (c) && !(s)[1])
#  define OFF_FMT "l"
#define ENABLE_DEBUG 0

#define ENABLE_FEATURE_TAR_AUTODETECT 0
# define IF_FEATURE_TAR_OLDSUN_COMPATIBILITY(...) __VA_ARGS__
#define ENABLE_FEATURE_TAR_OLDSUN_COMPATIBILITY 1



//#define WARN(Args) error Args

enum {
	ACTION_RECURSE        = (1 << 0),
	ACTION_FOLLOWLINKS    = (1 << 1),
	ACTION_FOLLOWLINKS_L0 = (1 << 2),
	ACTION_DEPTHFIRST     = (1 << 3),
	/*ACTION_REVERSE      = (1 << 4), - unused */
	ACTION_QUIET          = (1 << 5),
	ACTION_DANGLING_OK    = (1 << 6),
};

enum {	/* DO NOT CHANGE THESE VALUES!  cp.c, mv.c, install.c depend on them. */
	FILEUTILS_PRESERVE_STATUS = 1 << 0, /* -p */
	FILEUTILS_DEREFERENCE     = 1 << 1, /* !-d */
	FILEUTILS_RECUR           = 1 << 2, /* -R */
	FILEUTILS_FORCE           = 1 << 3, /* -f */
	FILEUTILS_INTERACTIVE     = 1 << 4, /* -i */
	FILEUTILS_MAKE_HARDLINK   = 1 << 5, /* -l */
	FILEUTILS_MAKE_SOFTLINK   = 1 << 6, /* -s */
	FILEUTILS_DEREF_SOFTLINK  = 1 << 7, /* -L */
	FILEUTILS_DEREFERENCE_L0  = 1 << 8, /* -H */
#if ENABLE_SELINUX
	FILEUTILS_PRESERVE_SECURITY_CONTEXT = 1 << 9, /* -c */
	FILEUTILS_SET_SECURITY_CONTEXT = 1 << 10,
#endif
	FILEUTILS_IGNORE_CHMOD_ERR = 1 << 11,
};

archive_handle_t* init_handle(void);
void header_skip(const file_header_t *file_header);
void data_skip(archive_handle_t *archive_handle);
char filter_accept_all(archive_handle_t *archive_handle);
void bb_copyfd_exact_size(int fd1, int fd2, off_t size);
void seek_by_read(int fd, off_t amount);
void seek_by_jump(int fd, off_t amount);
ssize_t safe_read(int fd, void *buf, size_t count);

#define bb_msg_read_error "read error"
#define bb_msg_write_error "write error"
void bb_perror_msg(const char *s, ...);
void bb_error_msg_and_die(const char *s, ...);
void bb_simple_perror_msg_and_die(const char *s);
void bb_error_msg(const char *s, ...);
void xfunc_die(void);

ssize_t full_write(int fd, const void *buf, size_t len);
ssize_t safe_write(int fd, const void *buf, size_t count);
int xopen(const char *pathname, int flags);
void  xfstat(int fd, struct port_stat *stat_buf, const char *errmsg);
void xwrite(int fd, const void *buf, size_t count);

extern int recursive_action(const char *fileName, unsigned flags,
	int (*fileAction)(const char *fileName, struct port_stat* statbuf, void* userData, int depth),
	int (*dirAction)(const char *fileName, struct port_stat* statbuf, void* userData, int depth),
	void* userData, unsigned depth);

char* concat_subpath_file(const char *path, const char *f);
char* last_char_is(const char *s, int c);
char* concat_path_file(const char *path, const char *filename);
char*  xasprintf(const char *format, ...);
const char* strip_unsafe_prefix(const char *str);
int open_or_warn(const char *pathname, int flags);
char* safe_strncpy(char *dst, const char *src, size_t size);
void* xrealloc(void *ptr, size_t size);
char* xmalloc_readlink_or_warn(const char *path);
void readlink(char const *name);
void header_verbose_list(const file_header_t *file_header);
void header_list(const file_header_t *file_header);

void data_extract_all(archive_handle_t *archive_handle);
char filter_accept_reject_list(archive_handle_t *archive_handle);
char get_header_tar(archive_handle_t *archive_handle);
int bb_make_directory(char *path, long mode, int flags);
void bb_perror_msg_and_die(const char *s, ...);
int xopen3(const char *pathname, int flags, int mode);
void data_align(archive_handle_t *archive_handle, unsigned boundary);
void xread(int fd, void *buf, size_t count);
ssize_t full_read(int fd, void *buf, size_t len);
char* xstrdup(const char *s);
void overlapping_strcpy(char *dst, const char *src);
unsigned bb_strtou(const char *arg, char **endp, int base);
char* xstrndup(const char *s, int n);
#endif