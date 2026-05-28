#include "Event.h"

Event::Event(const std::string& name,
             std::chrono::milliseconds interval,
             int repeat_count,
             std::function<void()> callback)
    : name(name),
      interval(interval),
      repeat_count(repeat_count),
      callback(callback) {}

std::string Event::getName() const { return name; }
std::chrono::milliseconds Event::getInterval() const { return interval; }
int Event::getRepeatCount() const { return repeat_count; }
void Event::execute() const { if (callback) callback(); }
