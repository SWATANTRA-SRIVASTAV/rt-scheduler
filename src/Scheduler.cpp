#include "Scheduler.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <memory>

Scheduler::Scheduler() : stop_flag_(false) {}
Scheduler::~Scheduler() { stop(); join(); }

void Scheduler::addEvent(const Event& event) {
    events_.push_back(event);
}

int Scheduler::getEventCount() const {
    return static_cast<int>(events_.size());
}

void Scheduler::start() {
    stop_flag_ = false;
    for (const auto& event : events_) {
        // ThreadArgs lifetime: owned by unique_ptr, released into the thread
        // via raw pointer. The thread takes exclusive ownership at that point
        // and frees it. This avoids manual new/delete while keeping the
        // pthread C API happy (it takes void*).
        auto args = std::make_unique<ThreadArgs>(ThreadArgs{event, &stop_flag_});
        pthread_t tid;
        pthread_create(&tid, nullptr, workerThread, args.release());
        threads_.push_back(tid);
    }
}

void Scheduler::stop() { stop_flag_ = true; }

void Scheduler::join() {
    for (auto& tid : threads_) pthread_join(tid, nullptr);
    threads_.clear();
}

void* Scheduler::workerThread(void* raw_args) {
    // Re-acquire ownership immediately so it's freed on any exit path
    std::unique_ptr<ThreadArgs> args(static_cast<ThreadArgs*>(raw_args));
    Event event            = args->event;
    std::atomic<bool>* stop = args->stop_flag;
    // args goes out of scope here — memory released cleanly

    int fired   = 0;
    int repeats = event.getRepeatCount();

    using Clock = std::chrono::steady_clock;

    while (!stop->load()) {
        auto target = Clock::now() + event.getInterval();
        std::this_thread::sleep_until(target);
        if (stop->load()) break;

        // Measure actual vs expected interval and accumulate drift
        auto actual_wake  = Clock::now();
        auto drift_us     = std::chrono::duration_cast<std::chrono::microseconds>(
                                actual_wake - target).count();

        auto now_sys = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now_sys);
        char timebuf[20];
        std::strftime(timebuf, sizeof(timebuf), "%H:%M:%S", std::localtime(&t));

        std::cout << "[" << timebuf << "] "
                  << event.getName()
                  << "  fired=" << (fired + 1)
                  << "  drift=" << drift_us << "us\n"
                  << std::flush;

        event.execute();
        fired++;
        if (repeats != -1 && fired >= repeats) break;
    }
    return nullptr;
}
