# rt-scheduler — Real-Time Multithreaded Event Scheduler in C++

A POSIX-thread-based real-time event scheduler demonstrating systems programming concepts relevant to precision timing systems (e.g., CERN White Rabbit).

## Features
- **Multithreaded** — each event runs on its own POSIX thread (`pthread`)
- **Precise timing** — uses `std::chrono` for interval control
- **OOP design** — clean `Event` / `Scheduler` separation
- **Unit tested** — 6 tests with Google Test
- **CMake build** — cross-platform build system

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

## Architecture
