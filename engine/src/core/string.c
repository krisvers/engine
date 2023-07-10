#include <core/string.h>
#include <core/mem.h>
#include <defines.h>
#include <string.h>

u64 kstrlen(const char * str) {
    return strlen(str);
}

char * kstrdup(char * str) {
    u64 len = strlen(str);
    char * cpy = kmalloc(len + 1, MEMORY_TAG_STRING);
    kmemcpy(cpy, str, len + 1);
    return cpy;
}

u64 kstrcmp(const char * str1, const char * str2) {
    return strcmp(str1, str2);
}