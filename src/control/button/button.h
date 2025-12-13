#ifndef BUTTON_H
#define BUTTON_H

#include "control/control.h"
#include "renderer/texture/texture.h"
#include <string>
#include <functional>

class Button : public Control {
public:
    explicit Button(const std::string& name = "Button");
    ~Button() override = default;
    
    void set_text(const std::string& text);
    const std::string& get_text() const { return m_text; }
    
    void set_icon(const std::shared_ptr<Texture>& icon);
    std::shared_ptr<Texture> get_icon() const { return m_icon; }
    
    enum ButtonState {
        STATE_NORMAL,
        STATE_HOVER,
        STATE_PRESSED,
        STATE_DISABLED
    };
    
    void set_disabled(bool disabled);
    bool is_disabled() const { return m_state == STATE_DISABLED; }
    
    void set_toggle_mode(bool toggle);
    bool is_toggle_mode() const { return m_toggle_mode; }
    
    void set_pressed(bool pressed);
    bool is_pressed() const { return m_pressed; }
    
    void connect_pressed(const std::function<void()>& callback);
    void connect_toggled(const std::function<void(bool)>& callback);
    
    void _draw() override;
    void _gui_input(const InputEvent& event);
    void _mouse_enter();
    void _mouse_exit();
    
private:
    std::string m_text;
    std::shared_ptr<Texture> m_icon;
    
    ButtonState m_state = STATE_NORMAL;
    bool m_toggle_mode = false;
    bool m_pressed = false;
    
    std::function<void()> m_pressed_callback;
    std::function<void(bool)> m_toggled_callback;
    
    void update_state();
    void emit_pressed();
    void emit_toggled(bool pressed);
};

#endif // BUTTON_H