#pragma once
#include <string>
#include <functional>
#include <chrono>

class Event {
public:
    std::string name;
    std::chrono::milliseconds interval;   // how often it fires
    int repeat_count;                     // -1 = infinite
    std::function<void()> callback;       // what to execute

    Event(const std::string& name,
          std::chrono::milliseconds interval,
          int repeat_count,
          std::function<void()> callback);

    std::string getName() const;
    std::chrono::milliseconds getInterval() const;
    int getRepeatCount() const;
    void execute() const;
};
