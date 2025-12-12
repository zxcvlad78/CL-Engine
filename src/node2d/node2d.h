//node2d.h

#ifndef NODE_2D_H
#define NODE_2D_H

#include "node/node.h
#include "glm.hpp"

class Node2D : public Node{
public:
    Node2D();
    ~Node2D();

    void set_position(glm::vec2 position&);
    void set_scale(glm::vec2 scale&);
    void set_rotation(glm::vec2 rotation&);

    glm::vec2 get_position() const { return m_position }
    glm::vec2 get_scale() const { return m_scale; }
    glm::vec2 get_rotation() const { return m_rotation; }

private:
    glm::vec2 m_position;
    glm::vec2 m_scale;
    glm::vec2 m_rotation;
}


#endif