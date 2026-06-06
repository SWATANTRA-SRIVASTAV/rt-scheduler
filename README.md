# rt-scheduler

A multithreaded event scheduler in C++17 using POSIX threads directly.

## What it does

You register named events with an interval and a repeat count. The scheduler spawns one `pthread` per event. Each thread sleeps until its next fire time using `std::this_thread::sleep_until` (which accumulates less drift than repeated `sleep_for` calls), executes the callback, and records how far the actual wake time deviated from the target.

## Why pthreads instead of std::thread

`std::thread` is fine for most use cases. I used `pthread_create` directly because I wanted to understand what `std::thread` wraps — specifically the thread attribute API (`pthread_attr_t`), stack size control, and the explicit join semantics. The scheduler works correctly with either; the choice was deliberate for learning purposes.

## Thread ownership and memory safety

`ThreadArgs` is allocated with `std::make_unique`, then `release()`d into the `pthread` via `void*`. The thread immediately re-wraps the raw pointer in a `unique_ptr`, so the memory is freed on any exit path. This is the standard pattern for passing owned data across the POSIX thread API boundary.

## Drift measurement

`sleep_until` is not a real-time guarantee on a non-RTOS. On macOS, the scheduler is preemptible and the kernel does not guarantee wake times. The scheduler measures actual vs expected wake time in microseconds and prints it per-fire:
[19:46:52] heartbeat  fired=1  drift=5046us
[19:46:53] sensor_poll fired=1  drift=5046us
Typical drift on macOS is 3–5ms. On a real-time kernel (PREEMPT_RT Linux) with `SCHED_FIFO`, this drops to tens of microseconds. The measurement is there to make the gap between "precise timing" as a claim and "precise timing" as a measured property visible.

## Build

```bash
mkdir build && cd build
cmake ..
make
./rt_scheduler
```

## Tests

```bash
cd build && ctest --output-on-failure
```

6 tests: event registration, multiple events, callback execution count, clean shutdown of an infinite-repeat event, name storage, interval storage.

## Known limitations

The stop flag is `std::atomic<bool>`. Threads check it once per loop iteration, after waking from sleep. This means a thread can fire one more time after `stop()` is called if it woke up between the stop call and the flag check. For the current use case this is acceptable. A condition variable would allow immediate cancellation but adds complexity that isn't justified here.
