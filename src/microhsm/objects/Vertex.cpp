/**
 * @file Vertex.cpp
 * @brief Vertex declaration
 *
 * @author Jelle Meijer
 * @date 2025-05-23
 */

#include <microhsm/microhsm.hpp>

namespace microhsm
{

    Vertex::Vertex(unsigned int id, eType type):
        ID(id),
        TYPE(type)
    {
    }

}
