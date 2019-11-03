/*
    Pixie include file wrapper 
    --------------------------

    Used wherever pixie/pixie.h would be included, 
    to ensure we provide consistent configuration 
    options (#defines) for every instance it is 
    included.
*/

//#define PIXIE_NO_MATH
//#define PIXIE_NO_NAMESPACE
//#define PIXIE_NO_MAIN
//#define PIXIE_NO_BUILD
//#define PIXIE_ASSERT_IN_RELEASE_BUILD
//#define PIXIE_MAX_STRING_LENGTH 256

//#define _CRT_NONSTDC_NO_DEPRECATE 
//#define _CRT_SECURE_NO_WARNINGS
//#include <assert.h>
//#define PIXIE_ASSERT( expression, message ) assert( expression )

//#define _CRT_NONSTDC_NO_DEPRECATE 
//#define _CRT_SECURE_NO_WARNINGS
//#include <stdint.h>
//#define PIXIE_I8 int8_t
//#define PIXIE_I16 int16_t
//#define PIXIE_I32 int32_t
//#define PIXIE_I64 int64_t
//#define PIXIE_U8 uint8_t
//#define PIXIE_U16 uint16_t
//#define PIXIE_U32 uint32_t
//#define PIXIE_U64 uint64_t

#include "pixie/pixie.h"