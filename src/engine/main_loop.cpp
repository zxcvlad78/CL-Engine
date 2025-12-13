#include "main_loop.h"
#include "window/window.h"
#include "viewport/viewport.h"
#include "scene/scene_tree.h"
#include <chrono>
#include <thread>
#include <iostream>

MainLoop::MainLoop() {
    std::cout << "MainLoop created" << std::endl;
}

MainLoop::~MainLoop() {
    cleanup();
    std::cout << "MainLoop destroyed" << std::endl;
}

bool MainLoop::initialize() {
    std::cout << "MainLoop initializing..." << std::endl;
    return true;
}

void MainLoop::cleanup() {
    std::cout << "MainLoop cleaning up..." << std::endl;
    m_window.reset();
    m_main_viewport.reset();
    m_scene_tree.reset();
}

void MainLoop::set_window(std::shared_ptr<Window> window) {
    m_window = window;
}

void MainLoop::set_main_viewport(std::shared_ptr<Viewport> viewport) {
    m_main_viewport = viewport;
}

void MainLoop::set_scene_tree(std::shared_ptr<SceneTree> scene_tree) {
    m_scene_tree = scene_tree;
}

void MainLoop::process() {
    static auto last_time = std::chrono::high_resolution_clock::now();
    auto current_time = std::chrono::high_resolution_clock::now();
    
    // Вычисляем delta time
    m_delta_time = std::chrono::duration<float>(current_time - last_time).count();
    last_time = current_time;
    
    // Ограничиваем delta time
    if (m_delta_time > 0.1f) {
        m_delta_time = 0.1f;
    }
    
    // Обрабатываем события окна
    if (m_window) {
        m_window->poll_events();
    }
    
    // Обрабатываем логику сцены
    if (m_scene_tree) {
        m_scene_tree->process(m_delta_time);
    }
    
    // Рассчитываем FPS
    calculate_fps();
}

void MainLoop::physics_process() {
    // Физика должна работать с фиксированным шагом времени
    static float accumulator = 0.0f;
    const float physics_delta = 1.0f / 60.0f; // 60 FPS для физики
    
    accumulator += m_delta_time;
    
    while (accumulator >= physics_delta) {
        if (m_scene_tree) {
            m_scene_tree->physics_process(physics_delta);
        }
        accumulator -= physics_delta;
    }
}

void MainLoop::render() {
    // Очищаем окно
    if (m_window) {
        m_window->clear();
    }
    
    // Рендерим основной viewport
    if (m_main_viewport) {
        m_main_viewport->render();
    }
    
    // Рендерим сцену
    if (m_scene_tree) {
        m_scene_tree->render();
    }
    
    // Отображаем результат
    if (m_window) {
        m_window->swap_buffers();
    }
    
    // Ограничение FPS
    if (m_target_fps > 0) {
        static auto last_frame_time = std::chrono::high_resolution_clock::now(); // Исправлено
        auto end_time = std::chrono::high_resolution_clock::now();
        float elapsed = std::chrono::duration<float>(end_time - last_frame_time).count(); // Исправлено
        
        if (elapsed < m_target_fps) {
            float sleep_time = (1.0f / m_target_fps) - elapsed; // Исправлено
            if (sleep_time > 0.001f) {
                std::this_thread::sleep_for(std::chrono::duration<float>(sleep_time));
            }
        }
        last_frame_time = end_time; // Добавлено
    }
    
}

bool MainLoop::should_quit() const {
    if (m_should_quit) {
        return true;
    }
    
    if (m_window && m_window->should_close()) {
        return true;
    }
    
    if (m_scene_tree) {
        // Проверяем, не запросил ли SceneTree выход
        // (нужно добавить метод в SceneTree)
    }
    
    return false;
}

void MainLoop::quit() {
    m_should_quit = true;
    std::cout << "MainLoop quit requested" << std::endl;
    
    if (m_scene_tree) {
        m_scene_tree->quit();
    }
}

void MainLoop::calculate_fps() {
    static int frame_count = 0;
    static float time_accumulator = 0.0f;
    
    frame_count++;
    time_accumulator += m_delta_time;
    
    if (time_accumulator >= 1.0f) {
        m_fps = static_cast<float>(frame_count) / time_accumulator;
        frame_count = 0;
        time_accumulator = 0.0f;
        
        // Вывод FPS (можно убрать в релизе)
        std::cout << "FPS: " << m_fps << std::endl;
    }
}