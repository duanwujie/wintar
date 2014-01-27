#ifndef __PLATFORM_H_
#define __PLATFORM_H_

#define MAXPATHLEN 4096

# include <string.h>
# include <stdlib.h>

#define _WIN32 1
#if _WIN32  /* **********************************************************Windows */
# include <stdio.h>
# include <stdlib.h>
# include <io.h>
# include <share.h>
# include <errno.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <stdarg.h>
# include <wchar.h>
# include <time.h>
# include <direct.h>
# include <Windows.h>

inline int getError(){return GetLastError();} 

#define port_strcpy strcpy_s
#define port_spiltpath _splitpath_s
#define port_strcat strcat_s
//#define port_open _sopen_s
#define port_open open
#define port_close _close
#define port_lseek _lseek
#define port_read _read
#define port_write _write
#define port_vsnprintf vsnprintf_s
#define port_strdup _strdup

#define port_localtime_r localtime
#define port_mkdir _mkdir
#define port_strtoll _strtoi64 
#define chown(a,b,c) -1

#ifdef _USE_32BIT_TIME_T
#define port_lstat _fstat
#define port_stat _fstat
#else
#define port_lstat _stat  
#define port_stat _stat  
#define port_fstat _fstat 
#endif

#ifndef S_ISDIR
# define S_ISDIR(mode) (((mode) & S_IFMT) == S_IFDIR)
#endif


# ifndef S_ISLNK
#  ifdef S_IFLNK
#   define S_ISLNK(m) (((m) & S_IFMT) == S_IFLNK)
#  else
#   define S_ISLNK(m) 0
#  endif
# endif

# ifndef S_ISSOCK
#  ifdef S_IFSOCK
#   define S_ISSOCK(m) (((m) & S_IFMT) == S_IFSOCK)
#  else
#   define S_ISSOCK(m) 0
#  endif
# endif

# ifndef S_ISREG
#  ifdef S_IFREG
#   define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
#  else
#   define S_ISREG(m) 0
#  endif
# endif


# ifndef S_ISCHR
#  ifdef S_IFCHR
#   define S_ISCHR(m) (((m) & S_IFMT) == S_IFCHR)
#  else
#   define S_ISCHR(m) 0
#  endif
# endif

# ifndef S_ISFIFO
#  ifdef S_IFIFO
#   define S_ISFIFO(m) (((m) & S_IFMT) == S_IFIFO)
#  else
#   define S_ISFIFO(m) 0
#  endif
# endif

#ifndef S_ISBLK
# ifdef S_IFBLK
#  define S_ISBLK(mode) (((mode) & S_IFMT) == S_IFBLK)
# else
#  define S_ISBLK(mode) 0
# endif
#endif

# ifndef S_ISDIR
#  ifdef S_IFDIR
#   define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#  else
#   define S_ISDIR(m) 0
#  endif
# endif

# ifndef S_ISLNK
#  ifdef S_IFLNK
#   define S_ISLNK(m) (((m) & S_IFMT) == S_IFLNK)
#  else
#   define S_ISLNK(m) 0
#  endif
# endif

#define ALIGNED(m) __declspec(align(m))

typedef long		off_t;
typedef unsigned long uoff_t;
typedef unsigned char uint8_t;
typedef	unsigned short	uid_t;
typedef	unsigned short	gid_t;
typedef int pid_t;
typedef char int8_t;

#ifndef __mode_t_defined
#define __mode_t_defined
typedef unsigned mode_t;
#endif

/* Some <sys/types.h> defines the macros. */
#ifdef major
# define GOT_MAJOR
#endif

#ifndef GOT_MAJOR
# define MSDOS 1
# if MSDOS
#  define major(device)		(device)
#  define minor(device)		(device)
#  define makedev(major, minor)	(((major) << 8) | (minor))
#  define GOT_MAJOR
# endif
#endif

#ifndef va_copy
#define va_copy(dst, src) ((void)((dst) = (src)))
#endif

#if defined(__INT_MAX__) && __INT_MAX__ == 2147483647
typedef int _ssize_t;
#else
typedef long _ssize_t;
#endif
typedef _ssize_t ssize_t;
#define STDIN_FILENO 0 
#define STDOUT_FILENO 1

#define O_CREAT _O_CREAT
#define O_RDONLY _O_RDONLY
#define O_WRONLY _O_WRONLY
#define O_TRUNC _O_TRUNC

#else   /* ******************************************Unix  linux like system */
# include <sys/stat.h> 
# include <fcntl.h>
# include <errno.h>

inline int getError(){return errno;}
#define port_strcpy strcpy
#define port_strcat strcat
#define port_open open
#define port_close _close
#define port_lseek lseek
#define port_read read
#define port_write write
#define port_lstat lstat
#define port_stat stat
#define port_vsnprintf vsnprintf
#define port_strdup strdup
#define port_localtime_r localtime_r
#define port_mkdir mkdir
#define port_strtoll strtoll
#define ALIGNED(m) __attribute__ ((__aligned__(m)))


#endif


/* **********************************************************Common defined */
typedef int smallint;
typedef unsigned smalluint;

#define CONFIG_FEATURE_COPYBUF_KB 4

#endif