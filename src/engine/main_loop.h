#ifndef MAIN_LOOP_H
#define MAIN_LOOP_H

#include <memory>
#include <vector>

class Window;
class Viewport;
class SceneTree;

class MainLoop {
public:
    MainLoop();
    ~MainLoop();
    
    bool initialize();
    void cleanup();
    
    void set_window(std::shared_ptr<Window> window);
    void set_main_viewport(std::shared_ptr<Viewport> viewport);
    void set_scene_tree(std::shared_ptr<SceneTree> scene_tree);
    
    void process();
    void physics_process();
    void render();
    
    bool should_quit() const;
    void quit();
    
    float get_delta_time() const { return m_delta_time; }
    float get_fps() const { return m_fps; }
    
private:
    std::shared_ptr<Window> m_window;
    std::shared_ptr<Viewport> m_main_viewport;
    std::shared_ptr<SceneTree> m_scene_tree;
    
    float m_delta_time = 0.0f;
    float m_fps = 0.0f;
    float m_target_fps = 60.0f;
    bool m_should_quit = false;
    
    void calculate_fps();
};

#endif // MAIN_LOOP_H