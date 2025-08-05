#ifndef MICROHSM_TESTS_CUSTOM_CONFIG
#define MICROHSM_TESTS_CUSTOM_CONFIG


// Enable assertions
#define MICROHSM_ASSERTIONS 1

#include <assert.h>

// Provide assertion hook
#define MICROHSM_ASSERT(expr) assert(expr)


// Enable tracing
#define MICROHSM_TRACING 1

#include <iostream>

inline const char* get_event_name_(unsigned int event)
{
    switch(event) {
        case 0:
            return "eANONYMOUS";
        case 1:
            return "eA";
        case 2:
            return "eB";
        case 3:
            return "eC";
        case 4:
            return "eD";
        case 5:
            return "eE";
        case 6:
            return "eF";
        case 7:
            return "eG";
        default:
            return "eUNKOWN";
    }
}

inline const char* get_state_name_(unsigned int state)
{
    switch(state) {
        case 0:
            return "S";
        case 1:
            return "S1";
        case 2:
            return "S2";
        case 3:
            return "S21";
        case 4:
            return "S22";
        case 5:
            return "U";
        case 6:
            return "V";
        case 7:
            return "X";
        case 8:
            return "S";
        case 9:
            return "S1";
        case 10:
            return "S2";
        case 11:
            return "S21";
        case 12:
            return "S22";
        case 13:
            return "U";
        case 14:
            return "H";
        case 15:
            return "H(shallow history)";
        case 16:
            return "H(deep history)";
        case 17:
            return "H1";
        case 18:
            return "H11";
        case 19:
            return "H12";
        case 20:
            return "H2";
        case 21:
            return "H21";
        case 22:
            return "H22";
        case 23:
            return "I";
        default:
            return "eUNKOWN";
    }

}

// Define tracing hooks
#define MICROHSM_TRACE_ENTRY(id) std::cout << "ENTRY," << get_state_name_(id) << std::endl
#define MICROHSM_TRACE_EXIT(id) std::cout << "EXIT," << get_state_name_(id) << std::endl
#define MICROHSM_TRACE_DISPATCH_IGNORED(event) std::cout << "IGNORED," << get_event_name_(event) << std::endl
#define MICROHSM_TRACE_DISPATCH_MATCHED(event, id) std::cout << "MATCH," << get_event_name_(event) << "," << get_state_name_(id) << std::endl

#define MICROHSM_TEST_MESSAGE(msg) std::cout << "MESSAGE," << msg << std::endl;

#endif
