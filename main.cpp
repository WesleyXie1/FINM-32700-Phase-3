#include <iostream>
#include "market_snapshot.h"
#include "order_manager.h"
#include "feed_parser.h"

int main() {
    MarketSnapshot snapshot;      // 创建一个本地市场快照对象. 名字叫 snapshot
    OrderManager om;              // 创建一个订单管理器对象. 名字叫 om

    auto feed = load_feed("sample_feed.txt");

    bool placed_order_1 = false;
    bool placed_order_2 = false;
    // 如果没有这两个布尔变量，你的策略条件一旦满足，可能会在循环里重复下很多次同样的单


    // 把 feed 里的每一条事件拿出来. 按顺序处理
    for (const auto& event : feed) {
        event.print();   // 先把当前这条 feed event 打印出来

        // 如果这条事件是 BID 更新，就更新 snapshot 里的 bids，并打印当前 best bid
        if (event.type == FeedType::BID) {
            snapshot.update_bid(event.price, event.quantity);
            snapshot.print_best_bid();
        } 
        // 收到 ask 更新 → 改本地 ask book → 打印当前 best ask
        else if (event.type == FeedType::ASK) {
            snapshot.update_ask(event.price, event.quantity);
            snapshot.print_best_ask();
        } 
        // 如果事件是 EXECUTION.  就更新订单状态
        else if (event.type == FeedType::EXECUTION) {
            om.handle_fill(event.order_id, event.quantity);
        }
        
        // 每处理完一条事件后，都重新取一下当前的 best bid 和 best ask
        const PriceLevel* best_bid = snapshot.get_best_bid();
        const PriceLevel* best_ask = snapshot.get_best_ask();

        // 看到便宜 ask 就下一个 SELL
        if (best_ask && !placed_order_1 && best_ask->price <= 100.20) {
            om.place_order(Side::Sell, best_ask->price, 30);
            placed_order_1 = true;
        }
        // 看到高 bid 就下一个 BUY
        if (best_bid && !placed_order_2 && best_bid->price >= 100.17) {
            om.place_order(Side::Buy, best_bid->price, 100);
            placed_order_2 = true;
        }
    }

    om.print_active_orders();

    return 0;
}