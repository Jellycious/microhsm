# MicroHSM: Embedded Hierarchical State Machine (HSM) Library

A simple lightweight, embedded-friendly C++ library implementing UML-compliant hierarchical
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

## How to use

One can define state machines in two different ways:

- Option 1 - Defining states and HSMs manually.
> Create your HSMs by creating your own HSMs and state classes that inherit from the
`BaseHSM` and `BaseState` classes.

- Option 2 - Defining states and HSMs through the use of macros (**recommended**)
> Define your HSMs using a set of expressive macros. This approach offers a more concise and readable syntax, 
significantly reducing boilerplate code. While it abstracts away 
some implementation details, it streamlines development and improves maintainability for most use cases.

### Example

```
Some code
```





