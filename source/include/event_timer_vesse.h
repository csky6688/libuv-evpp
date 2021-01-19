#ifndef __EVENT_TIMER_VESSE_H__
#define __EVENT_TIMER_VESSE_H__
#include <config.h>
#include <unordered_map>
#include <memory>
namespace Evpp
{
    class EventLoop;
    class EventTimer;
    class EventTimerVesse
    {
    public:
        explicit EventTimerVesse(EventLoop* loop);
        virtual ~EventTimerVesse();
    public:
        bool AssignTimer(const u96 index, const u64 delay, const u64 repeat);
        // TODO Close || Sotped;
    private:
        bool CreateTimer(const u96 index);
    private:
        EventLoop*                                                      event_loop;
        std::unordered_map<u96, std::shared_ptr<EventTimer>>            event_timer;
    };
}
#endif // __EVENT_TIMER_VESSE_H__