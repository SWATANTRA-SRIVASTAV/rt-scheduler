#include "Scheduler.h"
#include <iostream>
#include <thread>
#include <chrono>

Scheduler::Scheduler() : stop_flag_(false) {}

Scheduler::~Scheduler() { stop(); join(); }

void Scheduler::addEvent(const Event& event) {
    events_.push_back(event);
}

int Scheduler::getEventCount() const {
    return static_cast<int>(events_.size());
}

// Each event gets its own POSIX thread
void Scheduler::start() {
    stop_flag_ = false;
    for (const auto& event : events_) {
        ThreadArgs* args = new ThreadArgs{event, &stop_flag_};
        pthread_t tid;
        pthread_create(&tid, nullptr, workerThread, args);
        threads_.push_back(tid);
    }
}

void Scheduler::stop() { stop_flag_ = true; }

void Scheduler::join() {
    for (auto& tid : threads_) pthread_join(tid, nullptr);
    threads_.clear();
}

// Static worker: fires the event on its interval, repeat_count times
void* Scheduler::workerThread(void* raw_args) {
    ThreadArgs* args = static_cast<ThreadArgs*>(raw_args);
    Event event = args->event;
    std::atomic<bool>* stop = args->stop_flag;
    delete args;

    int fired = 0;
    int repeats = event.getRepeatCount();  // -1 = run until stop

    while (!stop->load()) {
        std::this_thread::sleep_for(event.getInterval());
        if (stop->load()) break;

        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        char timebuf[20];
        std::strftime(timebuf, sizeof(timebuf), "%H:%M:%S", std::localtime(&t));

        std::cout << "[" << timebuf << "] EVENT fired: "
                  << event.getName() << "\n" << std::flush;

        event.execute();
        fired++;

        if (repeats != -1 && fired >= repeats) break;
    }
    return nullptr;
}
