#pragma once             // 这个文件只准被编译器正式读一次    没有这个 编译器可能会重复看到同一个类定义
#include <map>
#include <memory>
#include <iostream>


// order book 里的一个价格档位
struct PriceLevel {
    double price;
    int quantity;
// PriceLevel 的构造函数 p q分别先初始化
    PriceLevel(double p, int q) : price(p), quantity(q) {}
};


// 下面是本地保存的市场快照
/* 
当前有哪些 bid price levels 
当前有哪些 ask price levels
当前 best bid 是什么
当前 best ask 是什么
*/

class MarketSnapshot {
private:
    // 用一个按价格从高到低排序的 map 来存所有 bid price levels；每个 price level 用 unique_ptr 管理。     谁出价高，谁站最前面  
    std::map<double, std::unique_ptr<PriceLevel>, std::greater<double>> bids;
    // 用一个按价格升序排列的 map 来保存所有 ask price levels，key 是价格，value 是由 unique_ptr 管理的 PriceLevel 对象     谁报价低，谁站最前面
    std::map<double, std::unique_ptr<PriceLevel>> asks;

public:
    // 当有一个新的 bid 更新 来的时候，用这个函数更新本地买盘
    void update_bid(double price, int qty);
    // 当有一个新的 ask 更新 来的时候，用这个函数更新本地买盘
    void update_ask(double price, int qty);

    // 返回当前 best bid 的只读指针；如果没有 bid，就返回空指针
    const PriceLevel* get_best_bid() const;
    // 
    const PriceLevel* get_best_ask() const;

    // 把当前 best bid 打印出来 只是读取内部数据，不应该修改对象状态
    void print_best_bid() const;
    void print_best_ask() const;
};






