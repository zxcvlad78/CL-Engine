//engine.cpp

#include "engine.h"
#include <thread>

Engine::Engine() 
    : delta_time(0.0f)
    , target_frame_time(0.016666666666f)
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
    
    // Всегда считаем delta_time от реального последнего кадра
    delta_time = std::chrono::duration<float>(current_time - last_time).count();
    
    // Ограничение FPS только если нужно
    if (target_frame_time > 0.0f) {
        // Считаем, сколько времени уже прошло с начала кадра
        float elapsed = std::chrono::duration<float>(current_time - last_time).count();
        
        // Сколько нужно еще подождать
        float sleep_needed = target_frame_time - elapsed;
        
        if (sleep_needed > 0.001f) { // Только если нужно ждать >1ms
            // Первая часть: точный сон
            if (sleep_needed > 0.002f) { // >2ms - используем sleep
                std::this_thread::sleep_for(std::chrono::duration<float>(
                    sleep_needed * 0.9f)); // Спим 90% времени
            }
            
            // Вторая часть: busy-wait для точности
            auto wake_time = last_time + std::chrono::duration<float>(target_frame_time);
            while (std::chrono::high_resolution_clock::now() < wake_time) {
                std::this_thread::yield();
            }
            
            current_time = std::chrono::high_resolution_clock::now();
        }
    }
    
    // Пересчитываем delta_time после ограничения FPS
    delta_time = std::chrono::duration<float>(current_time - last_time).count();
    last_time = current_time;
    
    frame_count++;
    
    // Обновление статистики FPS (раз в секунду)
    auto elapsed = std::chrono::duration<float>(current_time - fps_start_time).count();
    if (elapsed >= 1.0f) {
        current_fps = static_cast<float>(frame_count) / elapsed;
        frame_count = 0;
        fps_start_time = current_time;
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

void Engine::set_target_fps(float fps) {
    if (fps > 0.0f) {
        target_frame_time = 1.0f / fps;
    } else {
        target_frame_time = 0.0f;
    }
}

float Engine::get_fps() const {
    return current_fps;
}