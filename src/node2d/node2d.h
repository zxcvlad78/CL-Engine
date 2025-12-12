#ifndef NODE_2D_H
#define NODE_2D_H

#include "canvas_item/canvas_item.h"
#include <glm/glm.hpp>

class Node2D : public CanvasItem {
public:
    explicit Node2D(const std::string& name = "Node2D");
    ~Node2D() override = default;
    
    // Трансформация (используем методы CanvasItem)
    using CanvasItem::set_position;
    using CanvasItem::get_position;
    using CanvasItem::set_rotation;
    using CanvasItem::get_rotation;
    using CanvasItem::set_scale;
    using CanvasItem::get_scale;
    
    void set_rotation_degrees(float degrees);
    float get_rotation_degrees() const;
    
    void set_global_position(const glm::vec2& position);
    glm::vec2 get_global_position() const;
    
    void set_global_rotation(float rotation);
    float get_global_rotation() const;
    
    void set_global_scale(const glm::vec2& scale);
    glm::vec2 get_global_scale() const;
    
    // Преобразование координат
    glm::vec2 to_local(const glm::vec2& global_point) const;
    glm::vec2 to_global(const glm::vec2& local_point) const;
    
    // Перемещение
    void translate(const glm::vec2& offset);
    void rotate(float radians);
    void look_at(const glm::vec2& point);
    
    glm::vec2 get_forward() const;
    
    // CanvasItem методы
    void _draw() override;
    void _process(float delta) override;
    
protected:
    // Данные трансформации уже есть в CanvasItem
    
private:
    void update_transform_matrix();
};

#endif // NODE_2D_H