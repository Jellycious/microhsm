/**
 * @file History.cpp
 * @brief History Vertex used for tracking deep/shallow history.
 *
 * Contains class declaratinos for:
 *  - BaseHistory
 *  - DeepHistory
 *  - ShallowHistory
 *
 * @author Jelle Meijer
 * @date 2025-05-23
 */

#include <microhsm/microhsm.hpp>
#include <microhsm/config.hpp>

namespace microhsm
{

    /* --- History --- */
    BaseHistory::BaseHistory(unsigned int id, BaseState* defaultHistory) :
        Vertex(id, ePSEUDO_HISTORY),
        defaultHistory_(defaultHistory)
    {
        this->setHistoryState(defaultHistory);
    }

    void BaseHistory::setHistoryState(BaseState* state)
    {
        this->historyState_ = state;
    }

    BaseState* BaseHistory::getHistoryState(void)
    {
        return this->historyState_;
    }


    /* --- DeepHistory --- */
    DeepHistory::DeepHistory(unsigned int id) : DeepHistory(id, nullptr) {}
    DeepHistory::DeepHistory(unsigned int id, BaseState* defaultHistory) : BaseHistory(id, defaultHistory) {}

    void DeepHistory::init(BaseState* parent)
    {

#if MICROHSM_ASSERTIONS == 1
        MICROHSM_ASSERT(parent != nullptr);             // Parent must exist and be composite
        MICROHSM_ASSERT(parent->initial != nullptr);    // Parent must be a composite state with initial state set
#endif
        // Determine default history state
        BaseState* s = (defaultHistory_ == nullptr) ? parent->initial : defaultHistory_;

#if MICROHSM_ASSERTIONS == 1
        bool isDescendentOfParent = s->isDescendentOf(parent->ID);
        MICROHSM_ASSERT(s != nullptr);                  // Parent must be a composite state with initial state set
        MICROHSM_ASSERT(isDescendentOfParent);          // Default history must be descendent of parent
#endif

        // Walk down parents initial states until leave state is reached
        while (s->initial != nullptr) {
            s = s->initial;
        }

        // Set history state
        setHistoryState(s);
    }

    /* --- ShallowHistory --- */
    ShallowHistory::ShallowHistory(unsigned int id) : ShallowHistory(id, nullptr) {}
    ShallowHistory::ShallowHistory(unsigned int id, BaseState* defaultHistory) : BaseHistory(id, defaultHistory) {}

    void ShallowHistory::init(BaseState* parent)
    {

#if MICROHSM_ASSERTIONS == 1
        MICROHSM_ASSERT(parent != nullptr);             // Parent must exist and be composite
        MICROHSM_ASSERT(parent->initial != nullptr);    // Parent must be a composite state with initial state set
#endif
        // Determine default history state
        BaseState* s = (defaultHistory_ == nullptr) ? parent->initial : defaultHistory_;

#if MICROHSM_ASSERTIONS == 1
        bool isDescendentOfParent = s->isDescendentOf(parent->ID);
        MICROHSM_ASSERT(s != nullptr);                  // Parent must be a composite state with initial state set
        MICROHSM_ASSERT(isDescendentOfParent);          // Default history must be descendent of parent
#endif

        // Set history state
        setHistoryState(s);
    }


}
