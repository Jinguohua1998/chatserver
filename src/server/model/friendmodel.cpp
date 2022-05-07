#include "friendmodel.hpp"
#include "db.h"

// 添加好友关系
void FriendModel::insert(int userid, int friendid){
    // 1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "insert into friend VALUES(%d, %d)", userid, friendid);

    MySQL mysql;
    if(mysql.connect()){
        mysql.update(sql);
    }
}

// 返回用户好友列表（在实际的项目开发中，好友列表一般存储在客户端，放在服务器端每次返回，服务器压力太大）
// 返回user的详细信息，所以需要user表和friend表的联合查询结果
vector<User> FriendModel::query(int userid){
    // 1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "select a.id, a.name, a.state from user a inner join friend b on a.id = b.friendid where b.userid = %d", userid);
    
    vector<User> vec;
    MySQL mysql;
    if(mysql.connect()){
        MYSQL_RES* res = mysql.query(sql);
        if(res != nullptr){
            MYSQL_ROW row;
            while((row = mysql_fetch_row(res)) != nullptr){
                User user;
                user.setId(stoi(row[0]));
                user.setName(row[1]);
                user.setState(row[2]);
                vec.push_back(user);
            }
            mysql_free_result(res); // 释放指针所指向的内存资源，防止内存泄漏，类似于delete
            return vec; // 这里的return可以不要
        }
    }
    return vec;
}