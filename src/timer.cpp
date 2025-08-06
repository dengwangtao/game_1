#include "timer.h"
#include "game.h"

Timer::Timer()
{
}

Timer::~Timer()
{
}

s64 Timer::addTimer(s64 delay, const TimerFunc &func)
{
    s64 now_ms = G_GAME.now_ms();
    u64 timer_id = nextTimerId_ ++;
    TimerItem timerItem{now_ms + delay, timer_id, func};
    timers_.push(std::move(timerItem));
    return timer_id;
}

s32 Timer::removeTimer(s64 timerId)
{
    deletedTimers_.insert(timerId);
    return 0;
}

s32 Timer::update(s64 now_ms)
{
    while (! timers_.empty() && timers_.top().expireTime_ <= now_ms)
    {
        auto& timerItem = timers_.top();
        if (deletedTimers_.count(timerItem.timerId_) > 0)
        {
            timers_.pop();
            continue;
        }
        if (timerItem.func_)
        {
            timerItem.func_(); // 执行回调
        }
        LOG_INFO("timer:(expireTime:%lld, timerId:%llu) expried", timerItem.expireTime_, timerItem.timerId_);
        timers_.pop();
    }

    return 0;
}