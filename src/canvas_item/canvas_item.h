#ifndef CANVAS_ITEM_H
#define CANVAS_ITEM_H

#include "node/node.h"
#include <glm/glm.hpp>
#include <memory>
#include <functional>
#include "renderer/material/material.h"

class CanvasItem : public Node {
public:
    explicit CanvasItem(const std::string& name = "CanvasItem");
    ~CanvasItem() override = default;
    
    void show();
    void hide();
    void set_visible(bool visible);
    bool is_visible() const { return m_visible; }
    bool is_visible_in_tree() const;
    
    void set_modulate(const glm::vec4& color);
    const glm::vec4& get_modulate() const { return m_modulate; }
    
    void set_z_index(int index);
    int get_z_index() const { return m_z_index; }
    
    void set_z_as_relative(bool relative);
    bool is_z_relative() const { return m_z_as_relative; }
    
    void set_self_modulate(const glm::vec4& color);
    const glm::vec4& get_self_modulate() const { return m_self_modulate; }
    
    void set_opacity(float opacity);
    float get_opacity() const { return m_opacity; }

    enum BlendMode {
        BLEND_MIX,
        BLEND_ADD,
        BLEND_SUB,
        BLEND_MUL,
        BLEND_PREMULT_ALPHA
    };
    
    void set_blend_mode(BlendMode mode);
    BlendMode get_blend_mode() const { return m_blend_mode; }
    
    enum LightMode {
        LIGHT_MODE_NORMAL,
        LIGHT_MODE_UNSHADED,
        LIGHT_MODE_LIGHT_ONLY
    };
    
    void set_light_mode(LightMode mode);
    LightMode get_light_mode() const { return m_light_mode; }
    
    void set_material(const std::shared_ptr<Material>& material);
    std::shared_ptr<Material> get_material() const { return m_material; }
    
    void set_use_parent_material(bool use);
    bool is_using_parent_material() const { return m_use_parent_material; }
    
    virtual void _draw();
    virtual void _process(float delta) {}
    virtual void _ready() {}
    virtual void _enter_tree() {}
    virtual void _exit_tree() {}
    
    void queue_redraw();
    void update();
    
    void set_position(const glm::vec2& position);
    const glm::vec2& get_position() const { return m_position; }
    
    void set_rotation(float rotation);
    float get_rotation() const { return m_rotation; }
    
    void set_scale(const glm::vec2& scale);
    const glm::vec2& get_scale() const { return m_scale; }
    
    void set_transform(const glm::mat3& transform);
    const glm::mat3& get_transform() const { return m_transform; }
    
    glm::mat3 get_global_transform() const;
    glm::mat3 get_global_transform_with_canvas() const;
    
    void ready() override;
    void process(float delta) override;
    void enter_tree() override;
    void exit_tree() override;
    
    using DrawCallback = std::function<void()>;
    void connect_draw(const DrawCallback& callback);
    
protected:
    bool m_visible = true;
    glm::vec4 m_modulate = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec4 m_self_modulate = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    float m_opacity = 1.0f;
    int m_z_index = 0;
    bool m_z_as_relative = true;
    bool m_needs_redraw = true;
    bool m_use_parent_material = false;
    
    glm::vec2 m_position = glm::vec2(0.0f);
    float m_rotation = 0.0f;
    glm::vec2 m_scale = glm::vec2(1.0f);
    glm::mat3 m_transform = glm::mat3(1.0f);
    bool m_transform_dirty = true;
    
    BlendMode m_blend_mode = BLEND_MIX;
    LightMode m_light_mode = LIGHT_MODE_NORMAL;
    std::shared_ptr<Material> m_material;
    
private:
    void update_transform();
    DrawCallback m_draw_callback;
};

#endif // CANVAS_ITEM_H