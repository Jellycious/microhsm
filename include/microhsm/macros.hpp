#ifndef _H_MICROHSM_MACROS
#define _H_MICROHSM_MACROS

#include <microhsm/microhsm.hpp>

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
 * @brief Create id list
 * Will create an enumerator with the provide ids
 * The first id will be assigned '0'
 * At the end an identifier '`enum_name`_COUNT' will be placed
 * this identifier represents the total number of states.
 * IDs are generally used to unique identify a vertex inside
 * of the HSM.
 * @param enum_name Enum type identifier
 * @param ... Other id identifiers
 */
#define HSM_CREATE_ID_LIST(enum_name, ...)                          \
    enum enum_name: unsigned int {                                  \
        __VA_ARGS__                                                 \
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
 * @brief Declare a top-level state
 * @note A top-level state is a state with no parents
 * @param class_name Class name of state    
 * @param id Unique Identifier of state (`unsigned int`)
 */
#define HSM_DECLARE_STATE_TOP_LEVEL(class_name, id, ...)                                            \
    class class_name : public State {                                                               \
        public:                                                                                     \
            /**                                                                                     \
             * @brief Constructor for class_name                                                    \
             * @param initialState Pointer to initial state (has to be`nullptr` for leaf states)    \
             */                                                                                     \
            class_name(State* initialState) :                                                       \
            State(id, nullptr, initialState) {};                                                    \
                                                                                                    \
            /**                                                                                     \
             * @brief Constructor for class_name                                                    \
             * @param initialState Pointer to initial state (has to be`nullptr` for leaf states)    \
             * @param shallowHistory Pointer to shallow history node, can be `nullptr` (`History*`) \
             * @param deepHistory Pointer to deep history node, can be `nullptr` (`History*`)       \
             */                                                                                     \
            class_name(State* initialState,                                                         \
                    History* shallowHistory, History* deepHistory) :                                \
            State(id, nullptr, initialState, shallowHistory, deepHistory) {};                       \
                                                                                                    \
            bool match(unsigned int event, sTransition* t, void* ctx) override;                     \
            __VA_ARGS__                                                                             \
    };

/**
 * @brief Declare a top-level state
 * @note A top-level state is a state with no parents
 * @param class_name Class name of state    
 * @param id Unique Identifier of state (`unsigned int`)
 * @param parent_state_class Class name of parent state
 */
#define HSM_DECLARE_STATE(class_name, id, parent_state_class, ...)                                  \
    class class_name : public State {                                                               \
        public:                                                                                     \
            /**                                                                                     \
             * @brief Constructor for class_name                                                    \
             * @param parentState Pointer to parent state (`parent_state_class*`)                   \
             * @param initialState Pointer to initial state (has to be`nullptr` for leaf states)    \
             */                                                                                     \
            class_name(parent_state_class* parentState, State* initialState) :                      \
            State(id, parentState, initialState) {};                                                \
                                                                                                    \
            /**                                                                                     \
             * @brief Constructor for class_name                                                    \
             * @param parentState Pointer to parent state (`parent_state_class*`)                   \
             * @param initialState Pointer to initial state (has to be`nullptr` for leaf states)    \
             * @param shallowHistory Pointer to shallow history node, can be `nullptr` (`History*`) \
             * @param deepHistory Pointer to deep history node, can be `nullptr` (`History*`)       \
             */                                                                                     \
            class_name(parent_state_class* parentState, State* initialState,                        \
                    History* shallowHistory, History* deepHistory) :                                \
            State(id, parentState, initialState, shallowHistory, deepHistory) {};                   \
                                                                                                    \
            bool match(unsigned int event, sTransition* t, void* ctx) override;                     \
            __VA_ARGS__                                                                             \
    };

/**
 * @brief Declare base state
 * A base state can be used as a base class for other states. Normally used to
 * extend the functionality of normal states by adding hooks such as `init_`.
 * The difference between `HSM_DECLARE_STATE` and `HSM_DECLARE_BASE_STATE`
 * is that `HSM_DECLARE_BASE_STATE` does not declare a match function.
 * @param base_class Class name of state    
 */
#define HSM_DECLARE_BASE_STATE(base_class, ...)                                     \
    class base_class : public State {                                               \
        public:                                                                     \
            base_class(unsigned int id, State* parentState, State* initialState) :  \
            State(id, parentState, initialState) {};                                \
                                                                                    \
            base_class(unsigned int id, State* parentState, State* initialState,    \
                    History* shallowHistory, History* deepHistory) :                \
            State(id, parentState, initialState, shallowHistory, deepHistory) {};   \
                                                                                    \
            __VA_ARGS__                                                             \
    };

/**
 * @brief Declare top-level state from base class
 * @note Top-level state is a state that has no parent state
 * @param class_name Class name of state    
 * @param base_class Class name of base class
 */
#define HSM_DECLARE_STATE_TOP_LEVEL_FROM_BASE(class_name, base_class, ...)                          \
    class class_name : public base_class {                                                          \
        public:                                                                                     \
            /* Constructor of class_name                                                            \
             * @param id Unique state ID                                                            \
             * @param initialState Poiter to initial state (has to be 'nullptr for leaf states)     \
             */                                                                                     \
            class_name(unsigned int id, State* initialState) :                                      \
            base_class(id, nullptr, initialState) {};                                               \
                                                                                                    \
            /* Constructor of class_name                                                            \
             * @param id Unique state ID                                                            \
             * @param initialState Poiter to initial state (has to be 'nullptr for leaf states)     \
             * @param shallowHistory Pointer to shallow history node                                \
             * @param deepHistory Pointer to deep history node                                      \
             */                                                                                     \
            class_name(unsigned int id, State* initialState,                                        \
                    History* shallowHistory, History* deepHistory) :                                \
            base_class(id, nullptr, initialState, shallowHistory, deepHistory) {};                  \
                                                                                                    \
            bool match(unsigned int event, sTransition* t, void* ctx) override;                     \
            __VA_ARGS__                                                                             \
    };

/**
 * @brief Declare state from base class
 * @param class_name Class name of state    
 * @param parent_state_class Class name of parent state
 * @param base_class Class name of base class
 */
#define HSM_DECLARE_STATE_FROM_BASE(class_name, parent_state_class, base_class, ...)                \
    class class_name : public base_class {                                                          \
        public:                                                                                     \
            /**                                                                                     \
             * Constructor of class_name                                                            \
             * @param id Unique state ID                                                            \
             * @param parentState Pointer to parent state                                           \
             * @param initialState Poiter to initial state (has to be 'nullptr for leaf states)     \
             */                                                                                     \
            class_name(unsigned int id, parent_state_class* parentState, State* initialState) :     \
            base_class(id, parentState, initialState) {};                                           \
                                                                                                    \
            /**                                                                                     \
             * Constructor of class_name                                                            \
             * @param id Unique state ID                                                            \
             * @param parentState Pointer to parent state (`parent_state_class*`)                   \
             * @param initialState Poiter to initial state (has to be 'nullptr` for leaf states)    \
             * @param shallowHistory Pointer to shallow history node (`History*`)                   \
             * @param deepHistory Pointer to deep history node (`History*`)                         \
             */                                                                                     \
            class_name(unsigned int id, parent_state_class* parentState, State* initialState,       \
                    History* shallowHistory, History* deepHistory) :                                \
            base_class(id, parentState, initialState, shallowHistory, deepHistory) {};              \
                                                                                                    \
            bool match(unsigned int event, sTransition* t, void* ctx) override;                     \
            __VA_ARGS__                                                                             \
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
