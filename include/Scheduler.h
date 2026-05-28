#pragma once
#include "Event.h"
#include <vector>
#include <pthread.h>
#include <atomic>

// Passed to each worker thread
struct ThreadArgs {
    Event event;
    std::atomic<bool>* stop_flag;
};

class Scheduler {
public:
    Scheduler();
    ~Scheduler();

    void addEvent(const Event& event);
    void start();   // launches all threads
    void stop();    // signals all threads to stop
    void join();    // waits for all threads to finish
    int getEventCount() const;

private:
    std::vector<Event> events_;
    std::vector<pthread_t> threads_;
    std::atomic<bool> stop_flag_;

    static void* workerThread(void* args);
};
