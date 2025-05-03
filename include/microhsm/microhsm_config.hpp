#ifndef _H_MICROHSM_CONFIG
#define _H_MICROHSM_CONFIG

/* Assertions */
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

/* Tracing */
#ifndef MICROHSM_TRACING
    #define MICROHSM_TRACING 1
#endif

#if MICROHSM_TRACING == 1

    #ifndef MICROHSM_TRACE_ENTRY
        #include <iostream>
        #define MICROHSM_TRACE_ENTRY(id) std::cout << "entry(s" << id << ")" << std::endl
    #endif

    #ifndef MICROHSM_TRACE_EXIT
        #include <iostream>
        #define MICROHSM_TRACE_EXIT(id) std::cout << "exit(s" << id << ")" << std::endl
    #endif

    #ifndef MICOHSM_TRACE_DISPATCH_IGNORED
        #include <iostream>
        #define MICROHSM_TRACE_DISPATCH_IGNORED(event) std::cout << "ignored(e" << event << ")" << std::endl
    #endif

    #ifndef MICOHSM_TRACE_DISPATCH_MATCHED
        #include <iostream>
        #define MICROHSM_TRACE_DISPATCH_MATCHED(event, state) std::cout << "matched(e" << event << ", s" << state << ")" << std::endl
    #endif
#endif



#endif /* _H_MICROHSM_CONFIG */
