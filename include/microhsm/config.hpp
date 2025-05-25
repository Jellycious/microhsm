/**
 * @file config.hpp
 * @brief MicroHSM configuration options
 *
 * @author Jelle Meijer
 * @date 2025-05-23
 */

#ifndef _H_MICROHSM_CONFIG
#define _H_MICROHSM_CONFIG

#ifdef MICROHSM_CUSTOM_CONFIG
    #include <microhsm_config.hpp>
#endif

/* Assertions */
#ifndef MICROHSM_ASSERTIONS
    // Enable assertions by default
    #define MICROHSM_ASSERTIONS 1
#endif

#if MICROHSM_ASSERTIONS == 1
    #ifndef MICROHSM_ASSERT
        // Define assertion handler if no assertion handler is specified
        #include <assert.h>
        /**
         * @brief Assertion handler
         * @param expr Expression to evaluate
         */
        #define MICROHSM_ASSERT(expr) assert(expr)
    #endif
#endif

/* Tracing */
#ifndef MICROHSM_TRACING
    #define MICROHSM_TRACING 0
#endif

#if MICROHSM_TRACING == 1

    #ifndef MICROHSM_TRACE_STATE_NAME
        /**
         * @brief Convert state ID to string
         * @param id State ID
         * @return String of state ID
         */
        #define MICROHSM_TRACE_STATE_NAME(id) id
    #endif

    #ifndef MICROHSM_TRACE_EVENT_NAME
        /**
         * @brief Convert event ID to string
         * @param event Event ID
         * @return String of event ID
         */
        #define MICROHSM_TRACE_EVENT_NAME(event) event
    #endif

    #ifndef MICROHSM_TRACE_ENTRY
        #include <iostream>
        /**
         * @brief Called when perform entry behavior of state
         * @param id State ID
         */
        #define MICROHSM_TRACE_ENTRY(id) std::cout << "ENTRY," << MICROHSM_TRACE_STATE_NAME(id) << std::endl
    #endif

    #ifndef MICROHSM_TRACE_EXIT
        #include <iostream>
        /**
         * @brief Called when perform exit behavior of state
         * @param id State ID
         */
        #define MICROHSM_TRACE_EXIT(id) std::cout << "EXIT," << MICROHSM_TRACE_STATE_NAME(id) << std::endl
    #endif

    #ifndef MICROHSM_TRACE_DISPATCH_IGNORED
        #include <iostream>
        /**
         * @brief Called when an event did not match any transitions
         * @param event Event ID
         */
        #define MICROHSM_TRACE_DISPATCH_IGNORED(event) std::cout << "IGNORED," << MICROHSM_TRACE_EVENT_NAME(event) << std::endl
    #endif

    #ifndef MICROHSM_TRACE_DISPATCH_MATCHED
        #include <iostream>
        /**
         * @brief Called when an event matched a transition on a state
         * @param event Event ID
         * @param id State ID
         */
        #define MICROHSM_TRACE_DISPATCH_MATCHED(event, id) std::cout << "MATCH," << MICROHSM_TRACE_EVENT_NAME(event) << "," << MICROHSM_TRACE_STATE_NAME(id) << std::endl
    #endif

    #ifndef MICROHSM_TRACE_MESSAGE
        #include <iostream>
        /**
         * @brief Use to send a random trace message
         * @param msg Message string
         */
        #define MICROHSM_TRACE_MESSAGE(msg) std::cout << "MESSAGE," << msg<< std::endl;
    #endif
#endif

#endif /* _H_MICROHSM_CONFIG */
