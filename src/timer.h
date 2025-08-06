#pragma once


#include "singleton.h"
#include "comm_def.h"
#include <queue>
#include <functional>
#include <unordered_set>

class Timer : public Singleton<Timer>
{
public:
    Timer();
    ~Timer();

    
    using TimerFunc = std::function<void()>;

    s64 addTimer(s64 delay, const TimerFunc& func);

    s32 removeTimer(s64 timerId);

    s32 update(s64 now_ms);

private:
    struct TimerItem
    {
        s64 expireTime_;
        u64 timerId_;
        TimerFunc func_;
        bool operator<(const TimerItem& rhs) const
        {
            return expireTime_ < rhs.expireTime_;
        }
        bool operator>(const TimerItem& rhs) const
        {
            return expireTime_ > rhs.expireTime_;
        }
    };

    u64 nextTimerId_ = 0;

    std::priority_queue<TimerItem, std::vector<TimerItem>, std::greater<TimerItem>> timers_; // 小顶堆
    std::unordered_set<u64> deletedTimers_; // 标记删除的timer
};

#define G_TIMER Timer::Instance()