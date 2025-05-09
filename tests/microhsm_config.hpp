#ifndef MICROHSM_TESTS_CUSTOM_CONFIG
#define MICROHSM_TESTS_CUSTOM_CONFIG

// Enable tracing
#define MICROHSM_TRACING 1

inline const char* get_state_name_(unsigned int id) 
{
    switch(id) {
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
        default:
            return "UNKOWN";
    }
}

inline const char* get_event_name_(unsigned int event)
{
    switch(event) {
        case 0:
            return "E0"; 
        case 1:
            return "EA"; 
        case 2:
            return "EB"; 
        case 3:
            return "EC"; 
        case 4:
            return "ED"; 
        case 5:
            return "EE"; 
        case 6:
            return "EF"; 
        case 7:
            return "EG"; 
        default:
            return "UNKOWN";
    }

}
/**
 * @brief Convert state ID to string
 * @param id State ID
 * @return String of state ID
 */
#define MICROHSM_TRACE_STATE_NAME(id) get_state_name_(id)

/**
 * @brief Convert event ID to string
 * @param event Event ID
 * @return String of event ID
 */
#define MICROHSM_TRACE_EVENT_NAME(event) get_event_name_(event)

#endif
