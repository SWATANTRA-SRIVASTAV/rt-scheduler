#include "Scheduler.h"
#include "Event.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>

int main() {
    std::cout << "=== rt-scheduler: multithreaded event scheduler ===\n\n";

    Scheduler scheduler;

    // Track how many times each event actually fired
    std::atomic<int> heartbeat_count{0};
    std::atomic<int> sensor_count{0};
    std::atomic<int> watchdog_count{0};
    std::atomic<int> status_count{0};

    scheduler.addEvent(Event("heartbeat",  std::chrono::milliseconds(500),  6,
        [&]() { heartbeat_count++; }));

    scheduler.addEvent(Event("sensor_poll", std::chrono::milliseconds(1000), 3,
        [&]() { sensor_count++; }));

    scheduler.addEvent(Event("status_log", std::chrono::milliseconds(1500), 2,
        [&]() { status_count++; }));

    scheduler.addEvent(Event("watchdog",   std::chrono::milliseconds(2000), 2,
        [&]() { watchdog_count++; }));

    scheduler.start();
    std::this_thread::sleep_for(std::chrono::seconds(4));
    scheduler.stop();
    scheduler.join();

    std::cout << "\n=== results ===\n";
    std::cout << "heartbeat  fired: " << heartbeat_count.load()
              << " (expected 6)\n";
    std::cout << "sensor_poll fired: " << sensor_count.load()
              << " (expected 3)\n";
    std::cout << "status_log fired: " << status_count.load()
              << " (expected 2)\n";
    std::cout << "watchdog   fired: " << watchdog_count.load()
              << " (expected 2)\n";
    return 0;
}
