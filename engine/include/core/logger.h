#ifndef KENGINE_CORE_LOGGER_H
#define KENGINE_CORE_LOGGER_H

#include <defines.h>

#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1
#ifdef DEBUG_FLAG
	#define LOG_DEBUG_ENABLED 1
	#define LOG_TRACE_ENABLED 1
#else
	#define LOG_DEBUG_ENABLED 0
	#define LOG_TRACE_ENABLED 0
#endif

typedef enum logLevelEnum {
	LOG_LEVEL_FATAL = 0,
	LOG_LEVEL_ERROR = 1,
	LOG_LEVEL_WARN = 2,
	LOG_LEVEL_INFO = 3,
	LOG_LEVEL_DEBUG = 4,
	LOG_LEVEL_TRACE = 5
} log_level_enum;

b8 log_init(void);
void log_deinit(void);

void KAPI log_set_logfile(char * filename);
void KAPI log_unset_logfile(void);

typedef void (* logger_func)(log_level_enum level, const char * message);

logger_func KAPI log_get_current_custom(void);
void KAPI log_hijack(logger_func lf);
void KAPI log_output(log_level_enum level, const char * message, ...);

#define KFATAL(message, ...) log_output(LOG_LEVEL_FATAL, message, ##__VA_ARGS__)
#define KERROR(message, ...) log_output(LOG_LEVEL_ERROR, message, ##__VA_ARGS__)

#if LOG_WARN_ENABLED == 1
	#define KWARN(message, ...) log_output(LOG_LEVEL_WARN, message, ##__VA_ARGS__)
#else
	#define KWARN(message, ...)
#endif

#if LOG_INFO_ENABLED == 1
	#define KINFO(message, ...) log_output(LOG_LEVEL_INFO, message, ##__VA_ARGS__)
#else
	#define KINFO(message, ...)
#endif

#define KLOG KINFO

#if LOG_DEBUG_ENABLED == 1
	#define KDEBUG(message, ...) log_output(LOG_LEVEL_DEBUG, message, ##__VA_ARGS__)
#else
	#define KDEBUG(message, ...)
#endif

#if LOG_TRACE_ENABLED == 1
	#define KTRACE(message, ...) log_output(LOG_LEVEL_TRACE, message, ##__VA_ARGS__)
#else
	#define KTRACE(message, ...)
#endif

#endif
