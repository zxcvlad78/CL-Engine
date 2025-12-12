#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <memory>
#include <vector>
#include <string>
#include <glm/glm.hpp>

class CanvasLayer;

class Viewport {
public:
    Viewport();
    explicit Viewport(const std::string& name);
    ~Viewport();
    
    // Размер и положение
    void set_size(const glm::ivec2& size);
    glm::ivec2 get_size() const { return m_size; }
    
    int get_width() const { return m_size.x; }
    int get_height() const { return m_size.y; }
    
    // Слои
    void add_layer(CanvasLayer* layer);
    void remove_layer(CanvasLayer* layer);
    void clear_layers();
    
    // Имя
    const std::string& get_name() const { return m_name; }
    void set_name(const std::string& name) { m_name = name; }
    
    // Основные методы
    void render();
    void process(float delta);
    void physics_process(float delta);
    
    // Жизненный цикл
    void enter_tree();
    void exit_tree();
    void ready();
    
    // Состояние
    bool is_in_tree() const { return m_in_tree; }
    bool is_visible_in_tree() const { return m_in_tree; }
    
    // Дополнительные методы для камеры
    void set_background_color(const glm::vec4& color) { m_background_color = color; }
    glm::vec4 get_background_color() const { return m_background_color; }
    
    void set_clear_color_enabled(bool enabled) { m_clear_color_enabled = enabled; }
    bool is_clear_color_enabled() const { return m_clear_color_enabled; }
    
private:
    std::string m_name;
    glm::ivec2 m_size = {800, 600};
    glm::vec4 m_background_color = {0.0f, 0.0f, 0.0f, 1.0f};
    bool m_clear_color_enabled = true;
    std::vector<CanvasLayer*> m_layers;
    bool m_in_tree = false;
};

#endif // VIEWPORT_H