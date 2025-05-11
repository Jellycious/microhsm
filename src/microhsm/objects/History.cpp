#include "microhsm/objects/State.hpp"
#include <microhsm/objects/History.hpp>
#include <microhsm/config.hpp>

namespace microhsm
{

    History::History(unsigned int id) : History(id, nullptr)
    {
    }

    History::History(unsigned int id, State* defaultHistory) : 
        Vertex(id, ePSEUDO_HISTORY)
    {
        this->setHistoryState(defaultHistory);
    }

    void History::init(State* parent)
    {
#if MICROHSM_ASSERTIONS == 1
        MICROHSM_ASSERT(parent != nullptr); // Parent must exist
#endif        

        if (this->historyState_ == nullptr) {
            // History node has no default transition
            setHistoryState(parent->initial);
        }
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
