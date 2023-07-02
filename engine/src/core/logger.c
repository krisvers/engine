#include <core/logger.h>
#include <platform/platform.h>
#include <defines.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

static logger_func log_custom_output;
static void log_output_file_default(log_level_enum level, const char * message);
static logger_func log_output_file = log_output_file_default;
static b8 log_to_files = FALSE;
static char * logfile;
static FILE * logfp;
static const char * log_level_strings[6] = {
	"[FATAL]", "[ERROR]", "[WARN]", "[INFO]", "[DEBUG]", "[TRACE]"
};

b8 KAPI log_init(void) {
	// init
	return TRUE;
}

void KAPI log_deinit(void) {
	// cleanup
	if (logfp != NULL) { log_unset_logfile(); }
}

void KAPI log_set_logfile(char * filename) {
	logfile = filename;
	logfp = fopen(filename, "w");
	if (logfp == NULL) {
		KFATAL("[log_set_logfile()]:");
		KFATAL(" can't create file %s", filename);
		log_deinit();
		abort();
	}
	log_to_files = TRUE;
	time_t raw;
	struct tm * time_info;
	time(&raw);
	time_info = localtime(&raw);
	fprintf(logfp, "Log File Creation Time: %s\n", asctime(time_info));
	fclose(logfp);
	logfp = fopen(filename, "a");
}

void KAPI log_unset_logfile(void) {
	logfile = NULL;
	fclose(logfp);
	log_to_files = FALSE;
}

logger_func KAPI log_get_current_custom(void) {
	return log_custom_output;
}

void KAPI log_hijack(logger_func lf) {
	log_custom_output = lf;
}

void KAPI log_output(log_level_enum level, const char * message, ...) {
	__builtin_va_list arg_ptr;
	va_start(arg_ptr, message);

	char format_buffer[1024];
	memset(format_buffer, 0, 1024);

	vsnprintf(format_buffer, 1024, message, arg_ptr);
	va_end(arg_ptr);

	if (log_custom_output != NULL) {
		log_custom_output(level, format_buffer);
	}

	if (log_to_files && log_output_file != NULL) {
		log_output_file(level, format_buffer);
	}

	size_t last = 0;
	for (size_t i = 0; format_buffer[i] != '\0'; ++i) {
		if (format_buffer[i] == '\n') {
			format_buffer[i] = '\0';
			platform_console_write(log_level_strings[level], level);
			platform_console_write(": ", level);
			platform_console_write(&format_buffer[last], level);
			platform_console_write("\n", 7);
			last = i + 1;
		}

		if (i >= 1024) {
			format_buffer[1023] = '\0';
			break;
		}
	}
	if (last == 0) {
		platform_console_write(log_level_strings[level], level);
		platform_console_write(": ", level);
		platform_console_write(&format_buffer[last], level);
		platform_console_write("\n", 7);
	}
}

static void log_output_file_default(log_level_enum level, const char * formatted_message) {
	char * msg = (char *) formatted_message;

	size_t last = 0;
	for (size_t i = 0; msg[i] != '\0'; ++i) {
		if (msg[i] == '\n') {
			msg[i] = '\0';
			fprintf(logfp, "%s: %s\n", log_level_strings[level], &msg[last]);
			last = i + 1;
		}
	}
	if (last == 0) {
		fprintf(logfp, "%s: %s\n", log_level_strings[level], msg);
	}
}
