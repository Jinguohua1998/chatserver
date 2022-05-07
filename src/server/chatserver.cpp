#include "chatserver.hpp"
#include <functional>
#include <string>
#include "json.hpp"
#include "chatservice.hpp"

using namespace std;
using namespace placeholders;
using json = nlohmann::json;


ChatServer::ChatServer(EventLoop* loop,             //事件循环
            const InetAddress& listenAddr,  //IP+port
            const string& nameArg)          //服务器的名字
            :_server(loop, listenAddr, nameArg)
            ,_loop(loop){
        // 注册连接回调
        _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));

        //注册消息回调
        _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2, _3));
    
        //设置线程数量
        _server.setThreadNum(4); // 1个IO线程，3个worker线程
}

// 启动服务
void ChatServer::start(){
        _server.start();
}

// 上报连接相关信息的回调函数
void ChatServer::onConnection(const TcpConnectionPtr& conn){
    // 客户端断开连接
    if(!conn->connected()){
        ChatService::instance()->clientCloseException(conn);
        conn->shutdown();
    }
}

// 上报读写相关信息的回调函数
void ChatServer::onMessage(const TcpConnectionPtr& conn, // 连接
                    Buffer* buffer, // 缓冲区
                    Timestamp time) // 时间信息
{
    string buf = buffer->retrieveAllAsString();
    // 数据的反序列化
    json js = json::parse(buf);
    // 将网络模块和业务模块解耦：通过不同的js["msgid"]获取不同的业务处理器（可以传入conn, js, time）
    // 其中js["msgid"]返回的只是json对象，还需要用get函数转换成int类型
    auto msgHandler = ChatService::instance()->getHandler(js["msgid"].get<int>());
    // 回调消息绑定好的事件处理器，来执行相应的业务处理
    msgHandler(conn, js, time);
}