//node.cpp

#include "node.h"
#include <algorithm>
#include <stdexcept>

Node::Node(const std::string& name)
    : Object(name)
{
}

Node::~Node()
{
    m_children.clear();
}

void Node::process(float delta)
{
    for (auto& child : m_children) {
        child->process(delta);
    }
}

void Node::physics_process(float delta)
{
    for (auto& child : m_children) {
        child->physics_process(delta);
    }
}

void Node::add_child(std::unique_ptr<Node> child)
{
    if (!child) return;
    
    Node* current = this;
    while (current) {
        if (current == child.get()) {
            throw std::runtime_error("Cannot add node as a child of itself");
        }
        current = current->get_parent();
    }
    
    child->m_parent = this;
    
    m_children.push_back(std::move(child));
}

void Node::remove_child(Node* child)
{
    auto it = std::find_if(m_children.begin(), m_children.end(),
        [child](const std::unique_ptr<Node>& ptr) {
            return ptr.get() == child;
        });
    
    if (it != m_children.end()) {
        (*it)->m_parent = nullptr;
        m_children.erase(it);
    }
}

Node* Node::get_child(int index) const
{
    if (index < 0 || index >= static_cast<int>(m_children.size())) {
        return nullptr;
    }
    return m_children[index].get();
}

void Node::free()
{
    if (m_parent) {
        m_parent->remove_child(this);
    }
}

void Node::queue_free()
{
    // реализация отложенного удаления
    free();
}

void Node::ready()
{
    if (!m_ready_called) {
        m_ready_called = true;
        for (auto& child : m_children) {
            child->ready();
        }
    }
}

void Node::enter_tree()
{
    m_inside_tree = true;
    for (auto& child : m_children) {
        child->enter_tree();
    }
}

void Node::exit_tree()
{
    m_inside_tree = false;
    for (auto& child : m_children) {
        child->exit_tree();
    }
}

Node* Node::find_node(const std::string& path) const
{
    for (const auto& child : m_children) {
        if (child->get_name() == path) {
            return child.get();
        }
        Node* result = child->find_node(path);
        if (result) {
            return result;
        }
    }
    return nullptr;
}