//engine.h

#ifndef ENGINE_H
#define ENGINE_H

#include <chrono>

class Engine {
public:
    Engine();
    //~Engine();

    void set_start_time(std::chrono::time_point<std::chrono::high_resolution_clock> value);
    std::chrono::time_point<std::chrono::high_resolution_clock> get_start_time() const;

    void set_last_time(std::chrono::time_point<std::chrono::high_resolution_clock> value);
    std::chrono::time_point<std::chrono::high_resolution_clock> get_last_time() const;

    void process();
    
    float get_delta_time() const;
    float get_fps() const;
    
    void set_target_fps(float fps);
    float get_target_fps() const { return 1 / target_frame_time; }

    int get_frame_count() const { return frame_count; }
    //auto get_start_time() const { return start_time; }
    
private:
    float delta_time;
    float target_frame_time;
    float current_fps;
    
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    std::chrono::time_point<std::chrono::high_resolution_clock> last_time;
    
    int frame_count;
    std::chrono::time_point<std::chrono::high_resolution_clock> fps_start_time;
};

#endif