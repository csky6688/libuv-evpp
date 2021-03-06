#include <tcp_connect.h>
#include <tcp_client.h>
#include <event_socket.h>
#include <event_loop.h>
namespace Evpp
{
    TcpConnect::TcpConnect(EventLoop* loop, const std::shared_ptr<socket_tcp>& client) :
        event_base(loop),
        tcp_client(client),
        tcp_connect(std::make_unique<socket_connect>())
    {

    }

    TcpConnect::~TcpConnect()
    {

    }

    bool TcpConnect::ConnectServers(const std::unique_ptr<EventSocket>& socket, void* client)
    {
        if (nullptr != socket && nullptr != client)
        {
            if (InitialConnect(client))
            {
                return ConnectService(socket);
            }
            return ConnectService(socket);
        }
        return false;
    }

    bool TcpConnect::InitTcpService()
    {
        return 0 == uv_tcp_init(event_base->EventBasic(), tcp_client.get());
    }

    bool TcpConnect::CreaterConnect(const sockaddr* addr)
    {
        return 0 == uv_tcp_connect(tcp_connect.get(), tcp_client.get(), addr, &TcpClient::DefaultConnect);
    }

    bool TcpConnect::ConnectService(const std::unique_ptr<EventSocket>& socket)
    {
        if (nullptr != socket)
        {
            if (InitTcpService())
            {
                if (uv_tcp_nodelay(tcp_client.get(), 1))
                {
                    printf("��ʼ��ʧ��\n");
                }

                return CreaterConnect(&socket->GetSocketInfo()->addr);
            }
        }
        return false;
    }

    bool TcpConnect::InitialConnect(void* client)
    {
        if (nullptr == tcp_connect->data)
        {
            tcp_connect->data = client;
            return true;
        }
        return false;
    }
}