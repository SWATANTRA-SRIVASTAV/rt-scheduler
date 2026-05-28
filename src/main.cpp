#include "Scheduler.h"
#include "Event.h"
#include <iostream>
#include <chrono>
#include <thread>

int main() {
    std::cout << "=== RT-Scheduler: Real-Time Multithreaded Event Scheduler ===\n";
    std::cout << "Simulating CERN-style timing pulse dispatch...\n\n";

    Scheduler scheduler;

    // Event 1: Heartbeat pulse every 500ms, fires 6 times
    scheduler.addEvent(Event("HeartbeatPulse", std::chrono::milliseconds(500), 6,
        []() { std::cout << "  >> Pulse sync signal dispatched.\n"; }));

    // Event 2: Sensor poll every 1s, fires 3 times
    scheduler.addEvent(Event("SensorPoll", std::chrono::milliseconds(1000), 3,
        []() { std::cout << "  >> Sensor data acquired.\n"; }));

    // Event 3: Status log every 1.5s, fires 2 times
    scheduler.addEvent(Event("StatusLog", std::chrono::milliseconds(1500), 2,
        []() { std::cout << "  >> System status: NOMINAL.\n"; }));

    // Event 4: Watchdog check every 2s, fires 2 times
    scheduler.addEvent(Event("WatchdogCheck", std::chrono::milliseconds(2000), 2,
        []() { std::cout << "  >> Watchdog: all threads alive.\n"; }));

    scheduler.start();

    // Run for 4 seconds then cleanly shut down
    std::this_thread::sleep_for(std::chrono::seconds(4));
    scheduler.stop();
    scheduler.join();

    std::cout << "\n=== Scheduler stopped cleanly. All threads joined. ===\n";
    return 0;
}
