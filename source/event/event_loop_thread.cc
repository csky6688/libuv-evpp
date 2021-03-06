#include <event_status.h>
#include <event_loop.h>
#include <event_share.h>
#include <event_loop_thread.h>
namespace Evpp
{
    EventLoopThread::EventLoopThread(const u96 index) : event_base(nullptr), event_index(index)
    {

    }

    EventLoopThread::EventLoopThread(EventLoop* loop, const std::shared_ptr<EventShare>& share, const u96 index) : 
        event_base(loop), 
        event_share(share), 
        event_index(index),
        event_thread_(std::make_unique<uv_thread_t>())
    {
        
    }

    EventLoopThread::~EventLoopThread()
    {
        Join();
    }

    bool EventLoopThread::CreaterSubThread(bool wait)
    {
        if (nullptr != event_base)
        {
            if (event_base->EventThread())
            {
                if (this->CreaterThread())
                {
                    if (wait)
                    {
                        return Join();
                    }
                    return true;
                }
            }
            return event_base->RunInLoop(std::bind((bool(EventLoopThread::*)(bool))&EventLoopThread::CreaterThread, this, wait));
        }
        return false;
    }

    bool EventLoopThread::DestroyThread()
    {
        if(uv_loop_alive(loop->EventBasic()))
        {
            return loop->StopDispatch();
        }
        return false;
    }

    EventLoop* EventLoopThread::GetEventLoop()
    {
        if (nullptr != loop)
        {
            return loop.get();
        }

        if (nullptr != event_base)
        {
            return event_base;
        }

        return nullptr;
    }

    bool EventLoopThread::CreaterThread()
    {
        return 0 == uv_thread_create(event_thread_.get(), &EventLoopThread::ThreadRun, this);
    }

    bool EventLoopThread::Join()
    {
        return 0 == uv_thread_join(event_thread_.get());
    }

    void EventLoopThread::ThreadRun()
    {
        if (ChangeStatus(None, Init))
        {
            loop.reset(new EventLoop(event_share->EventLoop(event_index), event_index));
            {
                if (ChangeStatus(Init, Exec))
                {
                    if (loop->InitialEvent())
                    {
                        if (false == loop->ExecDispatch())
                        {
                            assert(0);
                            return;
                        }

                        if (ChangeStatus(Exec, Stop))
                        {
                            assert(loop->ExistsStoped());
                        }
                    }
                }
            }
        }
    }

    void EventLoopThread::ThreadRun(void* handler)
    {
        if (nullptr != handler)
        {
            EventLoopThread* watcher = static_cast<EventLoopThread*>(handler);
            {
                if (nullptr != watcher)
                {
                    watcher->ThreadRun();
                }
            }
        }
    }
}