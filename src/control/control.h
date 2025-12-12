#ifndef CONTROL_H
#define CONTROL_H

#include "canvas_item/canvas_item.h"
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <functional>

// Простой класс для событий ввода
struct InputEvent {
    enum Type {
        MOUSE_BUTTON,
        MOUSE_MOTION,
        KEY
    };
    
    Type type;
    glm::vec2 position;
    int button;
    int action;
};

class Control : public CanvasItem {
public:
    explicit Control(const std::string& name = "Control");
    ~Control() override = default;
    
    // Размер и положение
    void set_rect(const glm::vec4& rect); // x, y, width, height
    glm::vec4 get_rect() const { return m_rect; }
    
    void set_position(const glm::vec2& position);
    void set_size(const glm::vec2& size);
    void set_width(float width);
    void set_height(float height);
    
    glm::vec2 get_position() const { return glm::vec2(m_rect.x, m_rect.y); }
    glm::vec2 get_size() const { return glm::vec2(m_rect.z, m_rect.w); }
    float get_width() const { return m_rect.z; }
    float get_height() const { return m_rect.w; }
    
    // Якоря и отступы (anchors & margins)
    enum AnchorPreset {
        PRESET_TOP_LEFT,
        PRESET_TOP_RIGHT,
        PRESET_BOTTOM_LEFT,
        PRESET_BOTTOM_RIGHT,
        PRESET_CENTER_LEFT,
        PRESET_CENTER_RIGHT,
        PRESET_CENTER_TOP,
        PRESET_CENTER_BOTTOM,
        PRESET_CENTER,
        PRESET_LEFT_WIDE,
        PRESET_TOP_WIDE,
        PRESET_RIGHT_WIDE,
        PRESET_BOTTOM_WIDE,
        PRESET_VCENTER_WIDE,
        PRESET_HCENTER_WIDE,
        PRESET_FULL_RECT
    };
    
    enum Side {
        SIDE_LEFT = 0,
        SIDE_TOP = 1,
        SIDE_RIGHT = 2,
        SIDE_BOTTOM = 3
    };
    
    void set_anchor(Side side, float anchor, bool keep_margin = true);
    float get_anchor(Side side) const;
    
    void set_margin(Side side, float margin);
    float get_margin(Side side) const;
    
    void set_anchor_preset(AnchorPreset preset, bool keep_margin = true);
    
    // Выравнивание
    enum SizeFlags {
        SIZE_FILL = 1,
        SIZE_EXPAND = 2,
        SIZE_SHRINK_CENTER = 4,
        SIZE_SHRINK_END = 8
    };
    
    void set_h_size_flags(int flags);
    int get_h_size_flags() const { return m_h_size_flags; }
    
    void set_v_size_flags(int flags);
    int get_v_size_flags() const { return m_v_size_flags; }
    
    // Фокус
    void grab_focus();
    void release_focus();
    bool has_focus() const { return m_has_focus; }
    
    enum FocusMode {
        FOCUS_NONE,
        FOCUS_CLICK,
        FOCUS_ALL
    };
    
    void set_focus_mode(FocusMode mode);
    FocusMode get_focus_mode() const { return m_focus_mode; }
    
    // Текст
    void set_text(const std::string& text);
    const std::string& get_text() const { return m_text; }
    
    void set_tooltip(const std::string& tooltip);
    const std::string& get_tooltip() const { return m_tooltip; }
    
    // Тема и стили (упрощенно)
    void set_theme(const std::shared_ptr<class Theme>& theme);
    std::shared_ptr<class Theme> get_theme() const { return m_theme; }
    
    // События
    void connect_gui_input(const std::function<void(const InputEvent&)>& callback);
    void connect_mouse_entered(const std::function<void()>& callback);
    void connect_mouse_exited(const std::function<void()>& callback);
    void connect_focus_entered(const std::function<void()>& callback);
    void connect_focus_exited(const std::function<void()>& callback);
    
    // CanvasItem методы
    void _draw() override;
    void _process(float delta) override;
    void _ready() override;
    void _gui_input(const InputEvent& event);
    void _mouse_enter();
    void _mouse_exit();
    
    // Переопределение Node методов
    void process(float delta) override;
    
    // Уведомления (упрощенно)
    enum Notification {
        NOTIFICATION_RESIZED,
        NOTIFICATION_THEME_CHANGED
    };
    
    virtual void _notification(int what);
    
    // Минимальный размер
    virtual glm::vec2 get_minimum_size() const;
    
protected:
    glm::vec4 m_rect = glm::vec4(0.0f, 0.0f, 64.0f, 64.0f); // x, y, width, height
    
    struct Anchor {
        float left = 0.0f;
        float top = 0.0f;
        float right = 0.0f;
        float bottom = 0.0f;
    };
    
    struct Margin {
        float left = 0.0f;
        float top = 0.0f;
        float right = 0.0f;
        float bottom = 0.0f;
    };
    
    Anchor m_anchor;
    Margin m_margin;
    int m_h_size_flags = 0;
    int m_v_size_flags = 0;
    
    std::string m_text;
    std::string m_tooltip;
    
    bool m_has_focus = false;
    bool m_mouse_inside = false;
    
    FocusMode m_focus_mode = FOCUS_NONE;
    
    std::shared_ptr<class Theme> m_theme;
    std::map<std::string, std::shared_ptr<class StyleBox>> m_theme_overrides;
    
    // Callbacks
    std::function<void(const InputEvent&)> m_gui_input_callback;
    std::function<void()> m_mouse_entered_callback;
    std::function<void()> m_mouse_exited_callback;
    std::function<void()> m_focus_entered_callback;
    std::function<void()> m_focus_exited_callback;
    
private:
    void update_anchors_and_margins();
    bool is_point_inside(const glm::vec2& point) const;
};

#endif // CONTROL_H