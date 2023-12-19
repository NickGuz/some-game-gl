#include <functional>
#include <iostream>
#include <chrono>

enum TimerState {
    STOPPED,
    PAUSED,
    ACTIVE
};

class Timer {
public:
    Timer(double time, std::function<void(void)> on_end = nullptr, bool loop = false)
    : total_time(time), time_left(0), on_end(on_end), loop(loop), state(STOPPED), num_runs(0)
    {}

    bool is_stopped();
    bool is_paused();
    bool has_run();

    void start();
    void reset();
    void pause();
    void end();

    void update(float dt);

private:
    TimerState state;
    std::function<void(void)> on_end;
    bool loop;
    double total_time;
    double time_left;
    int num_runs;

};
