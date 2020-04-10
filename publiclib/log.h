#ifndef LOG_H
#define LOG_H
#include "publiclib_global.h"

typedef void (*log_LockFn)(void *udata, int lock);

enum { LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_FATAL };

#define log_trace(...) log_log(LOG_TRACE, __FUNCTION__, __LINE__, __VA_ARGS__)
#define log_debug(...) log_log(LOG_DEBUG, __FUNCTION__, __LINE__, __VA_ARGS__)
#define log_info(...)  log_log(LOG_INFO,  __FUNCTION__, __LINE__, __VA_ARGS__)
#define log_warn(...)  log_log(LOG_WARN,  __FUNCTION__, __LINE__, __VA_ARGS__)
#define log_error(...) log_log(LOG_ERROR, __FUNCTION__, __LINE__, __VA_ARGS__)
#define log_fatal(...) log_log(LOG_FATAL, __FUNCTION__, __LINE__, __VA_ARGS__)

void PUBLICLIB_EXPORT log_set_udata(void *udata);
void PUBLICLIB_EXPORT log_set_lock(log_LockFn fn);
void PUBLICLIB_EXPORT log_set_fp(const char * file);
void PUBLICLIB_EXPORT log_set_level(int level);
void PUBLICLIB_EXPORT log_set_console(bool console);
void PUBLICLIB_EXPORT log_set_quiet(int enable);

void PUBLICLIB_EXPORT log_log(int level, const char *func, int line, const char *fmt, ...);

#endif
