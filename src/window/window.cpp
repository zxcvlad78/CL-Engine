//window.cpp

#include "window.h"
#include <iostream>

Window::Window() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return;
    }
}

Window::Window(const Config& config) : m_config(config) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return;
    }
    create(config);
}

Window::~Window() {
    destroy();
    glfwTerminate();
}

Window::Window(Window&& other) noexcept 
    : m_window(std::exchange(other.m_window, nullptr))
    , m_config(std::move(other.m_config))
    , m_width(other.m_width)
    , m_height(other.m_height)
    , m_initialized(other.m_initialized)
    , m_glad_loaded(other.m_glad_loaded)
    , m_key_callback(std::move(other.m_key_callback))
    , m_size_callback(std::move(other.m_size_callback))
    , m_mouse_button_callback(std::move(other.m_mouse_button_callback))
    , m_cursor_pos_callback(std::move(other.m_cursor_pos_callback)) {
    other.m_initialized = false;
    other.m_glad_loaded = false;
}

Window& Window::operator=(Window&& other) noexcept {
    if (this != &other) {
        destroy();
        
        m_window = std::exchange(other.m_window, nullptr);
        m_config = std::move(other.m_config);
        m_width = other.m_width;
        m_height = other.m_height;
        m_initialized = other.m_initialized;
        m_glad_loaded = other.m_glad_loaded;
        m_key_callback = std::move(other.m_key_callback);
        m_size_callback = std::move(other.m_size_callback);
        m_mouse_button_callback = std::move(other.m_mouse_button_callback);
        m_cursor_pos_callback = std::move(other.m_cursor_pos_callback);
        
        other.m_initialized = false;
        other.m_glad_loaded = false;
    }
    return *this;
}

bool Window::initialize() {
    if (m_initialized) return true;

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    return true;
}

bool Window::create(const Config& config) {
    m_config = config;
    m_width = config.width;
    m_height = config.height;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, config.gl_major_version);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, config.gl_minor_version);
    glfwWindowHint(GLFW_OPENGL_PROFILE, config.core_profile ? GLFW_OPENGL_CORE_PROFILE : GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, config.resizable);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWmonitor* monitor = config.fullscreen ? glfwGetPrimaryMonitor() : nullptr;
    m_window = glfwCreateWindow(m_width, m_height, config.title.c_str(), monitor, nullptr);
    
    if (!m_window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        return false;
    }

    glfwMakeContextCurrent(m_window);
    
    if (!m_glad_loaded) {
        if (!gladLoadGL()) {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            return false;
        }
        m_glad_loaded = true;
    }

    glfwSwapInterval(config.vsync ? 1 : 0);

    update_viewport();

    setup_callbacks();
L
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GPU: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;

    m_initialized = true;
    return true;
}

void Window::destroy() {
    if (m_window) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
    m_initialized = false;
}

bool Window::should_close() const {
    return m_window ? glfwWindowShouldClose(m_window) : true;
}

void Window::swap_buffers() {
    if (m_window) {
        glfwSwapBuffers(m_window);
    }
}

void Window::poll_events() {
    glfwPollEvents();
}

void Window::clear(const glm::vec4& color) {
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::set_should_close(bool value) {
    if (m_window) {
        glfwSetWindowShouldClose(m_window, value);
    }
}

void Window::set_key_callback(const KeyCallback& callback) {
    m_key_callback = callback;
}

void Window::set_size_callback(const SizeCallback& callback) {
    m_size_callback = callback;
}

void Window::set_mouse_button_callback(const MouseButtonCallback& callback) {
    m_mouse_button_callback = callback;
}

void Window::set_cursor_pos_callback(const CursorPosCallback& callback) {
    m_cursor_pos_callback = callback;
}

void Window::set_title(const std::string& title) {
    if (m_window) {
        glfwSetWindowTitle(m_window, title.c_str());
        m_config.title = title;
    }
}

void Window::set_size(int width, int height) {
    if (m_window) {
        glfwSetWindowSize(m_window, width, height);
        m_width = width;
        m_height = height;
        update_viewport();
    }
}

void Window::set_vsync(bool enabled) {
    if (m_window) {
        glfwSwapInterval(enabled ? 1 : 0);
        m_config.vsync = enabled;
    }
}

void Window::set_fullscreen(bool fullscreen) {
    if (!m_window) return;

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    if (fullscreen && !m_config.fullscreen) {
        glfwSetWindowMonitor(m_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    } else if (!fullscreen && m_config.fullscreen) {
        glfwSetWindowMonitor(m_window, nullptr, 100, 100, m_config.width, m_config.height, 0);
    }

    m_config.fullscreen = fullscreen;
}

void Window::setup_callbacks() {
    if (!m_window) return;

    glfwSetWindowUserPointer(m_window, this);

    glfwSetKeyCallback(m_window, glfw_key_callback);
    glfwSetWindowSizeCallback(m_window, glfw_window_size_callback);
    glfwSetMouseButtonCallback(m_window, glfw_mouse_button_callback);
    glfwSetCursorPosCallback(m_window, glfw_cursor_pos_callback);
}

void Window::update_viewport() {
    glViewport(0, 0, m_width, m_height);
}

void Window::glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win && win->m_key_callback) {
        win->m_key_callback(key, scancode, action, mods);
    }
}

void Window::glfw_window_size_callback(GLFWwindow* window, int width, int height) {
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win) {
        win->m_width = width;
        win->m_height = height;
        win->update_viewport();
        if (win->m_size_callback) {
            win->m_size_callback(width, height);
        }
    }
}

void Window::glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win && win->m_mouse_button_callback) {
        win->m_mouse_button_callback(button, action, mods);
    }
}

void Window::glfw_cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win && win->m_cursor_pos_callback) {
        win->m_cursor_pos_callback(xpos, ypos);
    }
}