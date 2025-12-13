#include "canvas_item.h"
#include "scene/scene_tree.h"
#include <glm/gtc/matrix_transform.hpp>

CanvasItem::CanvasItem(const std::string& name)
    : Node(name)
{
}

void CanvasItem::show() {
    if (!m_visible) {
        m_visible = true;
        queue_redraw();
    }
}

void CanvasItem::hide() {
    if (m_visible) {
        m_visible = false;
        queue_redraw();
    }
}

void CanvasItem::set_visible(bool visible) {
    if (m_visible != visible) {
        m_visible = visible;
        queue_redraw();
    }
}

bool CanvasItem::is_visible_in_tree() const {
    const CanvasItem* current = this;
    while (current) {
        if (!current->m_visible) {
            return false;
        }
        if (Node* parent = current->get_parent()) {
            current = dynamic_cast<CanvasItem*>(parent);
            if (!current) break;
        } else {
            break;
        }
    }
    return true;
}

void CanvasItem::set_modulate(const glm::vec4& color) {
    if (m_modulate != color) {
        m_modulate = color;
        queue_redraw();
    }
}

void CanvasItem::set_z_index(int index) {
    if (m_z_index != index) {
        m_z_index = index;
        // Уведомляем родительский CanvasLayer об изменении Z-index
        queue_redraw();
    }
}

void CanvasItem::set_z_as_relative(bool relative) {
    if (m_z_as_relative != relative) {
        m_z_as_relative = relative;
        queue_redraw();
    }
}

void CanvasItem::set_self_modulate(const glm::vec4& color) {
    if (m_self_modulate != color) {
        m_self_modulate = color;
        queue_redraw();
    }
}

void CanvasItem::set_opacity(float opacity) {
    if (m_opacity != opacity) {
        m_opacity = glm::clamp(opacity, 0.0f, 1.0f);
        queue_redraw();
    }
}

void CanvasItem::set_blend_mode(BlendMode mode) {
    if (m_blend_mode != mode) {
        m_blend_mode = mode;
        queue_redraw();
    }
}

void CanvasItem::set_light_mode(LightMode mode) {
    if (m_light_mode != mode) {
        m_light_mode = mode;
        queue_redraw();
    }
}

void CanvasItem::set_material(const std::shared_ptr<class Material>& material) {
    if (m_material != material) {
        m_material = material;
        queue_redraw();
    }
}

void CanvasItem::set_use_parent_material(bool use) {
    if (m_use_parent_material != use) {
        m_use_parent_material = use;
        queue_redraw();
    }
}

void CanvasItem::set_position(const glm::vec2& position) {
    if (m_position != position) {
        m_position = position;
        m_transform_dirty = true;
        queue_redraw();
    }
}

void CanvasItem::set_rotation(float rotation) {
    if (m_rotation != rotation) {
        m_rotation = rotation;
        m_transform_dirty = true;
        queue_redraw();
    }
}

void CanvasItem::set_scale(const glm::vec2& scale) {
    if (m_scale != scale) {
        m_scale = scale;
        m_transform_dirty = true;
        queue_redraw();
    }
}

void CanvasItem::set_transform(const glm::mat3& transform) {
    if (m_transform != transform) {
        m_transform = transform;
        m_transform_dirty = false; // Уже обновлено напрямую
        queue_redraw();
    }
}

void CanvasItem::update_transform() {
    if (m_transform_dirty) {
        // Создаем матрицу трансформации: Translate * Rotate * Scale
        glm::mat3 translation = glm::mat3(1.0f);
        translation[2][0] = m_position.x;
        translation[2][1] = m_position.y;
        
        glm::mat3 rotation = glm::mat3(1.0f);
        float c = cos(m_rotation);
        float s = sin(m_rotation);
        rotation[0][0] = c;
        rotation[0][1] = s;
        rotation[1][0] = -s;
        rotation[1][1] = c;
        
        glm::mat3 scaling = glm::mat3(1.0f);
        scaling[0][0] = m_scale.x;
        scaling[1][1] = m_scale.y;
        
        m_transform = translation * rotation * scaling;
        m_transform_dirty = false;
    }
}

glm::mat3 CanvasItem::get_global_transform() const {
    glm::mat3 global_transform = m_transform;
    
    // Умножаем на трансформации родителей
    const Node* parent = get_parent();
    while (parent) {
        const CanvasItem* parent_canvas_item = dynamic_cast<const CanvasItem*>(parent);
        if (parent_canvas_item) {
            global_transform = parent_canvas_item->m_transform * global_transform;
        }
        parent = parent->get_parent();
    }
    
    return global_transform;
}

glm::mat3 CanvasItem::get_global_transform_with_canvas() const {
    // Для простоты возвращаем глобальную трансформацию
    // В реальной реализации нужно учитывать CanvasLayer трансформации
    return get_global_transform();
}

void CanvasItem::_draw() {
    // Пустая реализация по умолчанию
    // Производные классы должны переопределить этот метод
    
    if (m_draw_callback) {
        m_draw_callback();
    }
}

void CanvasItem::queue_redraw() {
    m_needs_redraw = true;
}

void CanvasItem::update() {
    if (m_needs_redraw && is_visible_in_tree()) {
        update_transform();
        _draw();
        m_needs_redraw = false;
    }
}

void CanvasItem::ready() {
    Node::ready();
    _ready();
}

void CanvasItem::process(float delta) {
    Node::process(delta);
    
    _process(delta);
    
    if (m_needs_redraw && is_visible_in_tree()) {
        update_transform();
        _draw();
        m_needs_redraw = false;
    }
}

void CanvasItem::enter_tree() {
    Node::enter_tree();
    _enter_tree();
    queue_redraw(); // Перерисовываем при добавлении в дерево
}

void CanvasItem::exit_tree() {
    _exit_tree();
    Node::exit_tree();
}

void CanvasItem::connect_draw(const DrawCallback& callback) {
    m_draw_callback = callback;
}

void CanvasItem::set_shader(const std::shared_ptr<Shader>& shader) {
    if (m_shader != shader) {
        m_shader = shader;
        queue_redraw();
    }
}

std::shared_ptr<Shader> CanvasItem::get_shader() const
{
    return m_shader;
}