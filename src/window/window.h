#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <functional>
#include <memory>
#include <utility>
#include <mutex>

class Window {
public:
    using KeyCallback = std::function<void(int, int, int, int)>;
    using SizeCallback = std::function<void(int, int)>;
    using MouseButtonCallback = std::function<void(int, int, int)>;
    using CursorPosCallback = std::function<void(double, double)>;

    struct Config {
        std::string title = "CL Engine";
        int width = 1280;
        int height = 720;
        bool fullscreen = false;
        bool vsync = false;
        bool resizable = true;
        int gl_major_version = 4;
        int gl_minor_version = 6;
        bool core_profile = true;
    };

    Window();
    explicit Window(const Config& config);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    Window(Window&& other) noexcept;
    Window& operator=(Window&& other) noexcept;

    bool initialize();
    bool create(const Config& config);
    void destroy();

    bool should_close() const;
    void swap_buffers();
    void poll_events();
    void clear(const glm::vec4& color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    void set_should_close(bool value);

    void set_key_callback(const KeyCallback& callback);
    void set_size_callback(const SizeCallback& callback);
    void set_mouse_button_callback(const MouseButtonCallback& callback);
    void set_cursor_pos_callback(const CursorPosCallback& callback);

    GLFWwindow* get_glfw_window() const { return m_window; }
    bool is_initialized() const { return m_initialized; }
    int get_width() const { return m_width; }
    int get_height() const { return m_height; }
    float get_aspect_ratio() const { return static_cast<float>(m_width) / static_cast<float>(m_height); }
    glm::ivec2 get_size() const { return glm::ivec2(m_width, m_height); }

    void set_title(const std::string& title);
    void set_size(int width, int height);
    void set_vsync(bool enabled);
    void set_fullscreen(bool fullscreen);

    static Window* get_current_window();
    static void set_current_window(Window* window);
    static Window* get_window_by_id(GLFWwindow* glfw_window);
    
    static void poll_all_events();
    static void wait_events();
    static void wait_events_timeout(double timeout);
    
    static int get_monitor_count();
    static glm::ivec2 get_primary_monitor_size();
    static glm::ivec2 get_monitor_size(int monitor_index = 0);

private:
    void setup_callbacks();
    void update_viewport();
    void register_window();
    void unregister_window();

    static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void glfw_window_size_callback(GLFWwindow* window, int width, int height);
    static void glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void glfw_cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
    static void glfw_window_focus_callback(GLFWwindow* window, int focused);

    GLFWwindow* m_window = nullptr;
    Config m_config;
    int m_width = 0;
    int m_height = 0;
    bool m_initialized = false;
    bool m_glad_loaded = false;

    KeyCallback m_key_callback;
    SizeCallback m_size_callback;
    MouseButtonCallback m_mouse_button_callback;
    CursorPosCallback m_cursor_pos_callback;

    static Window* s_current_window;
    static std::mutex s_windows_mutex;
    
    struct WindowInfo {
        GLFWwindow* glfw_window;
        Window* window_instance;
    };
    
    static std::vector<WindowInfo> s_windows;
};

#endif // WINDOW_H