#include "viewport.h"
#include "canvas_layer/canvas_layer.h"
#include <iostream>
#include <algorithm>
#include <glad/glad.h>

Viewport::Viewport() : m_name("Viewport")
{
}

Viewport::Viewport(const std::string& name) : m_name(name)
{
}

Viewport::~Viewport()
{
    m_layers.clear();
}

void Viewport::set_size(const glm::ivec2& size)
{
    if (m_size != size && size.x > 0 && size.y > 0)
    {
        m_size = size;
    }
}

void Viewport::add_layer(CanvasLayer* layer) {
    if (!layer) {
        return;
    }
    
    auto it = std::find(m_layers.begin(), m_layers.end(), layer);
    if (it != m_layers.end()) {
        return;
    }
    
    m_layers.push_back(layer);
    
    if (m_in_tree) {
        layer->enter_tree();
        layer->ready();
    }
}

void Viewport::remove_layer(CanvasLayer* layer) {
    if (!layer) return;
    
    auto it = std::find(m_layers.begin(), m_layers.end(), layer);
    if (it != m_layers.end()) {
        if (m_in_tree) {
            layer->exit_tree();
        }
        
        m_layers.erase(it);
    }
}

void Viewport::clear_layers() {
    for (auto layer : m_layers) {
        if (layer && m_in_tree) {
            layer->exit_tree();
        }
    }
    
    m_layers.clear();
}

void Viewport::render() {
    if (!m_in_tree) {
        return;
    }
    

    if (m_clear_color_enabled) {
        glClearColor(m_background_color.r, m_background_color.g, 
                     m_background_color.b, m_background_color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    std::sort(m_layers.begin(), m_layers.end(),
        [](CanvasLayer* a, CanvasLayer* b) {
            return a->get_layer() < b->get_layer();
        });

    for (auto layer : m_layers) {
        if (layer && layer->is_visible_in_tree()) {
            layer->render();
        }
    }
}

void Viewport::process(float delta) {
    if (!m_in_tree) {
        return;
    }
    
    for (auto layer : m_layers) {
        if (layer && layer->is_visible_in_tree()) {
            layer->process(delta);
        }
    }
}

void Viewport::physics_process(float delta) {
    if (!m_in_tree) {
        return;
    }

    for (auto layer : m_layers) {
        if (layer && layer->is_visible_in_tree()) {
            layer->physics_process(delta);
        }
    }
}

void Viewport::enter_tree() {
    if (m_in_tree) {
        return;
    }
    
    m_in_tree = true;
    for (auto layer : m_layers) {
        if (layer) {
            layer->enter_tree();
        }
    }
}

void Viewport::exit_tree() {
    if (!m_in_tree) {
        return;
    }
    
    m_in_tree = false;
    for (auto layer : m_layers) {
        if (layer) {
            layer->exit_tree();
        }
    }
}

void Viewport::ready() {
    for (auto layer : m_layers) {
        if (layer) {
            layer->ready();
        }
    }
}