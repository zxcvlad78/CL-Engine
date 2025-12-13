#ifndef NODE_H
#define NODE_H

#include "object/object.h"
#include <string>
#include <memory>
#include <vector>
#include <functional>

class Node : public Object {
public:
    explicit Node(const std::string& name = "Node");
    virtual ~Node();
    
    const std::string& get_name() const { return m_name; }
    void set_name(const std::string& name) { m_name = name; }
    
    Node* get_parent() const { return m_parent; }
    Node* get_child(int index) const;
    int get_child_count() const { return static_cast<int>(m_children.size()); }
    
    void add_child(std::unique_ptr<Node> child);
    void remove_child(Node* child);
    
    Node* find_node(const std::string& path) const;
    Node* find_node(const std::function<bool(Node*)>& predicate) const;
    
    virtual void ready();
    virtual void enter_tree();
    virtual void exit_tree();
    virtual void process(float delta);
    virtual void physics_process(float delta);
    
    bool is_inside_tree() const { return m_inside_tree; }
    virtual bool is_visible_in_tree() const { return true; }
    
    virtual std::string get_scene_tree_info(int indent = 0) const;
    void print_scene_tree() const;
    
protected:
    std::string m_name;
    Node* m_parent = nullptr;
    std::vector<std::unique_ptr<Node>> m_children;
    bool m_inside_tree = false;
    
private:
    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;
};

#endif // NODE_H