#ifndef TEXTURE_RECT_H
#define TEXTURE_RECT_H

#include "control/control.h"
#include "renderer/texture/texture.h"
#include <memory>
#include <glm/glm.hpp>

class TextureRect : public Control {
public:
    explicit TextureRect(const std::string& name = "TextureRect");
    ~TextureRect() override = default;
    
    void set_texture(const std::shared_ptr<Texture>& texture);
    std::shared_ptr<Texture> get_texture() const { return m_texture; }
    
    enum StretchMode {
        STRETCH_SCALE,
        STRETCH_TILE,
        STRETCH_KEEP,
        STRETCH_KEEP_ASPECT,
        STRETCH_KEEP_ASPECT_CENTERED,
        STRETCH_KEEP_ASPECT_COVERED
    };
    
    void set_stretch_mode(StretchMode mode);
    StretchMode get_stretch_mode() const { return m_stretch_mode; }
    
    void set_flip_h(bool flip);
    bool is_flipped_h() const { return m_flip_h; }
    
    void set_flip_v(bool flip);
    bool is_flipped_v() const { return m_flip_v; }
    
    void set_expand(bool expand);
    bool is_expanding() const { return m_expand; }
    
    void set_clip_contents(bool clip);
    bool is_clipping_contents() const { return m_clip_contents; }
    
    void set_modulate(const glm::vec4& color);
    glm::vec4 get_modulate() const { return m_modulate; }
    
    void _draw() override;
    void _notification(int what) override;
    
private:
    std::shared_ptr<Texture> m_texture;
    StretchMode m_stretch_mode = STRETCH_SCALE;
    bool m_flip_h = false;
    bool m_flip_v = false;
    bool m_expand = false;
    bool m_clip_contents = false;
    glm::vec4 m_modulate = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    
    void draw_texture();
    glm::vec4 get_draw_rect() const;
};

#endif // TEXTURE_RECT_H