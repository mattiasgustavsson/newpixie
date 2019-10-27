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
//#define ASSERT( expression, message ) assert( expression )

#include "pixie/pixie.h"