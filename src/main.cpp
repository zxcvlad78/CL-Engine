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
#include "renderer/material/material.h"
#include "renderer/shader/shader.h"

int main() {
    std::cout << "Starting CL Engine..." << std::endl;

    Engine engine;
    engine.set_target_fps(60.0f);
    
    ResourceManager resource_manager;
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
    
    // SceneTree
    auto scene_tree = std::make_shared<SceneTree>("MainSceneTree");
    
    // CanvasLayer
    auto canvas_layer = std::make_shared<CanvasLayer>("MainCanvasLayer");
    canvas_layer->set_layer(0);
    
    
    // Text Shader
    auto label_shader = std::make_shared<Shader>();
    label_shader->load_from_files(
        ResourceManager::get_absolute_path("res/shaders/text/vertex.txt"),
        ResourceManager::get_absolute_path("res/shaders/text/fragment.txt")
    );
    // Label Material
    auto label_material = std::make_shared<Material>();
    label_material->set_shader(label_shader);

    // FPS Label
    auto fps_label = std::make_shared<Label>("FPSLabel");
    fps_label->set_material(label_material);

    fps_label->set_font( ResourceManager::get_absolute_path("res/fonts/default.otf"));
    fps_label->set_font_size(24);
    fps_label->set_position({250.0f, 250.0f});
    fps_label->set_text_color({1.0f, 1.0f, 1.0f, 1.0f});
    fps_label->set_size({200.0f, 30.0f});
    fps_label->set_text("FPS: 0");
    
    
    // Texture1
    auto texture = Texture::load(ResourceManager::get_absolute_path("res/textures/icon.png"));
    texture->set_filter_mag(GL_NEAREST);
    texture->set_filter_min(GL_NEAREST);
    // TextureRect2
    auto texture_rect = std::make_shared<TextureRect>("TextureRect");
    texture_rect->set_position({900.0f, 100.0f});
    texture_rect->set_size({200.0f, 200.0f});
    texture_rect->set_texture(texture);
    
    auto texture2 = Texture::load(ResourceManager::get_absolute_path("res/textures/lt.jpg"));
    texture2->set_filter_mag(GL_NEAREST);
    texture2->set_filter_min(GL_NEAREST);
    // TextureRect2
    auto texture_rect2 = std::make_shared<TextureRect>("TextureRect2");
    texture_rect2->set_position({400.0f, 300.0f});
    texture_rect2->set_size({400.0f, 400.0f});
    texture_rect2->set_texture(texture2);
    texture_rect2->set_stretch_mode(TextureRect::STRETCH_KEEP_ASPECT_CENTERED);


    // Add items
    canvas_layer->add_item(fps_label.get()); 
    canvas_layer->add_item(texture_rect.get());
    canvas_layer->add_item(texture_rect2.get());
    
    // Viewport
    auto viewport = std::make_shared<Viewport>("MainViewport");
    viewport->set_size({config.width, config.height});
    viewport->set_background_color({0.2f, 0.2, 0.2f, 1.0f});
    viewport->add_layer(canvas_layer.get());
    
    scene_tree->add_viewport(viewport);
    scene_tree->set_current_scene(canvas_layer);
    
    viewport->enter_tree();
    viewport->ready();

    while (!window.should_close()) {
        engine.process();
        
        static int frame_count = 0;
        static float time_accumulator = 0.0f;
        time_accumulator += engine.get_delta_time();
        frame_count++;
        
        if (time_accumulator >= 1.0f) {
            float fps = frame_count / time_accumulator;
            fps_label->set_text("FPS: " + std::to_string(static_cast<int>(fps)));
            frame_count = 0;
            time_accumulator = 0.0f;
            
        }
        
        window.poll_events();
        
        window.clear(glm::vec4(0.4f, 0.4f, 0.4f, 1.0f));
        
        scene_tree->process(engine.get_delta_time());
        
        scene_tree->render();

        window.swap_buffers();
    
    }
    
    std::cout << "Shutting down..." << std::endl;
    
    return 0;
}