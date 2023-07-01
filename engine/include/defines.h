#ifndef KENGINE_DEFINES_H
#define KENGINE_DEFINES_H

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long int u64;

typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long int i64;

typedef float f32;
typedef double f64;

typedef int b32;
typedef char b8;

#define TRUE 1
#define FALSE 0

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
	#define KPLATFORM_WINDOWS 1
	#define KPLATFORM_STRING "Windows"
	#ifndef _WIN64
		#error "64-bit windows required!"
	#endif
#elif defined(__linux__) || defined(__gnu_linux__)
	#define KPLATFORM_LINUX 1
	#define KPLATFORM_STRING "Linux"
	#if defined(__ANDROID__)
		#define KPLATFORM_ANDROID 1
		#define KPLATFORM_STRING "Android"
	#endif
#elif defined(__unix__)
	#define KPLATFORM_UNIX 1
	#define KPLATFORM_STRING "UNIX"
#elif defined(_POSIX_VERSION)
	#define KPLATFORM_POSIX 1
#elif __APPLE__
	#define KPLATFORM_APPLE 1
	#define KPLATFORM_STRING "Apple"
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR
		#define KPLATFORM_IOS 1
		#define KPLATFORM_IOS_SIMULATOR 1
		#define KPLATFORM_STRING "iOS Simulator"
	#elif TARGET_OS_IPHONE
		#define KPLATFORM_IOS 1
		#define KPLATFORM_STRING "iOS"
	#elif TARGET_OS_MAC
		#define KPLATFORM_STRING "macOS"
	#else
		#error "Unknown Apple platform"
	#endif
#else
	#error "Unknown platform"
#endif

#ifdef KEXPORT
	#ifdef KWINDOWS_DLL
		#define KAPI __declspec(dllexport)
	#else
		#define KAPI __attribute__((visibility("default")))
	#endif
#else
	#ifdef KWINDOWS_DLL
		#define KAPI __declspec(dllimport)
	#else
		#define KAPI
	#endif
#endif

#endif
