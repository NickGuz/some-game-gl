#include "Timer.h"

bool Timer::is_stopped() {
    return state == STOPPED;
}

bool Timer::is_paused() {
    return state == PAUSED;
}

bool Timer::has_run() {
    return num_runs > 0;
}

void Timer::start() {
    state = ACTIVE;
    time_left = total_time;
}

void Timer::reset() {
    time_left = total_time;
    num_runs = 0;
}

void Timer::pause() {
    state = PAUSED;
}

void Timer::update(float dt) {
    if (state == ACTIVE) {
        time_left -= dt * 1000.0;

        if (time_left <= 0) {
            time_left = 0.0;
            end();
        }
    }
}

void Timer::end() {
    state = STOPPED;
    num_runs++;

    // call the end function if there is one
    if (on_end != nullptr) {
        // idk how to do this yet
        on_end();
    }

    // loop if we want to
    if (loop) {
        state = ACTIVE;
        time_left = total_time;
    }
}
