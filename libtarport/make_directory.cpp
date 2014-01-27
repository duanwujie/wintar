#include "libbb.h"

int bb_make_directory(char *path, long mode, int flags)
{
	mode_t cur_mask;
	mode_t org_mask;
	const char *fail_msg;
	char *s;
	char c;
	struct stat st;

	/* Happens on bb_make_directory(dirname("no_slashes"),...) */
	if (LONE_CHAR(path, '.'))
		return 0;

	org_mask = cur_mask = (mode_t)-1L;
	s = path;
	while (1) {
		c = '\0';

		if (flags & FILEUTILS_RECUR) {  /* Get the parent */
			/* Bypass leading non-'/'s and then subsequent '/'s */
			while (*s) {
				if (*s == '/') {
					do {
						++s;
					} while (*s == '/');
					c = *s; /* Save the current char */
					*s = '\0'; /* and replace it with nul */
					break;
				}
				++s;
			}
		}

		if (c != '\0') {
			/* Intermediate dirs: must have wx for user */
			if (cur_mask == (mode_t)-1L) { /* wasn't done yet? */
				mode_t new_mask;
				org_mask = umask(0);
				cur_mask = 0;
				/* Clear u=wx in umask - this ensures
				 * they won't be cleared on mkdir */
				new_mask = (org_mask & ~(mode_t)0300);
				//bb_error_msg("org_mask:%o cur_mask:%o", org_mask, new_mask);
				if (new_mask != cur_mask) {
					cur_mask = new_mask;
					umask(new_mask);
				}
			}
		} else {
			/* Last component: uses original umask */
			//bb_error_msg("1 org_mask:%o", org_mask);
			if (org_mask != cur_mask) {
				cur_mask = org_mask;
				umask(org_mask);
			}
		}
#if _WIN32
		if (port_mkdir(path) < 0) {
#else
		if (port_mkdir(path, 0777) < 0) {
#endif
			/* If we failed for any other reason than the directory
			 * already exists, output a diagnostic and return -1 */
			if ((errno != EEXIST && errno != EISDIR)
			 || !(flags & FILEUTILS_RECUR)
			 || ((stat(path, &st) < 0) || !S_ISDIR(st.st_mode))
			) {
				fail_msg = "create";
				break;
			}
			/* Since the directory exists, don't attempt to change
			 * permissions if it was the full target.  Note that
			 * this is not an error condition. */
			if (!c) {
				goto ret0;
			}
		}

		if (!c) {
			/* Done.  If necessary, update perms on the newly
			 * created directory.  Failure to update here _is_
			 * an error. */
			if ((mode != -1) && (chmod(path, mode) < 0)) {
				fail_msg = "set permissions of";
				if (flags & FILEUTILS_IGNORE_CHMOD_ERR) {
					flags = 0;
					goto print_err;
				}
				break;
			}
			goto ret0;
		}

		/* Remove any inserted nul from the path (recursive mode) */
		*s = c;
	} /* while (1) */

	flags = -1;
 print_err:
	bb_perror_msg("can't %s directory '%s'", fail_msg, path);
	goto ret;
 ret0:
	flags = 0;
 ret:
	//bb_error_msg("2 org_mask:%o", org_mask);
	if (org_mask != cur_mask)
		umask(org_mask);
	return flags;
}
