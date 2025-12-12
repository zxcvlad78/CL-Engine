#include "viewport.h"
#include "canvas_layer/canvas_layer.h"
#include <iostream>
#include <algorithm>
#include <glad/glad.h>

Viewport::Viewport() : m_name("Viewport") {
    std::cout << "Viewport created: " << m_name << std::endl;
}

Viewport::Viewport(const std::string& name) : m_name(name) {
    std::cout << "Viewport created: " << m_name << std::endl;
}

Viewport::~Viewport() {
    std::cout << "Viewport destroyed: " << m_name << std::endl;
    m_layers.clear();
}

void Viewport::set_size(const glm::ivec2& size) {
    if (m_size != size && size.x > 0 && size.y > 0) {
        m_size = size;
        std::cout << "Viewport size set to: " << size.x << "x" << size.y << std::endl;
    }
}

void Viewport::add_layer(CanvasLayer* layer) {
    if (!layer) {
        std::cerr << "Cannot add null layer to viewport" << std::endl;
        return;
    }
    
    // Проверяем, нет ли уже этого слоя
    auto it = std::find(m_layers.begin(), m_layers.end(), layer);
    if (it != m_layers.end()) {
        std::cerr << "Layer already exists in viewport: " << layer->get_name() << std::endl;
        return;
    }
    
    m_layers.push_back(layer);
    std::cout << "Layer added to viewport: " << layer->get_name() << std::endl;
    
    // Если viewport уже в дереве, вызываем enter_tree для слоя
    if (m_in_tree) {
        layer->enter_tree();
        layer->ready();
    }
}

void Viewport::remove_layer(CanvasLayer* layer) {
    if (!layer) return;
    
    auto it = std::find(m_layers.begin(), m_layers.end(), layer);
    if (it != m_layers.end()) {
        // Если viewport в дереве, вызываем exit_tree для слоя
        if (m_in_tree) {
            layer->exit_tree();
        }
        
        m_layers.erase(it);
        std::cout << "Layer removed from viewport: " << layer->get_name() << std::endl;
    }
}

void Viewport::clear_layers() {
    // Выходим из дерева для всех слоев
    for (auto layer : m_layers) {
        if (layer && m_in_tree) {
            layer->exit_tree();
        }
    }
    
    m_layers.clear();
    std::cout << "All layers cleared from viewport" << std::endl;
}

void Viewport::render() {
    if (!m_in_tree) {
        return;
    }
    
    
    // Очищаем экран, если включено
    if (m_clear_color_enabled) {
        glClearColor(m_background_color.r, m_background_color.g, 
                     m_background_color.b, m_background_color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    // Сортируем слои по порядку (слой 0 рендерится первым, затем слой 1 и т.д.)
    std::sort(m_layers.begin(), m_layers.end(),
        [](CanvasLayer* a, CanvasLayer* b) {
            return a->get_layer() < b->get_layer();
        });
    
    // Рендерим все слои
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
    
    // Обрабатываем все слои
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
    
    // Обрабатываем физику всех слоев
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
    std::cout << "Viewport entered tree: " << m_name << std::endl;
    
    // Вызываем enter_tree для всех слоев
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
    std::cout << "Viewport exited tree: " << m_name << std::endl;
    
    // Вызываем exit_tree для всех слоев
    for (auto layer : m_layers) {
        if (layer) {
            layer->exit_tree();
        }
    }
}

void Viewport::ready() {
    std::cout << "Viewport ready: " << m_name << std::endl;
    
    // Вызываем ready для всех слоев
    for (auto layer : m_layers) {
        if (layer) {
            layer->ready();
        }
    }
}