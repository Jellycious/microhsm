#ifndef _H_MICROHSM_VERTEX 
#define _H_MICROHSM_VERTEX 

namespace microhsm 
{

    class Vertex
    {
        public:
            /// @brief Vertex type
            enum e_type {
                eSTATE,
                ePSEUDO_HISTORY,
            };

            /**
             * @brief Vertex constructor
             * @param id Unique ID of vertex
             * @param type Type of vertex
             */
            Vertex(unsigned int id, e_type type);

            /// @brief Unique vertex ID
            const unsigned int ID;
            /// @brief Vertex type (`Vertex::e_type`)
            const e_type TYPE;
    };

}

#endif
