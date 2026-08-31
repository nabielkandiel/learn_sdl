#pragma once

#include <cstdint>

class Timer
{
  private:
    uint64_t timer_start{0};
    uint64_t timer_paused{0};
    bool started{false};
    bool paused{false};

  public:
    Timer(const Timer &) = delete;
    Timer(Timer &&) noexcept = delete;
    Timer &operator=(const Timer &) = delete;
    Timer &operator=(Timer &&) noexcept = delete;

    Timer() = default;
    ~Timer() = default;

    void start();
    void stop();
    void pause();
    void unpause();
    [[nodiscard]] bool isStarted() const;
    [[nodiscard]] bool isPaused() const;
    [[nodiscard]] uint64_t getTicksNS() const;
};