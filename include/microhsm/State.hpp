#ifndef _H_MICROHSM_STATE
#define _H_MICROHSM_STATE

namespace microhsm
{
    // Forward declaration of HSM
    class HSM;
    class State;

    enum eTransitionKind {
        eKIND_EXTERNAL,     /// @brief External transition
        eKIND_LOCAL,        /// @brief Local transition
        eKIND_INTERNAL      /// @brief Internal (self) transition
    };

    typedef void (*fTransitionEffect)(void *ctx);

    /**
     * @struct Representation of transition
     */
    typedef struct {
        unsigned int sourceID;
        unsigned int targetID;
        eTransitionKind kind;         //< Type of transition
        fTransitionEffect effect;     //< Effect of transition
    } sTransition;


    class State
    {
        public:

            State(unsigned int id, State* parent, State* initial);

            /**
             * @brief Perform state entry
             * @param ctx Context object
             */
            virtual void entry(void* ctx) {(void)ctx;};

            /**
             * @brief Perform state exit
             * @param ctx Context object
             */
            virtual void exit(void* ctx) {(void)ctx;};

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
            virtual bool match(unsigned int event, sTransition* t, void* ctx);

            /**
             * @brief Optional initialization function
             * @param ctx Context object
             */
            virtual void init(void* ctx) {(void)ctx;};

            /**
             * @brief Check whether state is a composite state
             * A state with a non-null initial state is considered composite
             * @return `true` if state is composite
             */
            bool isComposite(void);

            /**
             * @brief Check whether this state is descendent of `state`
             * @param state State to check against
             * @retval `true` This state is descendent of `state`
             * @retval `false` This state is not descendent of `state`
             */
            bool isDescendentOf(unsigned int ID);

            /**
             * @brief Get ancestor with ID
             * @param ID Unique identifier of ancestor
             * @return Pointer to ancestor state or nullptr
             */
            State* getAncestor(unsigned int ID);

            /// @brief Parent state (set to `nullptr` if state is not a substate)
            State* const parent;
            /// @brief Initial state (set to `nullptr` for non-composite states)
            State* const initial;
            /// @brief Unique identifier of state
            const unsigned int ID;
            /// @brief Depth of state
            const unsigned int depth;
            /// @brief Hierarchical State Machine
            HSM* hsm = nullptr;
            /// @brief Pointer to state used for utility
            State* tmp = nullptr;

        protected:

            /**
             * @brief No transition matched
             */
            bool noTransition();

            /**
             * @brief Matched an external transition.
             * An external transition will result in exit/entry behavior
             * of the source vertex/target state to be executed.
             * The target and source can be the same vertex.
             * @param target Pointer to target state of transition
             * @param t Pointer to transition object
             * @param effect Effect to execute upon transition
             */
            bool transitionExternal(unsigned int target_ID, sTransition* t, fTransitionEffect effect);

            /**
             * @brief Matched a local transition
             * A transition which will not exit its containing state.
             * Local transitions can only exist within composite states.
             * For local transitions, the target and source vertex cannot be the same.
             * @param target Pointer to target state of transition
             * @param t Pointer to transition object
             * @param effect Effect to execute upon transition
             */
            bool transitionLocal(unsigned int target_ID, sTransition* t, fTransitionEffect effect);

            /**
             * @brief Matched an internal transition.
             * An internal transition is a type of self-transition that does not
             * result in any entry/exit behaviors from being executed.
             * @param t Pointer to transition object
             * @param effect Effect to execute upon transition
             */
            bool transitionInternal(sTransition* t, fTransitionEffect effect);

        private:
            
            static unsigned int computeDepth_(State* s);
            bool isComposite_ = false;

    };
}

#endif /* _H_MICROHSM_STATE */
