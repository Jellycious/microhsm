# MicroHSM: Embedded Hierarchical State Machine (HSM) Library

A lightweight, but powerful embedded-friendly C++ library implementing UML-compliant hierarchical
state machines.

---

## Features

- **UML-complicant semantics**, including:
    - ✅ Hierarchy
    - ✅ External, local and internal transitions
    - ✅ Transition guards and effects
    - ✅ Entry and exit actions
    - ✅ Anonymous transitions
    - ✅ Shallow and deep history

- **Run-to-completion event dispatcher** - Ensures determinism
- **Small memory footprint** - Simple and efficient
- **No use of dynamic memory** - Static memory only
- **No use of C++ standard library** - Ideal for bare-metal
- **Build as static libary or include directly** - Integrate in any project
- **Lightweight** - Library consists of less than 750 lines of code

---

## Hierarchical State Machines ⚙️

A Hierarchical State Machine (HSM) is a behavioral modeling construct that extends
traditional finite state machines (FSMs) with support for nested (hierarchical) states.
In an HSM, states can be organized in a tree-like structure where a superstate can
contain one or more substates. This enables powerful reuse of behavior,
reduces redundancy, and simplifies complex state models.

HSMs follow formal semantics defined by the UML (Unified Modeling Language) State Machine specification, 
supporting features like:

- Inheritance of behavior through superstates
- Entry/exit actions for encapsulating state-related behavior
- History mechanisms to remember previous substates
- Guard conditions and transition effects for decision logic

### Why use Hierarchical State Machines (HSMs)

While small state-based logic can be implemented using `switch`/`case` statements or simple
function pointers, this approach quickly breaks down as the system grows in complexity.
Such `switch`/`case` statements can quickly get out of hand and their behavior cannot be
easily understood from just looking at the code.

- Maintainability - HSMs can create cleaner code as shared behavior can be implemented once in a composite-state instead of copied across multiple states.
- Readability - visual HSM diagrams can provide an immediate intuitive understanding of system behavior. Furthermore, these diagrams can be read by programmers and non-programmers alike.
- Encapsulation - behavior can be more easily encapsulated in a single state by using entry and exit behaviors.
- Determinism - HSMs enforce a highly deterministic control flow for states and events. This makes your system more predictable and reliable.

---

# How to use

One can define state machines in two different ways:

- Option 1 - Defining states and HSMs manually.

> Create your HSMs by creating your own HSMs and state classes that inherit from the
`BaseHSM` and `BaseState` classes.

- Option 2 - Defining states and HSMs through the use of macros

> Define your HSMs using a set of expressive macros. This approach offers a more concise and readable syntax,
significantly reducing boilerplate code. While it abstracts away
some implementation details, it streamlines development and improves maintainability for most use cases.

## Vertex and Event IDs
Every vertex and event should have a unique ID associated to it.
```
// Vertex IDs
enum eValveState : unsigned int {
    eSTATE_IDLE = 0,
    eSTATE_RUNNING,
    eSTATE_CLOSED,
    eSTATE_OPEN,
    eSTATE_COUNT
};

// Event IDs
enum eValveEvent : unsigned int {
    eEVENT_ANONYMOUS = 0, // Reserved event ID 0 for anonymous events
    eEVENT_START,
    eEVENT_TICK,
    eEVENT_PAUSE,
};
```

or alternatively with macros:

```
HSM_CREATE_VERTEX_LIST(eValveState,
    eSTATE_IDLE,
    eSTATE_RUNNING,
    eSTATE_CLOSED,
    eSTATE_OPEN,
    eSTATE_COUNT
)
HSM_CREATE_EVENT_LIST(eValveEvent,
    eEVENT_START,
    eEVENT_PAUSE,
    eEVENT_TICK
)
```

## State declarations
```
// Declare a top-level state
class StateRunning : public microhsm::BaseState
{
    public:
        StateRunning(microhsm::BaseState* initialState) : microhsm::BaseState(
                eSTATE_RUNNING,     // ID of `StateRunning`
                nullptr,            // Pointer to parent state of `StateRunning` (`nullptr` because no parent)
                initialState)       // Pointer to initial state of `StateRunning`
        {
        };

        bool match(unsigned int event, microhsm::sTransition* t, void* ctx) override;
};

// Declare a state with a parent
class StateOpen : public microhsm::BaseState
{
    public:
        StateOpen(StateRunning* parentState) : microhsm::BaseState(
                eSTATE_OPEN,        // ID of `StateOpen`
                parentState,        // Pointer to parent state of `StateOpen`
                nullptr)            // Pointer to initial state of `StateOpen`
        {
        };

        bool match(unsigned int event, microhsm::sTransition* t, void* ctx) override;

        // Optional entry function
        void entry(void* ctx) override;
};
```

or alternatively with macros:

```
HSM_DECLARE_STATE_TOP_LEVEL(StateRunning, eSTATE_RUNNING)
HSM_DECLARE_STATE(StateClosed, eSTATE_CLOSED, StateRunning,
        HSM_DECLARE_STATE_ENTRY()
)
```

or alternatively

```
HSM_DECLARE_STATE_TOP_LEVEL(StateRunning, eSTATE_RUNNING)
```

## HSM declaration

Then we declare the HSM itself.

```
class ValveHSM : public microhsm::BaseHSM
    {
        public:
            /*
             * The constructor of `ValveHSM`
             * provides the initial state to the `BaseHSM`.
             */
            ValveHSM() : microhsm::BaseHSM(state_idle) {}

            /* Required functions */
            microhsm::Vertex* getVertex(unsigned int ID) override;
            unsigned int getMaxID(void) override;

        private:

            /* States */
            StateIdle state_idle = StateIdle(nullptr);                          // Argument is the initial state (`nullptr` because state has no children)
            StateRunning state_running = StateRunning(&state_closed);           // Argument is the initial state
            StateOpen state_open = StateOpen(&state_running, nullptr);          // Arguments are parent state and initial state respectively
            StateClosed state_closed = StateClosed(&state_running, nullptr);    // Arguments are parent and initial state respectively
    };
```

## Implement required HSM functions

For every HSM it is required to add two functions. These are used by the dispatcher to perform transitions.

```
// Should return the highest possible vertex ID
unsigned int ValveHSM::getMaxID()
{
    // Here we simply return the highest possible ID for a state
    return static_cast<unsigned int>(eSTATE_COUNT) - 1;
}

// Should return vertex associated to an ID
microhsm::Vertex* ValveHSM::getVertex(unsigned int id)
{
    // In this case modelled as a simple switch case
    switch(id) {
        case eSTATE_IDLE: return &this->state_idle;
        case eSTATE_RUNNING: return &this->state_running;
        case eSTATE_OPEN: return &this->state_open;
        case eSTATE_CLOSED: return &this->state_closed;
        default: return nullptr;
    }
}
```

## Implement state transitions

Every state requires a `match` function. The match function should match events to transitions and possibly check any guards.

```
bool StateClosed::match(unsigned int event, microhsm::sTransition* t, void* ctx)
{
    ValveContext* valve = static_cast<ValveContext*>(ctx);
    // Define transition table
    switch(event) {
        case eEVENT_TICK:
            // Guards must be implemented manually by checking them in the `match` function.
            if (!valve->isLocked()) {
                // Transition from `StateClosed` to `StateOpen`, if guard valve->canOpen succeeds.
                return transitionExternal(eSTATE_OPEN, t, nullptr);
            }
            break;
        default:
            break;
    }
    return noTransition();
}
```

or alternatively with macros:

```
HSM_DEFINE_STATE_MATCH(StateClosed)
{
    ValveContext* valve = static_cast<ValveContext*>(ctx);
    // Define transition table
    switch(event) {
        case eEVENT_TICK:
            // Guards must be implemented manually by checking them in the `match` function.
            if (!valve->isLocked()) {
                // Transition from `StateClosed` to `StateOpen`, if guard valve->canOpen succeeds.
                return transitionExternal(eSTATE_OPEN, t, nullptr);
            }
            break;
        default:
            break;
    }
    return noTransition();
}
```

---

# Examples
In `microhsm/example` you will find two examples on the same Hierarchical State Machine.
The HSM is a model of a trickle Valve that opens and closes repeatedly. The valve can
be locked by an external command. When this is done, it is not allowed to go to the
open state. Furthermore, it is possible to pause the valve which will make it close itself
and ignore commands until the next start event.

![./valve.png](Valve HSM)

The difference between the two implementations is that one makes use of normal C++ syntax
while the other makes use of macros. Macros reduce boilerplate code at the cost of hiding
implementation details. Both methods offer the same functionality, the choice is mostly stylistic.

### Example Features

The examples make use of the following HSM features:

- State hierarchy
- Transition effects
- External transitions
- Entry behavior
- Transition guards
- Context object

### Context Object

The example also shows how a context object may be passed to the HSM. One has access to this
object in the `match`,`entry` and `exit` functions. Context objects are generally used to provide a
communication layer between the HSM and the rest of the application. One can methods and access the
context data during the `entry` and `exit` behaviors. In the example, this is used to open/close the valve upon
entering the open and closed states. See the following code snippet:

```
/*
 * One has access to the following parameters in the `entry` function:
 *
 * - `void* ctx`
 */
HSM_DEFINE_STATE_ENTRY(StateOpen)
{
    // We use a context object to open the valve
    ValveContext* context = static_cast<ValveContext*>(ctx);
    context->open();
}
```

Another usecase is for implementing guards. It is up to the implementor
to check guards appropriately in the `match` function of a state. In the example the HSM may only move to the
open state if the valve is not locked. This is achieved in the following manner:

```
/*
 * One has access to the following parameters in the `match` function:
 *
 * - 'unsigned int event`
 * - `microhsm::sTransition* t`
 * - `void* ctx`
 */
HSM_DEFINE_STATE_MATCH(StateClosed)
{
    ValveContext* valve = static_cast<ValveContext*>(ctx);
    // Define transition table
    switch(event) {
        case eEVENT_TICK:
            // We use the context object to implement a transition guard.
            if (!valve->isLocked()) {
                // Transition from `StateClosed` to `StateOpen`, if guard: `!valve->isLocked()` succeeds.
                return transitionExternal(eSTATE_OPEN, t, nullptr);
            }
            break;
        default:
            break;
    }
    return noTransition();
}
```
