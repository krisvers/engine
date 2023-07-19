#include <core/mem.h>
#include <core/assert.h>
#include <core/logger.h>
#include <platform/platform.h>

#include <stdio.h>
#include <string.h>

static const char * memory_tag_strings[MEMORY_TAG_MAX] = {
	"UNKNOWN      ",
	"ARRAY        ",
	"DYNAMIC_ARRAY",
	"DICTIONARY   ",
	"RING_QUEUE   ",
	"BST          ",
	"STRING       ",
	"APPLICATION  ",
	"JOB          ",
	"TEXTURE      ",
	"MATERIAL     ",
	"RENDERER     ",
	"GAME         ",
	"TRANSFORM    ",
	"ENTITY       ",
	"NODE         ",
	"SCENE        ",
	"CAMERA       ",
	"MESH         ",
	"FILE         ",
};

typedef struct memoryStats {
	u64 total_allocated;
	u64 tagged_allocations[MEMORY_TAG_MAX];
} memory_stats_t;

static memory_stats_t stats;

void memory_init(void) {
	platform_memzero(&stats, sizeof(stats));
}

void memory_deinit(void) {
	// TODO
}

void * kmalloc(u64 size, memory_tag_enum tag) {
	if (tag == MEMORY_TAG_UNKNOWN) {
		KWARN("[kmalloc(size, tag)]:");
		KWARN(" using MEMORY_TAG_UNKNOWN instead of providing one");
	}

	stats.total_allocated += size;
	stats.tagged_allocations[tag] += size;

	// TODO: mem alignment
	void * ptr = platform_malloc(size, FALSE);
	KASSERT(ptr != NULL);
	platform_memzero(ptr, size);
	return ptr;
}

void kfree(void * ptr, u64 size, memory_tag_enum tag) {
	KASSERT(ptr != NULL);
	if (tag == MEMORY_TAG_UNKNOWN) {
		KWARN("[kmalloc(size, tag)]:");
		KWARN(" using MEMORY_TAG_UNKNOWN instead of providing one");
	}

	stats.total_allocated -= size;
	stats.tagged_allocations[tag] -= size;

	// TODO: mem alignment
	platform_free(ptr, FALSE);
}

void * kmemzero(void * ptr, u64 size) {
	return platform_memzero(ptr, size);
}

void * kmemcpy(void * dst, void * src, u64 size) {
	return platform_memcpy(dst, src, size);
}

void * krmemcpy(void * dst, void * src, u64 nmemb, u64 size) {
	for (u64 i = 0; i < nmemb; ++i) {
		kmemcpy((void *) (((u64) dst) + (i * size)), (void *) (((u64) src) + (i * size)), size);
	}
	return dst;
}

void * kmemset(void * dst, i32 value, u64 size) {
	return platform_memset(dst, value, size);
}

#define TMP_MEM_KIB_SIZE (1024)
#define TMP_MEM_MIB_SIZE (1024 * TMP_MEM_KIB_SIZE)
#define TMP_MEM_GIB_SIZE (1024 * TMP_MEM_MIB_SIZE)

#define MEMORY_USAGE_DEFAULT_STRING "System memory usage:\n"
static char memory_usage_buffer[1024] = MEMORY_USAGE_DEFAULT_STRING;

char * memory_get_usage_cstr(void) {
	u64 offset = strlen(MEMORY_USAGE_DEFAULT_STRING);
	kmemzero(memory_usage_buffer + offset, 1024 - offset);

	for (u32 i = 0; i < MEMORY_TAG_MAX; ++i) {
		char unit[4] = "_iB";
		float amount = 1.0f;

		if (stats.tagged_allocations[i] >= TMP_MEM_GIB_SIZE) {
			unit[0] = 'G';
			amount = stats.tagged_allocations[i] / (float) TMP_MEM_GIB_SIZE;
		} else if (stats.tagged_allocations[i] >= TMP_MEM_MIB_SIZE) {
			unit[0] = 'M';
			amount = stats.tagged_allocations[i] / (float) TMP_MEM_MIB_SIZE;
		} else if (stats.tagged_allocations[i] >= TMP_MEM_KIB_SIZE) {
			unit[0] = 'K';
			amount = stats.tagged_allocations[i] / (float) TMP_MEM_KIB_SIZE;
		} else {
			unit[0] = ' '; unit[1] = ' '; unit[2] = 'B'; unit[3] = '\0';
			amount = (float) stats.tagged_allocations[i];
		}

		i32 length = snprintf(memory_usage_buffer + offset, 1024 - offset, " %s    %.2f\t%s\n", memory_tag_strings[i], amount, unit);
		offset += length;
	}
	return memory_usage_buffer;
}
