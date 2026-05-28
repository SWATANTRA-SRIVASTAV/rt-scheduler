# rt-scheduler — Real-Time Multithreaded Event Scheduler in C++

A POSIX-thread-based real-time event scheduler demonstrating systems programming concepts relevant to precision timing systems (e.g., CERN White Rabbit).

## Architecture
Scheduler
├── addEvent(Event)   → registers timed events
├── start()           → spawns one pthread per event
├── stop()            → sets atomic stop flag
└── join()            → waits for all threads to finish
Event
├── name, interval, repeat_count
└── callback (std::function<void()>)
## Features
- **Multithreaded** — each event runs on its own POSIX thread (`pthread_create` / `pthread_join`)
- **Precise timing** — `std::chrono::milliseconds` interval control per event
- **Atomic shutdown** — `std::atomic<bool>` stop flag for race-free thread teardown
- **OOP design** — clean `Event` / `Scheduler` class separation
- **Unit tested** — 6 Google Test cases (100% passing, 1.41s)
- **CMake build** — auto-fetches GTest, cross-platform toolchain

## Build & Run

```bash
mkdir build && cd build
cmake ..
make
./rt_scheduler
```

## Run Tests

```bash
cd build
ctest --output-on-failure
```

## Sample Output
=== RT-Scheduler: Real-Time Multithreaded Event Scheduler ===
Simulating CERN-style timing pulse dispatch...
[10:32:16] EVENT fired: HeartbeatPulse


Pulse sync signal dispatched.
[10:32:17] EVENT fired: SensorPoll
Sensor data acquired.
[10:32:17] EVENT fired: StatusLog
System status: NOMINAL.
[10:32:18] EVENT fired: WatchdogCheck
Watchdog: all threads alive.


=== Scheduler stopped cleanly. All threads joined. ===
## Test Coverage

| Test | What it verifies |
|---|---|
| `AddEvent` | Single event registered correctly |
| `MultipleEvents` | Multiple events tracked independently |
| `CallbackFires` | Callback executes exact repeat count |
| `CleanStop` | Infinite event stops without hanging |
| `NameStored` | Event name stored correctly |
| `IntervalStored` | Interval stored correctly |

## Skills Demonstrated
C++17 · POSIX threads (pthreads) · std::chrono · std::atomic · OOP · CMake · Google Test · Real-Time Systems

## Relevance
Directly mirrors precision timing dispatch systems such as CERN's White Rabbit timing infrastructure, where events must fire on deterministic schedules across concurrent threads.
