#include "Scheduler.h"
#include "Event.h"
#include <gtest/gtest.h>
#include <atomic>
#include <chrono>
#include <thread>

// Test 1: Events are added correctly
TEST(SchedulerTest, AddEvent) {
    Scheduler s;
    s.addEvent(Event("TestEvent", std::chrono::milliseconds(100), 1, [](){}));
    EXPECT_EQ(s.getEventCount(), 1);
}

// Test 2: Multiple events tracked
TEST(SchedulerTest, MultipleEvents) {
    Scheduler s;
    s.addEvent(Event("E1", std::chrono::milliseconds(100), 1, [](){}));
    s.addEvent(Event("E2", std::chrono::milliseconds(200), 1, [](){}));
    EXPECT_EQ(s.getEventCount(), 2);
}

// Test 3: Event actually executes its callback
TEST(SchedulerTest, CallbackFires) {
    std::atomic<int> counter{0};
    Scheduler s;
    s.addEvent(Event("Counter", std::chrono::milliseconds(100), 3,
        [&counter]() { counter++; }));

    s.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(450));
    s.stop();
    s.join();

    EXPECT_EQ(counter.load(), 3);
}

// Test 4: Scheduler stops cleanly (no hang)
TEST(SchedulerTest, CleanStop) {
    Scheduler s;
    s.addEvent(Event("Infinite", std::chrono::milliseconds(50), -1, [](){}));
    s.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    s.stop();
    s.join();   // must not hang
    SUCCEED();
}

// Test 5: Event name is stored correctly
TEST(EventTest, NameStored) {
    Event e("MyEvent", std::chrono::milliseconds(500), 1, [](){});
    EXPECT_EQ(e.getName(), "MyEvent");
}

// Test 6: Interval is stored correctly
TEST(EventTest, IntervalStored) {
    Event e("E", std::chrono::milliseconds(250), 1, [](){});
    EXPECT_EQ(e.getInterval().count(), 250);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
