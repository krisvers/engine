#ifndef KENGINE_CORE_MEM_H
#define KENGINE_CORE_MEM_H

#include <defines.h>

typedef enum memoryTagEnum {
	MEMORY_TAG_UNKNOWN,
	MEMORY_TAG_ARRAY,
	MEMORY_TAG_DYNAMIC_ARRAY,
	MEMORY_TAG_DICT,
	MEMORY_TAG_RING_QUEUE,
	MEMORY_TAG_BST,
	MEMORY_TAG_STRING,
	MEMORY_TAG_APPLICATION,
	MEMORY_TAG_JOB,
	MEMORY_TAG_TEXTURE,
	MEMORY_TAG_MATERIAL,
	MEMORY_TAG_RENDERER,
	MEMORY_TAG_GAME,
	MEMORY_TAG_TRANSFORM,
	MEMORY_TAG_ENTITY,
	MEMORY_TAG_NODE,
	MEMORY_TAG_SCENE,

	MEMORY_TAG_MAX,
} memory_tag_enum;

void memory_init(void);
void memory_deinit(void);

void KAPI * kmalloc(u64 size, memory_tag_enum tag);
void KAPI kfree(void * ptr, u64 size, memory_tag_enum tag);
void KAPI * kmemzero(void * ptr, u64 size);
void KAPI * kmemcpy(void * dst, void * src, u64 size);
void KAPI * kmemset(void * dst, i32 value, u64 size);

char KAPI * memory_get_usage_cstr(void);

#endif
