#pragma once

#include <chrono>

class Timer {
    public:
    Timer(float& time);
    ~Timer();

    void start(float from_pos = 0.0) {}
    void update() {}

    //void set_wait_time(float& seconds) {}
    //float get_wait_time() const { return wait_time; }

    private:
    std::chrono::time_point<std::chrono::high_resolution_clock> last_time;
    float delta_time;

};