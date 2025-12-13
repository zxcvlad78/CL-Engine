#ifndef SCENE_TREE_H
#define SCENE_TREE_H

#include "node/node.h"
#include <memory>
#include <vector>

class Viewport;

class SceneTree : public Node {
public:
    explicit SceneTree(const std::string& name = "SceneTree");
    ~SceneTree() override;
    
    void set_current_scene(std::shared_ptr<Node> scene);
    std::shared_ptr<Node> get_current_scene() const { return m_current_scene; }
    
    void add_viewport(std::shared_ptr<Viewport> viewport);
    void remove_viewport(Viewport* viewport);
    
    void ready() override;
    void process(float delta) override;
    void physics_process(float delta) override;
    void render();
    
    void quit();
    
    std::string get_scene_tree_info(int indent = 0) const override;
    std::string get_full_scene_hierarchy() const;
    
    template<typename T>
    std::vector<T*> find_nodes_of_type() const {
        std::vector<T*> result;
        find_nodes_of_type_recursive<T>(this, result);
        return result;
    }
    
private:
    std::shared_ptr<Node> m_current_scene;
    std::vector<std::shared_ptr<Viewport>> m_viewports;
    bool m_should_quit = false;
    
    template<typename T>
    void find_nodes_of_type_recursive(Node* node, std::vector<T*>& result) const {
        if (auto casted = dynamic_cast<T*>(node)) {
            result.push_back(casted);
        }
        
        for (int i = 0; i < node->get_child_count(); ++i) {
            find_nodes_of_type_recursive<T>(node->get_child(i), result);
        }
    }
};

#endif // SCENE_TREE_H