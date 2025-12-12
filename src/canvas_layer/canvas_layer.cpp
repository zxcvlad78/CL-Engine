#include "canvas_layer.h"
#include "canvas_item/canvas_item.h"
#include <algorithm>

CanvasLayer::CanvasLayer(const std::string& name)
    : Node2D(name)
{
}

void CanvasLayer::set_layer(int layer) {
    m_layer = layer;
}

void CanvasLayer::set_follow_viewport(bool follow) {
    m_follow_viewport = follow;
}

void CanvasLayer::add_item(CanvasItem* item) {
    if (!item) return;
    
    // Проверяем, нет ли уже этого элемента
    for (auto existing : m_items) {
        if (existing == item) {
            return;
        }
    }
    
    m_items.push_back(item);
}

void CanvasLayer::remove_item(CanvasItem* item) {
    if (!item) return;
    
    for (auto it = m_items.begin(); it != m_items.end(); ++it) {
        if (*it == item) {
            m_items.erase(it);
            break;
        }
    }
}

void CanvasLayer::ready() {
    Node2D::ready();
}

void CanvasLayer::process(float delta) {
    Node2D::process(delta);
    
    // Обрабатываем все элементы
    for (auto item : m_items) {
        item->process(delta);
    }
}

void CanvasLayer::render() {
    // Сортируем элементы по Z-index
    std::sort(m_items.begin(), m_items.end(),
        [](CanvasItem* a, CanvasItem* b) {
            return a->get_z_index() < b->get_z_index();
        });
    
    // Рендерим все элементы
    for (auto item : m_items) {
        if (item->is_visible_in_tree()) {
            item->_draw();
        }
    }
}