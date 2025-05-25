/**
 * @file Vertex.hpp
 * @brief Vertex declaration
 *
 * @author Jelle Meijer
 * @date 2025-05-23
 */

#ifndef _H_MICROHSM_VERTEX
#define _H_MICROHSM_VERTEX

namespace microhsm
{


    /**
     * @class Vertex
     * @brief Vertex class used for states and pseudostates
     *
     * Vertex is the base class of states and psuedostates.
     * A vertex is simply a structure holding a type and a unique ID.
     * Transition identify their sources and targets by referencing
     * these IDs.
     */
    class Vertex
    {
        public:
            /**
             * @enum eType
             * @brief Type of vertex
             */
            enum eType {
                eSTATE,             ///< Vertex is a state
                ePSEUDO_HISTORY,    ///< Vertex is a history pseudostate
            };

            /**
             * @brief Vertex constructor
             * @param id Unique ID of vertex
             * @param type Type of vertex
             */
            Vertex(unsigned int id, eType type);

            /// @brief Unique vertex ID
            const unsigned int ID;
            /// @brief Vertex type (`Vertex::e_type`)
            const eType TYPE;
    };

}

#endif
