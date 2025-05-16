#ifndef _H_MICROHSM_STATE
#define _H_MICROHSM_STATE

#include <microhsm/objects/Vertex.hpp>

namespace microhsm
{

    // Forward declaration of history
    // Not included to avoid recursive inclusion
    class History;

    /// @brief Enumeration of transition types
    enum eTransitionKind {
        eKIND_EXTERNAL,     /// @brief External transition
        eKIND_LOCAL,        /// @brief Local transition
        eKIND_INTERNAL      /// @brief Internal (self) transition
    };

    /// @brief Function type of a transition effect
    typedef void (*fTransitionEffect)(void *ctx);

    /// @brief Transition structure
    typedef struct {
        unsigned int sourceID;      /// @brief Source of transition (State)
        unsigned int targetID;      /// @brief Target of transition (State/Pseudostate) 
        eTransitionKind kind;       /// @brief Type of transition
        fTransitionEffect effect;   /// @brief Effect of transition
    } sTransition;

    /// @brief State class
    class State : public Vertex
    {
        // Provide deep-coupling with `HSM` for dispatching purposes
        friend class HSM;

        public:

            /**
             * @brief State constructor
             * @param id Unique ID of state
             * @param parent Parent state (leave `nullptr` for top-level state)
             * @param initial Initial state for composite state (leave `nullptr` for non-composite state)
             */
            State(unsigned int id, State* parent, State* initial);

            /**
             * @brief State constructor
             * @param id Unique ID of state
             * @param parent Parent state (leave `nullptr` for top-level state)
             * @param initial Initial state for composite state (leave `nullptr` for non-composite state)
             * @param shallowHistory Pointer to `History` used for shallow history (`nullptr` if no history node needs to be tracked)
             * @param deepHistory Pointer to `History` used for deep history (`nullptr` if no history node needs to be tracked)
             */
            State(unsigned int id, State* parent, State* initial, History* shallowHistory, History* deepHistory);

            /**
             * @brief Destructor
             */
            virtual ~State();

            /**
             * @brief Perform state entry
             * @param ctx Context object
             */
            virtual void entry(void* ctx);

            /**
             * @brief Perform state exit
             * @param ctx Context object
             */
            virtual void exit(void* ctx);

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
            virtual bool match(unsigned int event, sTransition* t, void* ctx) = 0;

            /**
             * @brief Initialization
             * @param ctx Context object
             */
            void init(void* ctx);

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
             * @note State itself is not considered to be an ancestor
             * @param ID Unique identifier of ancestor
             * @return Pointer to ancestor state or nullptr
             */
            State* getAncestor(unsigned int ID);

            /// @brief Parent state (set to `nullptr` if state is not a substate)
            State* const parent;
            /// @brief Initial state (set to `nullptr` for non-composite states)
            State* const initial;
            /// @brief Depth of state
            const unsigned int depth;


        protected:

            /**
             * @brief Initialization callback
             * Called after state initialization. Provides user 
             * with a hook to perform actions after initialization.
             * @param ctx Context object
             */
            virtual void init_(void* ctx) {(void)ctx;}

            /**
             * @brief No transition matched
             * @return `false`
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
             * @return `true`
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
             * @return `true`
             */
            bool transitionLocal(unsigned int target_ID, sTransition* t, fTransitionEffect effect);

            /**
             * @brief Matched an internal transition.
             * An internal transition is a type of self-transition that does not
             * result in any entry/exit behaviors from being executed.
             * @param t Pointer to transition object
             * @param effect Effect to execute upon transition
             * @return `true`
             */
            bool transitionInternal(sTransition* t, fTransitionEffect effect);

        private:

            /**
             * @brief Temporary pointer used for internal purposes
             * This pointer is used to link states temporarily together.
             * This is used by `HSM` during event dispatching to create
             * paths that are traversed during the activation of transitions.
             */
            State* tmp_ = nullptr;
            
            /**
             * @brief Compute depth of state
             * @return Depth of state
             */
            static unsigned int computeDepth_(State* s);

            /// @brief Whether state is a composite state
            bool isComposite_ = false;

            History* shallowHistory_ = nullptr;
            History* deepHistory_ = nullptr;

    };
}

#endif /* _H_MICROHSM_STATE */
