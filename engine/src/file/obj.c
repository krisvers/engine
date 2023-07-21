#include <file/obj.h>
#include <core/file.h>
#include <core/mem.h>
#include <core/logger.h>
#include <core/string.h>
#include <containers/dynarray.h>

// TODO: remove this (just used for temporary string formatting)
#include <stdio.h>

typedef enum objTokenEnum {
	TOKEN_NONE = -3,
	TOKEN_DISREGARD = -2,
	TOKEN_NAN = -1,
	TOKEN_UNKNOWN = 0,
	TOKEN_INT,
	TOKEN_FLOAT,
	TOKEN_VERTEX,
	TOKEN_FACE,

	TOKEN_MAX,
} obj_token_enum;

typedef struct objParseContext {
	obj_token_enum token_type;
	obj_token_enum token_operation;
	u64 token_len;
	char * token_start;
} obj_parse_context_t;

obj_parse_context_t context = {
	TOKEN_NONE, TOKEN_NONE, 0, NULL
};

static b8 obj_skip_check(char c) {
	switch (c) {
		case '#':
			return TRUE;
		case 's':
			return TRUE;
		case 'o':
			return TRUE;
	}
	
	return FALSE;
}

static b8 obj_delimitter_check(char c) {
	switch (c) {
		case ' ':
			return TRUE;
		case '\t':
			return TRUE;
		case '\0':
			return TRUE;
		case '\n':
			return TRUE;
	}

	return FALSE;
}

static const char token_strings[][2] = {
	"__",
	"II",
	"FF",
	"v_",
	"f_",
	"__",
};

static obj_token_enum obj_token_number_type(void) {
	const u8 float_type = 1;
	const u8 int_type = 2;
	const u8 nan_type = 3;
	u8 type = int_type;
	for (u64 i = 0; i < context.token_len; ++i) {
		if (context.token_start[i] == '.' || context.token_start[i] == '-') {
			type = float_type;
			continue;
		}
		if (context.token_start[i] < '0' || context.token_start[i] > '9') {
			type = nan_type;
			break;
		}
	}

	if (type == nan_type) {
		return TOKEN_NAN;
	}
	if (type == int_type) {
		return TOKEN_INT;
	}
	if (type == float_type) {
		return TOKEN_FLOAT;
	}
	return TOKEN_UNKNOWN;
}

static obj_token_enum obj_token_type(void) {
	if (context.token_len == 0) {
		return TOKEN_DISREGARD;
	}

	obj_token_enum type = obj_token_number_type();
	if (type == TOKEN_UNKNOWN) {
		KERROR("[obj_token_type()]");
		KERROR("unknown token:");
		KWRITE(context.token_start, context.token_len);
		return TOKEN_UNKNOWN;
	}
	if (type != TOKEN_NAN) {
		return type;
	}
	for (u64 i = TOKEN_VERTEX; i < TOKEN_MAX; ++i) {
		u64 len = (context.token_len > 2) ? 2 : context.token_len;
		if (len > 2) {
			KERROR("[obj_token_type()]");
			KERROR("token length should not be longer than 2 if it is not number, token:");
			KWRITE(context.token_start, context.token_len);
			return TOKEN_UNKNOWN;
		}
		if (kmemcmp(context.token_start, token_strings[i], len) == 0) {
			return i;
		}
	}

	KERROR("[obj_token_type()]");
	KERROR("unknown token:");
	KWRITE(context.token_start, context.token_len);
	return TOKEN_UNKNOWN;
}

static b8 obj_process_token(void) {
	obj_token_enum next = obj_token_type();

	if (next == TOKEN_NAN || next == TOKEN_NONE || next == TOKEN_UNKNOWN) {
		KERROR("[obj_process_token()]");
		KERROR("cannot process invalid token");
		char buffer[30];
		char len_buf[2];
		snprintf(len_buf, 2, "%llu", context.token_len);
		snprintf(buffer, 30, "%i: %c%c%s%c", next, '%', '-', len_buf, 's');
		log_output(LOG_LEVEL_INFO, buffer, context.token_start);
		return FALSE;
	}

	{
		char buffer[30];
		char len_buf[2];
		snprintf(len_buf, 2, "%llu", context.token_len);
		snprintf(buffer, 30, "%i: %c%c%s%c", next, '%', '-', len_buf, 's');
		log_output(LOG_LEVEL_INFO, buffer, context.token_start);
	}

	return TRUE;
}

void obj_parse(file_t * file, mesh_t * mesh) {
	// step one: split into cstring dynarray for each new line
	dynarray_t * lines = dynarray_create(char *);
	char * buffer = kmalloc(file->length + 1, MEMORY_TAG_STRING);
	kmemcpy(buffer, file->buffer, file->length);
	u64 line_num = 0;
	{
		u64 last = 0;
		for (u64 i = 0; i < file->length; ++i) {
			if (buffer[i] == '\n') {
				dynarray_push(lines, ((char *) buffer + last), char *);
				buffer[i] = '\0';
				++line_num;
				last = i + 1;
			}
		}
	}

	// step two: parse valid lines
	context.token_start = NULL;
	context.token_len = 0;
	for (u64 i = 0; i < line_num; ++i) {
		u64 len = kstrlen(dynarray_at(lines, i, char *)) + 1;
		context.token_start = dynarray_at(lines, i, char *);
		context.token_len = 0;
		for (u64 j = 0; j <= len; ++j) {
			if (obj_skip_check(dynarray_at(lines, i, char *)[j])) {
				goto next_line_label;
			}
			if (obj_delimitter_check(dynarray_at(lines, i, char *)[j])) {
				// process token
				if (!obj_process_token()) {
					goto deinit_lable;
				}

				// reset token
				context.token_len = 0;
				context.token_start = &dynarray_at(lines, i, char *)[j] + 1;
			} else {
				++context.token_len;
			}
		}
next_line_label:;
	}
	 
	// step three: deinit

deinit_lable:;
	// deinit
	dynarray_destroy(lines);
	kfree(buffer, file->length + 1, MEMORY_TAG_STRING);
}