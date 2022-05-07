#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>

using namespace std;
using namespace muduo;
using namespace muduo::net;
using namespace placeholders;

// 聊天服务器的主类
class ChatServer{
public:
    // 初始化聊天服务器对象
    ChatServer(EventLoop* loop,             //事件循环
            const InetAddress& listenAddr,  //IP+port
            const string& nameArg);         //服务器的名字
            

    // 启动服务，开启事件循环
    void start();

private:
// 上报连接相关信息的回调函数
    void onConnection(const TcpConnectionPtr& conn);
        
// 上报读写事件相关信息的回调函数
    void onMessage(const TcpConnectionPtr& conn, // 连接
                    Buffer* buffer,     // 缓冲区
                    Timestamp time);    // 时间信息
    
    TcpServer _server; // 组合的muduo库，实现服务器功能的类对象
    EventLoop* _loop; // 指向事件循环对象的指针
};

#endif