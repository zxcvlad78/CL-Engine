//node2d.cpp

#include "node2d.h"

Node2D::Node2D(const std::string& name)
    : Object(name)
{
}

Node2D::set_position(glm::vec2 position&)
{
    m_position = position;
}

Node2D::set_scale(glm::vec2 scale&)
{
    m_scale = scale;
}

Node2D::set_rotation(glm::vec2 rotation&)
{
    m_rotation = rotation;
}