#include <chrono>

class Engine {
    private:
    float delta_time;
    

    public:
    Engine();
    ~Engine();

    float get_delta_time() const {};
    float get_fps() const {};
};