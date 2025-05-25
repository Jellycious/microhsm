/**
 * @file History.hpp
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

#ifndef _H_MICROHSM_HISTORY
#define _H_MICROHSM_HISTORY

#include <microhsm/objects/BaseState.hpp>

namespace microhsm
{
    /**
     * @class BaseHistory
     * @brief History base class
     *
     * Base class for the `DeepHistory` and `ShallowHistory` classes.
     * A history pseudostate is a state that is used for storing the
     * latest shallow or deep configuration of a composite state.
     * When a substate of a composite state is entered, its configuration
     * will be stored in the history psuedostate. When the history pseudostate
     * is entered, it will transition to the latest stored configuration.
     *
     * A history pseudostate can have a default history. This is the configuration
     * that the HSM will transition when the history's composite state had not been
     * entered before.
     */
    class BaseHistory : public Vertex
    {
        public:

            /**
             * @param History constructor with defaultHistory
             * @param id Unique ID of History Node
             * @param defaultHistory Optional default history node
             */
            BaseHistory(unsigned int id, BaseState* defaultHistory);


            /**
             * @brief Set the history to `state`
             * @param state State to set history to
             */
            void setHistoryState(BaseState* state);

            /**
             * @brief Get the history node
             * @return Last set state
             */
            BaseState* getHistoryState(void);

        protected:

            /// Default history state (can be `nullptr`)
            BaseState* const defaultHistory_;

        private:

            /**
             * @brief Initialize as deep history, called by `State`
             * @param parent The `State` to which this history belongs.
             */
            virtual void init(BaseState* parent) = 0;

            /// Internal structure for storing history
            BaseState* historyState_ = nullptr;

    };

    /**
     * @class DeepHistory
     * @brief Deep history class
     *
     * A deep history will store the full most recent configuration of its parent.
     * The deepest possible most recent substate of the parent will be stored.
     */
    class DeepHistory : public BaseHistory {

        // Allow `BaseState` to call private init function
        friend class BaseState;

        public:
            /**
             * @brief Deep history constructor
             * @param id Unique ID of deep history node
             */
            DeepHistory(unsigned int id);
            /**
             * @brief Deep history constructor
             * @param id Unique ID of deep history node
             * @param defaultHistory Default history node (can be `nullptr`)
             */
            DeepHistory(unsigned int id, BaseState* defaultHistory);

        private:
            /**
             * @brief Initialization deep history
             * @param state The `State` to which history belongs
             */
            void init(BaseState* state) override;
    };

    /**
     * @class ShallowHistory
     * @brief Shallow history class
     *
     * A shallow history will store the most recent active substate of its
     * parent. It will not store the most recent substates of that substate.
     * Only one level of depth is stored.
     */
    class ShallowHistory : public BaseHistory {

        // Allow `BaseState` to call private init function
        friend class BaseState;

        public:
            /**
             * @brief Shallow history constructor
             * @param id Unique ID of deep history node
             */
            ShallowHistory(unsigned int id);
            /**
             * @brief Shallow history constructor
             * @param id Unique ID of deep history node
             * @param defaultHistory Default history node (can be `nullptr`)
             */
            ShallowHistory(unsigned int id, BaseState* defaultHistory);

        private:
            /**
             * @brief Initialization shallow history
             * @param state The `State` to which history belongs
             */
            void init(BaseState* state) override;
    };
}

#endif
