//engine.cpp

#include "engine.h"
#include <thread>

Engine::Engine() 
    : delta_time(0.0f)
    , target_frame_time(0.1f)
    , current_fps(0.0f)
    , frame_count(0)
{
    auto now = std::chrono::high_resolution_clock::now();
    start_time = now;
    last_time = now;
    fps_start_time = now;
}


void Engine::process() {
    auto current_time = std::chrono::high_resolution_clock::now();
    
    delta_time = std::chrono::duration<float>(current_time - last_time).count();
    
    last_time = current_time;
    
    frame_count++;
    
    auto elapsed = std::chrono::duration<float>(current_time - fps_start_time).count();
    if (elapsed >= 1.0f) {
        current_fps = static_cast<float>(frame_count) / elapsed;
        frame_count = 0;
        fps_start_time = current_time;
    }
    
    if (target_frame_time > 0.0f && delta_time < target_frame_time) {
        float sleep_time = target_frame_time - delta_time;
        auto sleep_duration = std::chrono::duration<float>(sleep_time);
        std::this_thread::sleep_for(sleep_duration);
        
        auto after_sleep = std::chrono::high_resolution_clock::now();
        delta_time = std::chrono::duration<float>(after_sleep - last_time).count();
        last_time = after_sleep;
    }
}

void Engine::set_start_time(std::chrono::time_point<std::chrono::high_resolution_clock> time) {
    start_time = time;
}

std::chrono::time_point<std::chrono::high_resolution_clock> Engine::get_start_time() const { 
    return start_time; 
}

void Engine::set_last_time(std::chrono::time_point<std::chrono::high_resolution_clock> time) {
    last_time = time;
}

std::chrono::time_point<std::chrono::high_resolution_clock> Engine::get_last_time() const { 
    return last_time; 
}

float Engine::get_delta_time() const {
    return delta_time;
}

void Engine::set_target_fps(int fps) {
    if (fps > 0) {
        target_frame_time = 1.0f / static_cast<float>(fps);
    } else {
        target_frame_time = 0.0f;
    }
}

float Engine::get_fps() const {
    return current_fps;
}