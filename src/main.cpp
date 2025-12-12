#include <iostream>
#include <memory>
#include "window/window.h"
#include "scene/scene_tree.h"
#include "viewport/viewport.h"
#include "canvas_layer/canvas_layer.h"
#include "control/label/label.h"
#include "control/texture_rect/texture_rect.h"
#include "control/button/button.h"
#include "engine/engine.h"
#include "resources/resource_manager.h"

int main() {
    std::cout << "Starting CL Engine..." << std::endl;
    
    // Инициализация движка
    Engine engine;
    engine.set_target_fps(60.0f);
    
    ResourceManager resource_manager;
    // Создаем окно
    Window::Config config;
    config.title = "CL Engine";
    config.width = 1280;
    config.height = 720;
    config.vsync = true;
    
    Window window;
    if (!window.create(config)) {
        std::cerr << "Failed to create window" << std::endl;
        return -1;
    }
    
    std::cout << "Window created successfully" << std::endl;
    
    // Создаем SceneTree
    auto scene_tree = std::make_shared<SceneTree>("MainSceneTree");
    
    // Создаем CanvasLayer
    auto canvas_layer = std::make_shared<CanvasLayer>("MainCanvasLayer");
    canvas_layer->set_layer(0);
    
    // Создаем Label для FPS
    auto fps_label = std::make_shared<Label>("FPSLabel");
    fps_label->set_text("FPS: 0");
    
    fps_label->set_font_size(24);
    fps_label->set_position({250.0f, 250.0f});
    fps_label->set_text_color({1.0f, 1.0f, 1.0f, 1.0f});
    fps_label->set_size({200.0f, 30.0f});
    fps_label->set_font( ResourceManager::get_absolute_path("res/fonts/default.otf"));
    
    // Создаем кнопку
    auto button = std::make_shared<Button>("TestButton");
    button->set_text("Click Me!");
    button->set_position({100.0f, 100.0f});
    button->set_size({200.0f, 50.0f});
    button->connect_pressed([]() {
        std::cout << "=== Button was pressed! ===" << std::endl;
    });
    
    // Создаем TextureRect
    auto texture_rect = std::make_shared<TextureRect>("TextureRect");
    texture_rect->set_position({100.0f, 100.0f});
    texture_rect->set_size({200.0f, 200.0f});
    
    auto texture = Texture::load(ResourceManager::get_absolute_path("res/textures/icon.png") ); //("Z:/dev/cpp/CL-Engine/res/textures/icon.png");
    texture->set_filter_mag(GL_NEAREST);
    texture->set_filter_min(GL_NEAREST);
    

    texture_rect->set_texture(texture);

    canvas_layer->add_item(fps_label.get()); 
    canvas_layer->add_item(button.get());
    canvas_layer->add_item(texture_rect.get());
    
    // Создаем Viewport
    auto viewport = std::make_shared<Viewport>("MainViewport");
    viewport->set_size({config.width, config.height});
    viewport->set_background_color({0.2f, 0.2, 0.2f, 1.0f});
    viewport->add_layer(canvas_layer.get());
    
    // Добавляем viewport в SceneTree
    scene_tree->add_viewport(viewport);
    
    // Устанавливаем текущую сцену
    scene_tree->set_current_scene(canvas_layer);
    
    // Вход в дерево
    viewport->enter_tree();
    viewport->ready();
    
    
    while (!window.should_close()) {
        // Обработка времени
        engine.process();
        
        // Обновляем FPS
        static int frame_count = 0;
        static float time_accumulator = 0.0f;
        time_accumulator += engine.get_delta_time();
        frame_count++;
        
        if (time_accumulator >= 1.0f) {
            float fps = frame_count / time_accumulator;
            fps_label->set_text("FPS: " + std::to_string(static_cast<int>(fps)));
            frame_count = 0;
            time_accumulator = 0.0f;
            std::cout << "FPS: " + std::to_string(static_cast<int>(fps)) << std::endl;
            
        }
        
        // Обработка событий
        window.poll_events();
        
        // Очистка экрана
        window.clear(glm::vec4(0.4f, 0.4f, 0.4f, 1.0f));
        
        // Обработка сцены
        scene_tree->process(engine.get_delta_time());
        
        // Рендеринг
        scene_tree->render();
        
        // Обмен буферов
        window.swap_buffers();
    
    }
    
    std::cout << "Shutting down..." << std::endl;
    
    return 0;
}