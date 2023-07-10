#ifndef KENGINE_CORE_STRING_H
#define KENGINE_CORE_STRING_H

#include <defines.h>

u64 KAPI kstrlen(const char * str);
char KAPI * kstrdup(char * str);
u64 KAPI kstrcmp(const char * str1, const char * str2);

#endif