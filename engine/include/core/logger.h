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

void KAPI log_set_logfile(char * filename, log_level_enum level);
void KAPI log_unset_logfile(void);

typedef void (* logger_func)(log_level_enum level, const char * message);
typedef void (* logger_length_func)(log_level_enum level, u8 * message, u64 length);

logger_func KAPI log_get_current_custom(void);
void KAPI log_hijack(logger_func lf);
logger_length_func KAPI log_get_current_length_custom(void);
void KAPI log_length_hijack(logger_length_func lf);
void KAPI log_output(log_level_enum level, const char* message, ...);
void KAPI log_write(log_level_enum level, u8 * message, u64 length);

#ifdef DEBUG_FLAG
	#define KLOGGER_GET_FILE() __FILE__":%llu "
	#define KLOGGER_GET_LINE __LINE__
	#define KLOGGER_MESSAGE(msg) KLOGGER_GET_FILE() msg, KLOGGER_GET_LINE
#else
	#define KLOGGER_GET_FILE()
	#define KLOGGER_GET_LINE
	#define KLOGGER_MESSAGE(msg) msg
#endif

#define KFATAL(message, ...) log_output(LOG_LEVEL_FATAL, KLOGGER_MESSAGE(message), ##__VA_ARGS__)
#define KERROR(message, ...) log_output(LOG_LEVEL_ERROR, KLOGGER_MESSAGE(message), ##__VA_ARGS__)

#if LOG_WARN_ENABLED == 1
	#define KWARN(message, ...) log_output(LOG_LEVEL_WARN, KLOGGER_MESSAGE(message), ##__VA_ARGS__)
#else
	#define KWARN(message, ...)
#endif

#if LOG_INFO_ENABLED == 1
	#define KINFO(message, ...) log_output(LOG_LEVEL_INFO, KLOGGER_MESSAGE(message), ##__VA_ARGS__)
	#define KWRITE(message, len) log_write(LOG_LEVEL_INFO, message, len)
#else
	#define KINFO(message, ...)
	#define KWRITE(message, len)
#endif

#define KLOG KINFO

#if LOG_DEBUG_ENABLED == 1
	#define KDEBUG(message, ...) log_output(LOG_LEVEL_DEBUG, KLOGGER_MESSAGE(message), ##__VA_ARGS__)
#else
	#define KDEBUG(message, ...)
#endif

#if LOG_TRACE_ENABLED == 1
	#define KTRACE(message, ...) log_output(LOG_LEVEL_TRACE, KLOGGER_MESSAGE(message), ##__VA_ARGS__)
#else
	#define KTRACE(message, ...)
#endif

#endif
