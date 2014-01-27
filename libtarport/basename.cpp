
#include "platform.h"

#if _WIN32
static char *win_basename(const char * path)
{
	static char bname[MAXPATHLEN];
	char drive[MAXPATHLEN];
	char dirname[MAXPATHLEN];
	char ext[MAXPATHLEN];
	if (path == NULL || *path == '\0') {
		(void)port_strcpy(bname, ".");
		return(bname);
	}
	port_spiltpath(path,drive,dirname,bname,ext);
	port_strcat(bname,ext);
	return (bname);
}
#else
static char *openbsd_basename(const char * path)
{
	static char bname[MAXPATHLEN];
	register const char *endp, *startp;

	/* Empty or NULL string gets treated as "." */
	if (path == NULL || *path == '\0') {
		(void)strcpy(bname, ".");
		return(bname);
	}

	/* Strip trailing slashes */
	endp = path + strlen(path) - 1;
	while (endp > path && *endp == '/')
		endp--;

	/* All slashes becomes "/" */
	if (endp == path && *endp == '/') {
		(void)strcpy(bname, "/");
		return(bname);
	}

	/* Find the start of the base */
	startp = endp;
	while (startp > path && *(startp - 1) != '/')
		startp--;

	if (endp - startp + 1 > sizeof(bname)) {
		errno = ENAMETOOLONG;
		return(NULL);
	}
	(void)strncpy(bname, startp, endp - startp + 1);
	bname[endp - startp + 1] = '\0';
	return(bname);
}
#endif

char * port_basename(const char * path)
{
#if _WIN32
	return win_basename(path);
#else
	return openbsd_basename(path);
#endif
}
