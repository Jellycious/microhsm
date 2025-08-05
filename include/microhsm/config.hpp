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
    // Disable assertions by default
    #define MICROHSM_ASSERTIONS 0
#endif

#if MICROHSM_ASSERTIONS == 1
    /*
     * Assertions enabled.
     *
     * Provide an assertion hook `MICROHSM_ASSERT(expr)`
     * The assertion should evaluate the expression `expr` and
     * prevent further execution if `expr` evaluates to zero.
     */
    #ifndef MICROHSM_ASSERT
        #error Assertions enabled, but no assertion hook provided
    #endif
#endif

/* Tracing */
#ifndef MICROHSM_TRACING
    #define MICROHSM_TRACING 0
#endif

#if MICROHSM_TRACING == 1

    /*
     * Tracing enabled.
     *
     * Tracing is used to trace a path during event dispatches.
     * The hooks will be called during specific moments when
     * transitioning between states.
     *
     * The following hooks must be provided by the user:
     * `MICROHSM_TRACE_ENTRY(id)` - Called upon entry of state with ID `id`
     * `MICROHSM_TRACE_EXIT(id)` - Called upon exit of state with ID `id`
     * `MICROHSM_TRACE_DISPATCH_IGNORED(event)` - Called when `event` did not match any transition
     * `MICROHSM_TRACE_DISPATCH_MATCHED(event, id)` - Called when `event` matched transition from state with ID `id`
     */

    #ifndef MICROHSM_TRACE_ENTRY
        #error Tracing enabled, but no MICROHSM_TRACE_ENTRY hook provided
    #endif

    #ifndef MICROHSM_TRACE_EXIT
        #error Tracing enabled, but no MICROHSM_TRACE_EXIT hook provided
    #endif

    #ifndef MICROHSM_TRACE_DISPATCH_IGNORED
        #error Tracing enabled, but no MICROHSM_TRACE_DISPATCH_IGNORED hook provided
    #endif

    #ifndef MICROHSM_TRACE_DISPATCH_MATCHED
        #error Tracing enabled, but no MICROHSM_TRACE_DISPATCH_MATCHED hook provided
    #endif
#endif

#endif /* _H_MICROHSM_CONFIG */
