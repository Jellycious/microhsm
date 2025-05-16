#include <microhsm/objects/State.hpp>

namespace microhsm
{
    class History : public Vertex
    {
        friend class State;

        public:
            /**
             * @brief Constructor
             * @param id Unique ID of History Node
             */
            History(unsigned int id);

            /**
             * @param History constructor with defaultHistory
             * @param id Unique ID of History Node
             * @param defaultHistory Optional default history node
             */
            History(unsigned int id, State* defaultHistory);


            /**
             * @brief Set the history to `state`
             * @param state State to set history to
             */
            void setHistoryState(State* state);

            /**
             * @brief Get the history node
             * @return Last set state
             */
            State* getHistoryState(void);


        private:

            /**
             * @brief Initialize as deep history, called by `State`
             */
            void init_deep(State* parent);

            /**
             * @brief Initialize as shallow history, called by `State`
             */
            void init_shallow(State* parent);

            /// @brief Default history state (can be `nullptr`) 
            State* const defaultHistory_; 
            /// @brief Internal structure for storing history
            State* historyState_ = nullptr;
    };
}
