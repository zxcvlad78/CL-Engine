#include "control/control.h"
#include "window/window.h"
#include <iostream>

Control::Control(const std::string& name)
    : CanvasItem(name)
{
}

void Control::set_rect(const glm::vec4& rect) {
    if (m_rect != rect) {
        m_rect = rect;
        queue_redraw();
        _notification(NOTIFICATION_RESIZED);
    }
}

void Control::set_position(const glm::vec2& position) {
    glm::vec4 new_rect = m_rect;
    new_rect.x = position.x;
    new_rect.y = position.y;
    set_rect(new_rect);
}

void Control::set_size(const glm::vec2& size) {
    glm::vec4 new_rect = m_rect;
    new_rect.z = size.x;
    new_rect.w = size.y;
    set_rect(new_rect);
}

void Control::set_width(float width) {
    glm::vec4 new_rect = m_rect;
    new_rect.z = width;
    set_rect(new_rect);
}

void Control::set_height(float height) {
    glm::vec4 new_rect = m_rect;
    new_rect.w = height;
    set_rect(new_rect);
}

void Control::set_anchor(Side side, float anchor, bool keep_margin) {
    float* anchor_ptr = nullptr;
    
    switch (side) {
        case SIDE_LEFT: anchor_ptr = &m_anchor.left; break;
        case SIDE_TOP: anchor_ptr = &m_anchor.top; break;
        case SIDE_RIGHT: anchor_ptr = &m_anchor.right; break;
        case SIDE_BOTTOM: anchor_ptr = &m_anchor.bottom; break;
    }
    
    if (anchor_ptr && *anchor_ptr != anchor) {
        float old_anchor = *anchor_ptr;
        *anchor_ptr = anchor;
        
        if (keep_margin) {
        }
        
        update_anchors_and_margins();
        queue_redraw();
    }
}

float Control::get_anchor(Side side) const {
    switch (side) {
        case SIDE_LEFT: return m_anchor.left;
        case SIDE_TOP: return m_anchor.top;
        case SIDE_RIGHT: return m_anchor.right;
        case SIDE_BOTTOM: return m_anchor.bottom;
        default: return 0.0f;
    }
}

void Control::set_margin(Side side, float margin) {
    float* margin_ptr = nullptr;
    
    switch (side) {
        case SIDE_LEFT: margin_ptr = &m_margin.left; break;
        case SIDE_TOP: margin_ptr = &m_margin.top; break;
        case SIDE_RIGHT: margin_ptr = &m_margin.right; break;
        case SIDE_BOTTOM: margin_ptr = &m_margin.bottom; break;
    }
    
    if (margin_ptr && *margin_ptr != margin) {
        *margin_ptr = margin;
        update_anchors_and_margins();
        queue_redraw();
    }
}

float Control::get_margin(Side side) const {
    switch (side) {
        case SIDE_LEFT: return m_margin.left;
        case SIDE_TOP: return m_margin.top;
        case SIDE_RIGHT: return m_margin.right;
        case SIDE_BOTTOM: return m_margin.bottom;
        default: return 0.0f;
    }
}

void Control::set_anchor_preset(AnchorPreset preset, bool keep_margin) {
    switch (preset) {
        case PRESET_TOP_LEFT:
            m_anchor.left = 0.0f; m_anchor.top = 0.0f;
            m_anchor.right = 0.0f; m_anchor.bottom = 0.0f;
            break;
        case PRESET_CENTER:
            m_anchor.left = 0.5f; m_anchor.top = 0.5f;
            m_anchor.right = 0.5f; m_anchor.bottom = 0.5f;
            break;
        case PRESET_FULL_RECT:
            m_anchor.left = 0.0f; m_anchor.top = 0.0f;
            m_anchor.right = 1.0f; m_anchor.bottom = 1.0f;
            break;
        default:
            m_anchor.left = 0.0f; m_anchor.top = 0.0f;
            m_anchor.right = 0.0f; m_anchor.bottom = 0.0f;
            break;
    }
    
    if (!keep_margin) {
        m_margin.left = m_margin.top = m_margin.right = m_margin.bottom = 0.0f;
    }
    
    update_anchors_and_margins();
    queue_redraw();
}

void Control::set_h_size_flags(int flags) {
    if (m_h_size_flags != flags) {
        m_h_size_flags = flags;
        queue_redraw();
    }
}

void Control::set_v_size_flags(int flags) {
    if (m_v_size_flags != flags) {
        m_v_size_flags = flags;
        queue_redraw();
    }
}

void Control::grab_focus() {
    if (m_focus_mode != FOCUS_NONE) {
        m_has_focus = true;
        if (m_focus_entered_callback) {
            m_focus_entered_callback();
        }
        queue_redraw();
    }
}

void Control::release_focus() {
    if (m_has_focus) {
        m_has_focus = false;
        if (m_focus_exited_callback) {
            m_focus_exited_callback();
        }
        queue_redraw();
    }
}

void Control::set_focus_mode(FocusMode mode) {
    if (m_focus_mode != mode) {
        m_focus_mode = mode;
        if (mode == FOCUS_NONE && m_has_focus) {
            release_focus();
        }
    }
}

void Control::set_text(const std::string& text) {
    if (m_text != text) {
        m_text = text;
        queue_redraw();
    }
}

void Control::set_tooltip(const std::string& tooltip) {
    m_tooltip = tooltip;
}

void Control::set_theme(const std::shared_ptr<class Theme>& theme) {
    if (m_theme != theme) {
        m_theme = theme;
        _notification(NOTIFICATION_THEME_CHANGED);
        queue_redraw();
    }
}

void Control::connect_gui_input(const std::function<void(const InputEvent&)>& callback) {
    m_gui_input_callback = callback;
}

void Control::connect_mouse_entered(const std::function<void()>& callback) {
    m_mouse_entered_callback = callback;
}

void Control::connect_mouse_exited(const std::function<void()>& callback) {
    m_mouse_exited_callback = callback;
}

void Control::connect_focus_entered(const std::function<void()>& callback) {
    m_focus_entered_callback = callback;
}

void Control::connect_focus_exited(const std::function<void()>& callback) {
    m_focus_exited_callback = callback;
}

void Control::_draw() {
    CanvasItem::_draw();
}

void Control::_process(float delta) {
    CanvasItem::_process(delta);
}

void Control::_ready() {
    CanvasItem::_ready();
}

void Control::_gui_input(const InputEvent& event) {
    if (m_gui_input_callback) {
        m_gui_input_callback(event);
    }
}

void Control::_mouse_enter() {
    m_mouse_inside = true;
    if (m_mouse_entered_callback) {
        m_mouse_entered_callback();
    }
    queue_redraw();
}

void Control::_mouse_exit() {
    m_mouse_inside = false;
    if (m_mouse_exited_callback) {
        m_mouse_exited_callback();
    }
    queue_redraw();
}

void Control::process(float delta) {
    CanvasItem::process(delta);
}

void Control::_notification(int what)
{
}

glm::vec2 Control::get_minimum_size() const {
    return glm::vec2(0.0f);
}

void Control::update_anchors_and_margins() {
    // в реальной реализации вычисляет rect на основе anchors и margins
}

bool Control::is_point_inside(const glm::vec2& point) const {
    return point.x >= m_rect.x && point.x <= m_rect.x + m_rect.z &&
           point.y >= m_rect.y && point.y <= m_rect.y + m_rect.w;
}