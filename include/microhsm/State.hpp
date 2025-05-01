#ifndef _H_MICROHSM_STATE
#define _H_MICROHSM_STATE

namespace microhsm
{
    // Forward declaration of HSM
    class HSM;
    class State;

    enum e_transition_kind {
        eKIND_EXTERNAL,
        eKIND_LOCAL,
        eKIND_INTERNAL
    };

    typedef void (*f_transition_effect)(void *ctx);

    /**
     * @struct Representation of transition
     */
    typedef struct {
        unsigned int source_ID;
        unsigned int target_ID;
        e_transition_kind kind;         //< Type of transition
        f_transition_effect effect;     //< Effect of transition
    } s_transition;


    class State
    {
        public:

            State(unsigned int ID, State* parent, State* initial);
            
            /**
             * @brief Perform state entry
             * @param ctx Context object
             */
            virtual void entry(void* ctx) {};

            /**
             * @brief Perform state exit
             * @param ctx Context object
             */
            virtual void exit(void* ctx) {};

            /**
             * @brief Match event to transition
             * Override this function and implement the logic that matches an event and
             * inspects guards. Set the `t` parameter to represent the transition.
             * @param event Event that occurred 
             * @param t Pointer to transition struct
             * @param ctx Context object
             * @retval `true` Event matched a transition
             * @retval `false` Event did not match any transition
             */
            virtual bool match(unsigned int event, s_transition* t, void* ctx);

            /// @brief Parent state (set to `nullptr` if state is not a substate)
            State* const parent;
            /// @brief Initial state (set to `nullptr` for non-composite states)
            State* const initial;
            /// @brief Hierarchical State Machine
            HSM* hsm = nullptr;
            /// @brief Unique identifier of state
            const unsigned int ID; // Unique identifier of state

        protected:

            /**
             * @brief No transition matched
             */
            bool no_transition();

            /**
             * @brief Matched an external transition.
             * An external transition will result in exit behavior
             * of the source state to be executed.
             * @param target Pointer to target state of transition
             * @param t Pointer to transition object
             * @param effect Effect to execute upon transition
             */
            bool transition_external(unsigned int target_ID, s_transition* t, f_transition_effect effect);

            /**
             * @brief Matched a local transition
             * The transition will not exit the containing state.
             * Local transitions can only exist within a composite state.
             * @param target Pointer to target state of transition
             * @param t Pointer to transition object
             * @param effect Effect to execute upon transition
             */
            bool transition_local(unsigned int target_ID, s_transition* t, f_transition_effect effect);

            /**
             * @brief Matched an internal transition.
             * An internal transition is a type of self-transition that does not
             * result in any entry/exit being executed
             * @param t Pointer to transition object
             * @param effect Effect to execute upon transition
             */
            bool transition_internal(s_transition* t, f_transition_effect effect);

    };
}

#endif /* _H_MICROHSM_STATE */
