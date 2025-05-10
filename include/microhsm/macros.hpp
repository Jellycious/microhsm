#ifndef _H_MICROHSM_MACROS
#define _H_MICROHSM_MACROS

#include <microhsm/State.hpp>
#include <microhsm/HSM.hpp>

#define HSM_EXPAND_(x) x

/**
 * @brief Create event enumeration
 * @param enum_name Name of enum
 * @param ... list of event names
 */
#define HSM_CREATE_EVENT_LIST(enum_name, ...)                       \
    enum enum_name: unsigned int {                                  \
        enum_name##_ANONYMOUS = 0,                                  \
        ##__VA_ARGS__                                               \
    };

/**
 * @brief Create state list
 * Will create an enumerator with the provide states
 * The first state will be assigned '0'
 * At the end an identifier '`enum_name`_COUNT' will be placed
 * this identifiers represents the total number of states.
 * @param enum_name Enum type identifier
 * @param first_name First state identifier
 * @param ... Other state identifiers
 */
#define HSM_CREATE_STATE_LIST(enum_name, first_state, ...)          \
    enum enum_name: unsigned int {                                  \
        first_state = 0,                                            \
        ##__VA_ARGS__,                                              \
        enum_name##_COUNT                                           \
    };

/// @brief Declare state entry function
#define HSM_DECLARE_STATE_ENTRY()                                   \
            void entry(void* ctx) override;

/// @brief Declare state exit function
#define HSM_DECLARE_STATE_EXIT()                                    \
            void exit(void* ctx) override;

/// @brief Declare state initialization function
#define HSM_DECLARE_STATE_INIT()                                    \
            void init_(void* ctx) override;

/// @brief Declare arbitrary state member variable or function
#define HSM_DECLARE_MEMBER(member)                                  \
            member;

/**
 * @brief Declare state
 * @param class_name Class name of state    
 */
#define HSM_DECLARE_STATE(class_name, ...)                                          \
    class class_name : public State {                                               \
        public:                                                                     \
            class_name(unsigned int id, State* parentState, State* initialState) :  \
            State(id, parentState, initialState) {};                                \
                                                                                    \
            bool match(unsigned int event, sTransition* t, void* ctx) override;     \
            __VA_ARGS__                                                             \
    };

/**
 * @brief Declare base state
 * A base state can be used as a base class for other states
 * The difference between `HSM_DECLARE_STATE` and `HSM_DECLARE_BASE_STATE`
 * is that `HSM_DECLARE_BASE_STATE` does not declare a match function
 * @param base_class Class name of state    
 */
#define HSM_DECLARE_BASE_STATE(base_class, ...)                                     \
    class base_class : public State {                                               \
        public:                                                                     \
            base_class(unsigned int id, State* parentState, State* initialState) :  \
            State(id, parentState, initialState) {};                                \
            __VA_ARGS__                                                             \
    };

/**
 * @brief Declare state from base class
 * @note Base class constructor must be a constructor of form `base_class(`id`, `parent`, `initial`)
 * @param base_class Class name of base class
 * @param class_name Class name of state    
 */
#define HSM_DECLARE_STATE_FROM_BASE(class_name, base_class, ...)                    \
    class class_name : public base_class {                                          \
        public:                                                                     \
            class_name(unsigned int id, State* parentState, State* initialState) :  \
            base_class(id, parentState, initialState) {};                           \
                                                                                    \
            bool match(unsigned int event, sTransition* t, void* ctx) override;     \
            __VA_ARGS__                                                             \
    };

/// @brief Define state entry function
#define HSM_DEFINE_STATE_ENTRY(class_name)                                      \
            void class_name::entry(void* ctx)

/// @brief Define state exit function
#define HSM_DEFINE_STATE_EXIT(class_name)                                       \
            void class_name::exit(void* ctx)

/// @brief Define state init function
#define HSM_DEFINE_STATE_INIT(class_name)                                       \
            void class_name::init_(void* ctx)

/**
 * @brief Define state match function
 * First parameter `event` is of type `unsigned int`
 * Second parameter `t` is pointer of type `sTransition`
 * Third parameter `ctx` is context object
 * Return type is `bool`
 */
#define HSM_DEFINE_STATE_MATCH(class_name)                                       \
            bool class_name::match(unsigned int event, sTransition* t, void* ctx)

/**
 * @brief Define arbitrary state function
 * @param class_name Class name of state
 * @param return_type Return type of function
 * @param function_name Name of function
 * @param ... Optional number of parameter definitions
 */
#define HSM_DEFINE_STATE_FUNCTION(class_name, return_type, function_name, ...)  \
            return_type class_name::function_name(__VA_ARGS__)

#endif
