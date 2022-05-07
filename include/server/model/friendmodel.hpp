#ifndef FRIENDMODEL_H
#define FRIENDMIDEL_H

#include <vector>
#include "user.hpp"

using namespace std;

// 提供好友信息的操作接口方法
class FriendModel{
public:
    // 添加好友关系
    void insert(int userid, int friendid);

    // 返回用户好友列表（在实际的项目开发中，好友列表一般存储在客户端，放在服务器端每次返回，服务器压力太大）
    // 返回user的详细信息，所以需要user表和friend表的联合查询结果
    vector<User> query(int userid);
};

#endif