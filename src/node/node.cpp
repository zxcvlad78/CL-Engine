#include "node.h"
#include <iostream>
#include <algorithm>

Node::Node(const std::string& name)
    : m_name(name)
{
}

Node::~Node() {
    
    // Удаляем всех детей
    for (auto& child : m_children) {
        if (child->is_inside_tree()) {
            child->exit_tree();
        }
    }
    m_children.clear();
}

Node* Node::get_child(int index) const {
    if (index >= 0 && index < static_cast<int>(m_children.size())) {
        return m_children[index].get();
    }
    return nullptr;
}

void Node::add_child(std::unique_ptr<Node> child) {
    if (!child) return;
    
    if (child->m_parent) {
        std::cerr << "Cannot add child " << child->get_name() 
                  << " because it already has a parent" << std::endl;
        return;
    }
    
    child->m_parent = this;
    m_children.push_back(std::move(child));
    
    Node* new_child = m_children.back().get();
    
    if (m_inside_tree) {
        new_child->enter_tree();
        new_child->ready();
    }
    
}

void Node::remove_child(Node* child) {
    if (!child) return;
    
    auto it = std::find_if(m_children.begin(), m_children.end(),
        [child](const std::unique_ptr<Node>& ptr) {
            return ptr.get() == child;
        });
    
    if (it != m_children.end()) {
        if (child->is_inside_tree()) {
            child->exit_tree();
        }
        
        child->m_parent = nullptr;
        m_children.erase(it);
    }
}

Node* Node::find_node(const std::string& path) const {
    // Простой поиск по имени
    for (const auto& child : m_children) {
        if (child->get_name() == path) {
            return child.get();
        }
        
        Node* found = child->find_node(path);
        if (found) {
            return found;
        }
    }
    return nullptr;
}

Node* Node::find_node(const std::function<bool(Node*)>& predicate) const {
    for (const auto& child : m_children) {
        if (predicate(child.get())) {
            return child.get();
        }
        
        Node* found = child->find_node(predicate);
        if (found) {
            return found;
        }
    }
    return nullptr;
}

void Node::ready() {
    
    for (const auto& child : m_children) {
        child->ready();
    }
}

void Node::enter_tree() {
    if (m_inside_tree) return;
    
    m_inside_tree = true;
    
    for (const auto& child : m_children) {
        child->enter_tree();
    }
}

void Node::exit_tree() {
    if (!m_inside_tree) return;
    
    m_inside_tree = false;
    
    for (const auto& child : m_children) {
        child->exit_tree();
    }
}

void Node::process(float delta) {
    for (const auto& child : m_children) {
        child->process(delta);
    }
}

void Node::physics_process(float delta) {
    for (const auto& child : m_children) {
        child->physics_process(delta);
    }
}

std::string Node::get_scene_tree_info(int indent) const {
    std::string result;
    
    // Отступ для текущего узла
    std::string indent_str(indent * 2, ' ');
    result += indent_str + "└─ " + m_name + " (" + typeid(*this).name() + ")\n";
    
    // Информация о детях
    for (const auto& child : m_children) {
        result += child->get_scene_tree_info(indent + 1);
    }
    
    return result;
}

void Node::print_scene_tree() const {
    std::cout << "Scene Tree:\n";
    std::cout << get_scene_tree_info();
}