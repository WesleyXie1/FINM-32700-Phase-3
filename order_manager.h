#pragma once
#include <map>
#include <memory>
#include <iostream>

// 订单方向
enum class Side {
    Buy,
    Sell
};

// 订单状态 也就是一张订单现在处于什么阶段
enum class OrderStatus {
    New,
    Filled,
    PartiallyFilled,
    Cancelled
};

// 自己的一张订单长什么样
struct MyOrder {
    int id;
    Side side;
    double price;
    int quantity; // 想买/卖多少
    int filled;  // 已经成交了多少
    OrderStatus status; 
// 创建一张新订单时，初始内容应该是什么
    MyOrder(int i, Side s, double p, int q)
        : id(i), side(s), price(p), quantity(q), filled(0), status(OrderStatus::New) {}
};


// 订单管理器本体 & 一个专门帮你保管和管理所有活跃订单
class OrderManager {
private:
    std::map<int, std::unique_ptr<MyOrder>> orders;   // 用一个 map 存所有订单
    int next_id = 1;  // 下一个新订单要用的编号 从 1 开始

public:
    int place_order(Side side, double price, int qty); // 创建一张新订单，并返回它的订单 ID
    void cancel(int id);  // 输入订单 id 取消某个订单 
    void handle_fill(int id, int filled_qty);   // 处理某张订单收到的成交回报    输入1号订单成交了10 更新：filled/status   满了还要把订单移除
    void print_active_orders() const;   // 把当前还活跃的订单打印出来 打印：id side price quantity filled
};