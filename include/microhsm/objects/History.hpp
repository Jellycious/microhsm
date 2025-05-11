#include <microhsm/objects/State.hpp>

namespace microhsm
{
    class History : public Vertex
    {
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
             * @brief Initialization function called by `State`
             */
            void init(State* parent);

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

            /// @brief Internal structure for storing history
            State* historyState_ = nullptr;
    };
}
