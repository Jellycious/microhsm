#include "microhsm/objects/State.hpp"
#include <microhsm/objects/History.hpp>
#include <microhsm/config.hpp>

namespace microhsm
{

    History::History(unsigned int id) : History(id, nullptr)
    {
    }

    History::History(unsigned int id, State* defaultHistory) :
        Vertex(id, ePSEUDO_HISTORY),
        defaultHistory_(defaultHistory)
    {
        this->setHistoryState(defaultHistory);
    }

    void History::init_deep(State* parent)
    {

#if MICROHSM_ASSERTIONS == 1
        MICROHSM_ASSERT(parent != nullptr);             // Parent must exist and be composite
        MICROHSM_ASSERT(parent->initial != nullptr);    // Parent must be a composite state with initial state set
#endif
        // Determine default history state
        State* s = (defaultHistory_ == nullptr) ? parent->initial : defaultHistory_;

#if MICROHSM_ASSERTIONS == 1
        MICROHSM_ASSERT(s->isDescendentOf(parent->ID)); // Default history must be descendent of parent
        MICROHSM_ASSERT(s != nullptr);                  // Parent must be a composite state with initial state set
#endif

        // Walk down parents initial states until leave state is reached
        while (s->initial != nullptr) {
            s = s->initial;
        }

        // Set history state
        setHistoryState(s);
    }

    void History::init_shallow(State* parent)
    {

#if MICROHSM_ASSERTIONS == 1
        MICROHSM_ASSERT(parent != nullptr);             // Parent must exist and be composite
        MICROHSM_ASSERT(parent->initial != nullptr);    // Parent must be a composite state with initial state set
#endif
        // Determine default history state
        State* s = (defaultHistory_ == nullptr) ? parent->initial : defaultHistory_;

#if MICROHSM_ASSERTIONS == 1
        MICROHSM_ASSERT(s->isDescendentOf(parent->ID)); // Default history must be descendent of parent
        MICROHSM_ASSERT(s != nullptr);                  // Parent must be a composite state with initial state set
#endif

        // Set history state
        setHistoryState(s);
    }

    void History::setHistoryState(State* state)
    {
        this->historyState_ = state;
    }

    State* History::getHistoryState(void)
    {
        return this->historyState_;
    }

}
