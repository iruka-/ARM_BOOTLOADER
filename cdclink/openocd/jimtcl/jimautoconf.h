#ifndef _JIMAUTOCONF_H
#define _JIMAUTOCONF_H
/* #undef HAVE_ARPA_INET_H */
/* #undef HAVE_BACKTRACE */
/* #undef HAVE_CRT_EXTERNS_H */
/* #undef HAVE_DLFCN_H */
#define HAVE_DLOPEN_COMPAT 1
/* #undef HAVE_FORK */
/* #undef HAVE_GETADDRINFO */
/* #undef HAVE_GETEUID */
#define HAVE_GETTIMEOFDAY 1
#define HAVE_LONG_LONG 1
/* #undef HAVE_LSTAT */
#define HAVE_MKDIR_ONE_ARG 1
/* #undef HAVE_MKSTEMP */
/* #undef HAVE_NETDB_H */
/* #undef HAVE_NETINET_IN_H */
#define HAVE_OPENDIR 1
/* #undef HAVE_PIPE */
/* #undef HAVE_READLINK */
/* #undef HAVE_REALPATH */
/* #undef HAVE_REGCOMP */
/* #undef HAVE_SELECT */
/* #undef HAVE_SIGACTION */
/* #undef HAVE_SLEEP */
#define HAVE_STDLIB_H 1
/* #undef HAVE_STRPTIME */
/* #undef HAVE_SYSINFO */
/* #undef HAVE_SYSLOG */
#define HAVE_SYSTEM 1
/* #undef HAVE_SYS_SIGLIST */
/* #undef HAVE_SYS_SIGNAME */
/* #undef HAVE_SYS_SOCKET_H */
#define HAVE_SYS_STAT_H 1
#define HAVE_SYS_TYPES_H 1
/* #undef HAVE_SYS_UN_H */
/* #undef HAVE_TERMIOS_H */
/* #undef HAVE_UALARM */
#define HAVE_UNISTD_H 1
#define HAVE_USLEEP 1
/* #undef HAVE_UTIMES */
/* #undef HAVE_VFORK */
/* #undef HAVE_WAITPID */
#define HAVE_WINCONSOLE 1
#define HAVE_WINDOWS 1
/* #undef HAVE__NSGETENVIRON */
/* #undef JIM_INSTALL */
#define JIM_REFERENCES 1
#define JIM_REGEXP 1
#define JIM_STATICLIB 1
/* #undef JIM_UTF8 */
#define TCL_LIBRARY "/usr/local/lib/jim"
#define TCL_PLATFORM_OS "mingwmsvc"
#define TCL_PLATFORM_PATH_SEPARATOR ";"
#define TCL_PLATFORM_PLATFORM "windows"
#define USE_LINENOISE 1
#define jim_ext_aio 1
#define jim_ext_array 1
#define jim_ext_clock 1
#define jim_ext_eventloop 1
#define jim_ext_exec 1
#define jim_ext_file 1
#define jim_ext_glob 1
#define jim_ext_load 1
#define jim_ext_package 1
#define jim_ext_readdir 1
#define jim_ext_regexp 1
#define jim_ext_stdlib 1
#define jim_ext_tclcompat 1
#endif
