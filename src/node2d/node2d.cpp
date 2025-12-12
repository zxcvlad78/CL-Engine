#include "node2d.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <cmath>

Node2D::Node2D(const std::string& name)
    : CanvasItem(name)
{
}

void Node2D::set_rotation_degrees(float degrees) {
    set_rotation(glm::radians(degrees));
}

float Node2D::get_rotation_degrees() const {
    return glm::degrees(get_rotation());
}

void Node2D::set_global_position(const glm::vec2& position) {
    // Для простоты - в реальности нужно учитывать родительские трансформации
    set_position(position);
}

glm::vec2 Node2D::get_global_position() const {
    glm::mat3 global_transform = get_global_transform();
    return glm::vec2(global_transform[2][0], global_transform[2][1]);
}

void Node2D::set_global_rotation(float rotation) {
    // Для простоты
    set_rotation(rotation);
}

float Node2D::get_global_rotation() const {
    return get_rotation(); // В реальности нужно извлекать из глобальной матрицы
}

void Node2D::set_global_scale(const glm::vec2& scale) {
    // Для простоты
    set_scale(scale);
}

glm::vec2 Node2D::get_global_scale() const {
    glm::mat3 global_transform = get_global_transform();
    return glm::vec2(
        glm::length(glm::vec2(global_transform[0][0], global_transform[0][1])),
        glm::length(glm::vec2(global_transform[1][0], global_transform[1][1]))
    );
}

void Node2D::update_transform_matrix() {
    // Реализовано в CanvasItem
}

glm::vec2 Node2D::to_local(const glm::vec2& global_point) const {
    glm::mat3 inverse_transform = glm::inverse(get_global_transform());
    glm::vec3 result = inverse_transform * glm::vec3(global_point, 1.0f);
    return glm::vec2(result);
}

glm::vec2 Node2D::to_global(const glm::vec2& local_point) const {
    glm::mat3 transform = get_global_transform();
    glm::vec3 result = transform * glm::vec3(local_point, 1.0f);
    return glm::vec2(result);
}

void Node2D::translate(const glm::vec2& offset) {
    set_position(get_position() + offset);
}

void Node2D::rotate(float radians) {
    set_rotation(get_rotation() + radians);
}

void Node2D::look_at(const glm::vec2& point) {
    glm::vec2 direction = glm::normalize(point - get_position());
    float rotation = atan2(direction.y, direction.x);
    set_rotation(rotation);
}

glm::vec2 Node2D::get_forward() const {
    float rotation = get_rotation();
    return glm::vec2(cos(rotation), sin(rotation));
}

void Node2D::_draw() {
    CanvasItem::_draw();
    // Node2D по умолчанию ничего не рисует
    // Производные классы (Sprite, TileMap и т.д.) переопределят этот метод
}

void Node2D::_process(float delta) {
    CanvasItem::_process(delta);
}