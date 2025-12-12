//node.h

#ifndef NODE_H
#define NODE_H

#include "object/object.h"
#include <vector>
#include <memory>

class Node : public Object {
public:
    explicit Node(const std::string& name = "Node");
    ~Node() override;
    
    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;
    
    Node(Node&&) = default;
    Node& operator=(Node&&) = default;
    
    virtual void process(float delta);
    virtual void physics_process(float delta);
    
    void free();
    void queue_free();
    
    void add_child(std::unique_ptr<Node> child);
    void remove_child(Node* child);
    Node* get_child(int index) const;
    
    Node* get_parent() const { return m_parent; }
    const std::vector<std::unique_ptr<Node>>& get_children() const { return m_children; }
    
    Node* find_node(const std::string& path) const;
    
    virtual void ready();
    virtual void enter_tree();
    virtual void exit_tree();
    
protected:
    Node* m_parent = nullptr;
    std::vector<std::unique_ptr<Node>> m_children;
    
private:
    bool m_inside_tree = false;
    bool m_ready_called = false;
};

#endif