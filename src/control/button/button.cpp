#include "control/button/button.h"
#include <iostream>

Button::Button(const std::string& name)
    : Control(name)
{
}

void Button::set_text(const std::string& text) {
    if (m_text != text) {
        m_text = text;
        queue_redraw();
    }
}

void Button::set_icon(const std::shared_ptr<Texture>& icon) {
    if (m_icon != icon) {
        m_icon = icon;
        queue_redraw();
    }
}

void Button::set_disabled(bool disabled) {
    ButtonState new_state = disabled ? STATE_DISABLED : STATE_NORMAL;
    if (m_state != new_state) {
        m_state = new_state;
        queue_redraw();
    }
}

void Button::set_toggle_mode(bool toggle) {
    if (m_toggle_mode != toggle) {
        m_toggle_mode = toggle;
        queue_redraw();
    }
}

void Button::set_pressed(bool pressed) {
    if (m_pressed != pressed) {
        m_pressed = pressed;
        queue_redraw();
        emit_toggled(pressed);
    }
}

void Button::connect_pressed(const std::function<void()>& callback) {
    m_pressed_callback = callback;
}

void Button::connect_toggled(const std::function<void(bool)>& callback) {
    m_toggled_callback = callback;
}

void Button::_draw() {
    Control::_draw();
    
}

void Button::_gui_input(const InputEvent& event) {
    Control::_gui_input(event);
    
    if (m_state == STATE_DISABLED) {
        return;
    }
    
    // Используем константы из InputEvent
    if (event.type == InputEvent::MOUSE_BUTTON && 
        event.action == 1 && // PRESS
        event.button == 0) { // LEFT
        m_state = STATE_PRESSED;
        queue_redraw();
    }
    else if (event.type == InputEvent::MOUSE_BUTTON && 
             event.action == 0 && // RELEASE
             event.button == 0) { // LEFT
        if (m_state == STATE_PRESSED) {
            m_state = STATE_NORMAL;
            if (m_toggle_mode) {
                m_pressed = !m_pressed;
                emit_toggled(m_pressed);
            }
            emit_pressed();
            queue_redraw();
        }
    }
}

void Button::_mouse_enter() {
    Control::_mouse_enter();
    
    if (m_state != STATE_DISABLED && m_state != STATE_PRESSED) {
        m_state = STATE_HOVER;
        queue_redraw();
    }
}

void Button::_mouse_exit() {
    Control::_mouse_exit();
    
    if (m_state == STATE_HOVER) {
        m_state = STATE_NORMAL;
        queue_redraw();
    }
}

void Button::update_state() {
    // Обновление состояния кнопки
}

void Button::emit_pressed() {
    if (m_pressed_callback) {
        m_pressed_callback();
    }
}

void Button::emit_toggled(bool pressed) {
    if (m_toggled_callback) {
        m_toggled_callback(pressed);
    }
}