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
#include "resources/resource_loader.h"
#include "renderer/material/material.h"
#include "renderer/shader/shader.h"
#include "control/color_rect/color_rect.h"

int main() {
    std::cout << "Starting CL Engine..." << std::endl;

    Engine engine;
    engine.set_target_fps(60.0f);
    
    ResourceLoader resource_loader;

    Window::Config config;
    config.title = "CL Engine";
    config.width = 1280;
    config.height = 720;
    config.vsync = false;
    
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
        ResourceLoader::get_absolute_path("res/shaders/text/vertex.txt"),
        ResourceLoader::get_absolute_path("res/shaders/text/fragment.txt")
    );
    // Label Material
    auto label_material = std::make_shared<Material>();
    label_material->set_shader(label_shader);

    // FPS Label
    auto fps_label = std::make_shared<Label>("FPSLabel");
    fps_label->set_material(label_material);

    fps_label->set_font( ResourceLoader::get_absolute_path("res/fonts/default.otf"));
    fps_label->set_font_size(24);
    fps_label->set_position({12.0f, 8.0f});
    fps_label->set_text_color({1.0f, 1.0f, 1.0f, 1.0f});
    fps_label->set_text("FPS: 0");

    // LS Texture )))
    auto ls_texture_rect = std::make_shared<TextureRect>();
    ls_texture_rect->set_texture( Texture::load("res/textures/lt.jpg") );

    ls_texture_rect->set_size( glm::vec2(150, 550) );
    ls_texture_rect->set_position( glm::vec2(150, 150) );

    // Test ColorRect
    auto color_rect = std::make_shared<ColorRect>();

    // Add items
    canvas_layer->add_item(fps_label.get()); 
    canvas_layer->add_item(ls_texture_rect.get());
    canvas_layer->add_item(color_rect.get());
    //canvas_layer->add_item();
    
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
        
        fps_label->set_text("FPS: " + std::to_string(static_cast<int>(engine.get_fps())));

        window.poll_events();
        
        window.clear(glm::vec4(0.4f, 0.4f, 0.4f, 1.0f));
        
        scene_tree->process(engine.get_delta_time());
        
        scene_tree->render();

        window.swap_buffers();
    }
    
    std::cout << "Shutting down..." << std::endl;
    
    return 0;
}