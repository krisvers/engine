#ifndef KENGINE_CORE_ASSERT_H
#define KENGINE_CORE_ASSERT_H

#include <defines.h>

#define KASSERTIONS_ENABLED

#ifdef KASSERTIONS_ENABLED
	#ifdef _MSC_VER
		#include <intrin.h>
		#define DEBUG_BREAK() __debugbreak()
	#else
		#define DEBUG_BREAK() __builtin_trap()
	#endif

	void KAPI report_assertion_failure(const char * expression, const char * message, const char * file, u32 line);

	#define KASSERT(expr)											\
	{																\
		if (expr) {}												\
		else {														\
			report_assertion_failure(#expr, "", __FILE__, __LINE__);\
			DEBUG_BREAK();											\
		}															\
	}

	#define KASSERT_MSG(expr, msg)										\
	{																	\
		if (expr) {}													\
		else {															\
			report_assertion_failure(#expr, msg, __FILE__, __LINE__);	\
			DEBUG_BREAK();												\
		}																\
	}

	#ifdef DEBUG_FLAG
		#define KASSERT_DEBUG(expr)										\
		{																\
			if (expr) {}												\
			else {														\
				report_assertion_failure(#expr, "", __FILE__, __LINE__);\
				DEBUG_BREAK();											\
			}															\
		}
	#else
		#define KASSERT_DEBUG(expr)
	#endif
#else
	#define KASSERT(expr)
	#define KASSERT_MSG(expr, msg)
	#define KASSERT_DEBUG(expr)
#endif

#endif
