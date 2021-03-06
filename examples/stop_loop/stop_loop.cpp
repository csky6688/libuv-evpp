#include "stop_loop.h"
#include <event_loop.h>
#include <event_signal.h>
namespace Evpp
{
    StopLoop::StopLoop() : exit_tag(0)
    {
        event_base.reset(new EventLoop());
        event_signal_.reset(new EventSignal(event_base.get(), std::bind(&StopLoop::EchoSignalCallback, this)));
    }

    StopLoop::~StopLoop()
    {
        
    }

    void StopLoop::ExitLoop(EventLoop* loop)
    {
        while (0 == exit_tag);

        if (loop->ExistsRuning())
        {
            loop->StopDispatch();
        }
    }

    void StopLoop::SetExitTag()
    {
        exit_tag.store(1);
    }

    void StopLoop::RunExamples1()
    {
        socket_tcp server;
        sockaddr_in addr;

        uv_ip4_addr("0.0.0.0", 6666, &addr);
        uv_tcp_init(event_base->EventBasic(), &server);
        uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
        uv_listen((socket_stream*)&server, 128, 0);

        std::thread exit_thread(std::bind(&StopLoop::ExitLoop, this, event_base.get()));
        std::thread set_tag_thread(std::bind(&StopLoop::SetExitTag, this));

        if (event_base->InitialEvent())
        {
            event_base->ExecDispatch();
        }
        

        exit_thread.join();
        set_tag_thread.join();

        std::cout << "EventLoop Exit" << std::endl;
    }


    void StopLoop::RunExamples2()
    {
        socket_tcp server;
        sockaddr_in addr;

        uv_ip4_addr("0.0.0.0", 6666, &addr);
        uv_tcp_init(event_base->EventBasic(), &server);
        uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
        uv_listen((socket_stream*)&server, 128, 0);

        if (event_base->InitialEvent())
        {
            event_base->ExecDispatch(std::bind(&StopLoop::EchoLoopCallback, this, std::placeholders::_1), UV_RUN_NOWAIT);
        }

        std::cout << "EventLoop Exit" << std::endl;
    }

    void StopLoop::EchoLoopCallback(EventLoop* loop)
    {
        if (++exit_tag > 9)
        {
            loop->StopDispatch();
        }

        std::cout << "Hello EventLoop: " << exit_tag << std::endl;
    }

    void StopLoop::RunExamples3()
    {
        socket_tcp server;
        sockaddr_in addr;

        uv_ip4_addr("0.0.0.0", 6666, &addr);
        uv_tcp_init(event_base->EventBasic(), &server);
        uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
        uv_listen((socket_stream*)&server, 128, 0);

        event_signal_->InitialSignal();
        event_signal_->CreaterSignal();
        // TODO: Func Set Callback
        event_signal_->SetSignalCallback(std::bind(&StopLoop::EchoSignalCallback, this));

        if (event_base->InitialEvent())
        {
            event_base->ExecDispatch(std::bind(&StopLoop::EchoLoopCallback3, this, std::placeholders::_1), UV_RUN_NOWAIT);
        }

        std::cout << "EventLoop Exit" << std::endl;
        getchar();
    }

    void StopLoop::EchoLoopCallback3(EventLoop* loop)
    {
        if (exit_tag)
        {
            // TODO: DestroySignal;
            loop->StopDispatch();
        }

        std::cout << "Hello EventLoop: " << exit_tag << std::endl;
    }

    void StopLoop::EchoSignalCallback()
    {
        exit_tag.store(1);
    }
}