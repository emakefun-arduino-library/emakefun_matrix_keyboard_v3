#ifndef EMAKEFUN_DEBOUNCER_H_
#define EMAKEFUN_DEBOUNCER_H_

#include <Arduino.h>
#include <stdint.h>

namespace emakefun {
template <typename T>
class Debouncer {
 public:
  Debouncer(const T value) : debouncing_value_(value), last_value_(value) {
    start_debounce_time_ = millis();
  }

  void Start(const uint64_t debounce_duration_ms) {
    debounce_duration_ms_ = debounce_duration_ms;
    start_debounce_time_ = millis();
  }

  T Debounce(const T value) {
    if (value != debouncing_value_) {
      debouncing_value_ = value;
      start_debounce_time_ = millis();
    } else if (last_value_ != debouncing_value_ && millis() - start_debounce_time_ >= debounce_duration_ms_) {
      last_value_ = debouncing_value_;
    }

    return last_value_;
  }

 private:
  T debouncing_value_;
  T last_value_;
  uint64_t debounce_duration_ms_ = 10;
  uint64_t start_debounce_time_ = 0;
};
}  // namespace emakefun

#endif