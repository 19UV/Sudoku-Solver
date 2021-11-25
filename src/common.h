#pragma once

#define __STDC_WANT_LIB_EXT1__ 1
#include <string.h>

#define STR_EQ( a, b) (strcmp( a, b) == 0)

// TODO: Allow more compilers to use safe string functions
#if defined(_WIN32) || defined(__STDC_LIB_EXT1__)
	// This macro will allow Windows to use safe string copying
	#define STR_COPY( dest, src, max) (strcpy_s( dest, max, src))
#else
	// This will allow non-supported compilers to use the unsafe method
	#define STR_COPY( dest, src, max) (strcpy( dest, src))
#endif

typedef unsigned int uint;