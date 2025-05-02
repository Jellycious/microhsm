#ifndef _H_MICROHSM_CONFIG
#define _H_MICROHSM_CONFIG

// Check whether assertions are enabled
#ifndef MICROHSM_ASSERTIONS
    // Enable assertions by default
    #define MICROHSM_ASSERTIONS 1
#endif

#if MICROHSM_ASSERTIONS == 1
    #ifndef MICROHSM_ASSERT
        // Define assertion handler if no assertion handler is specified
        #include <assert.h>
        #define MICROHSM_ASSERT(expr) assert(expr)
    #endif
#endif

#endif /* _H_MICROHSM_CONFIG */
