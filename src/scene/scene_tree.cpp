#include "scene_tree.h"
#include "viewport/viewport.h"
#include <iostream>
#include <sstream>

SceneTree::SceneTree(const std::string& name)
    : Node(name)
{
    std::cout << "SceneTree created: " << name << std::endl;
}

SceneTree::~SceneTree() {
    std::cout << "SceneTree destroyed: " << get_name() << std::endl;
    m_viewports.clear();
}

void SceneTree::set_current_scene(std::shared_ptr<Node> scene) {
    if (m_current_scene != scene) {
        if (m_current_scene) {
            m_current_scene->exit_tree();
            remove_child(m_current_scene.get());
        }
        
        m_current_scene = scene;
        
        if (m_current_scene) {
            add_child(std::unique_ptr<Node>(m_current_scene.get()));
            m_current_scene->enter_tree();
            m_current_scene->ready();
        }
    }
}

void SceneTree::add_viewport(std::shared_ptr<Viewport> viewport) {
    if (!viewport) return;
    
    for (const auto& vp : m_viewports) {
        if (vp.get() == viewport.get()) {
            return;
        }
    }
    
    m_viewports.push_back(viewport);
    viewport->enter_tree();
    viewport->ready();
}

void SceneTree::remove_viewport(Viewport* viewport) {
    if (!viewport) return;
    
    for (auto it = m_viewports.begin(); it != m_viewports.end(); ++it) {
        if (it->get() == viewport) {
            viewport->exit_tree();
            m_viewports.erase(it);
            break;
        }
    }
}

void SceneTree::ready() {
    Node::ready();
    
    for (auto& viewport : m_viewports) {
        viewport->ready();
    }
    
    if (m_current_scene) {
        m_current_scene->ready();
    }
}

void SceneTree::process(float delta) {
    Node::process(delta);
    
    for (auto& viewport : m_viewports) {
        viewport->process(delta);
    }
    
    if (m_current_scene) {
        m_current_scene->process(delta);
    }
}

void SceneTree::physics_process(float delta) {
    for (auto& viewport : m_viewports) {
        viewport->physics_process(delta);
    }
    
    if (m_current_scene) {
        m_current_scene->physics_process(delta);
    }
}

void SceneTree::render() {
    for (auto& viewport : m_viewports) {
        viewport->render();
    }
}

void SceneTree::quit() {
    m_should_quit = true;
    std::cout << "SceneTree quit requested" << std::endl;
}

std::string SceneTree::get_scene_tree_info(int indent) const {
    std::stringstream ss;
    
    std::string indent_str(indent * 2, ' ');
    ss << indent_str << "└─ " << get_name() << " (SceneTree)\n";
    
    indent_str = std::string((indent + 1) * 2, ' ');
    for (const auto& viewport : m_viewports) {
        ss << indent_str << "├─ Viewport: " << viewport->get_name() << "\n";
    }
    
    if (m_current_scene) {
        ss << m_current_scene->get_scene_tree_info(indent + 1);
    }
    
    for (int i = 0; i < get_child_count(); ++i) {
        if (get_child(i) != m_current_scene.get()) {
            ss << get_child(i)->get_scene_tree_info(indent + 1);
        }
    }
    
    return ss.str();
}

std::string SceneTree::get_full_scene_hierarchy() const {
    std::stringstream ss;
    
    ss << "=== Full Scene Hierarchy ===\n";
    ss << "Scene Tree: " << get_name() << "\n\n";
    
    ss << "Viewports (" << m_viewports.size() << "):\n";
    for (size_t i = 0; i < m_viewports.size(); ++i) {
        ss << "  [" << i << "] " << m_viewports[i]->get_name() << "\n";
    }
    
    ss << "\nCurrent Scene:\n";
    if (m_current_scene) {
        ss << "  " << m_current_scene->get_scene_tree_info(1);
    } else {
        ss << "  None\n";
    }
    
    ss << "\nAll Nodes in Tree:\n";
    ss << get_scene_tree_info(0);
    
    return ss.str();
}