#include "order_manager.h"

// 创建一张新订单，存到 orders 里，并把订单 ID 返回出去
int OrderManager::place_order(Side side, double price, int qty) { // 你下单以后，系统要把这张单的编号告诉你
    int id = next_id++;  // 当前 next_id 赋给这张新订单  然后 next_id 自己加 1
    orders[id] = std::make_unique<MyOrder>(id, side, price, qty);    // 创建一张新的 MyOrder   然后放进 orders 这个 map 里

    std::cout << "[Order] Placed "
              << (side == Side::Buy ? "BUY" : "SELL") // 如果 side == Side::Buy，打印 "BUY" 否则打印 "SELL"
              << " order at " << price
              << " x " << qty
              << " (ID = " << id << ")\n";

    return id;
}

// 取消一张已有订单
void OrderManager::cancel(int id) {
    auto it = orders.find(id);  // 在 orders map 里找 key 等于 id 的订单
    if (it != orders.end()) {   // 如果真的找到了这张订单  才继续做取消
        it->second->status = OrderStatus::Cancelled;
        std::cout << "[Order] Order " << id << " cancelled\n";
        orders.erase(it);  // 把这张订单从 orders map 里移除
    }
}

// 收到成交回报后  更新订单已成交量和状态
void OrderManager::handle_fill(int id, int filled_qty) {
    auto it = orders.find(id);
    if (it == orders.end()) {
        std::cout << "[Execution] Order " << id << " not found\n"; // 如果这个 order id 根本不在 active orders 里，就别继续处理了
        return;
    }

// order 现在指向那张真实订单   .get() 拿出里面的原始指针
    MyOrder* order = it->second.get();
    order->filled += filled_qty;   // 把这次新成交的量，加到订单原来的已成交量上

    std::cout << "[Execution] Order " << id << " filled: " << filled_qty << "\n";


    if (order->filled >= order->quantity) {   // 累计成交量是不是已经达到或超过订单总量
        // 如果全成交 改状态并删除   完全成交后订单应从 active list 移除
        order->status = OrderStatus::Filled;
        std::cout << "[Order] Order " << id << " completed (" 
                  << order->filled << " / " << order->quantity << ") and removed\n";
        orders.erase(it);

        // 如果 filled < quantity，说明订单还没完全成交完
    } else {
        order->status = OrderStatus::PartiallyFilled;
        std::cout << "[Order] Order " << id << " partially filled: "
                  << order->filled << " / " << order->quantity << "\n";
    }
}

// 把当前还在 orders map 里的所有活跃订单打印出来
void OrderManager::print_active_orders() const {
    std::cout << "\n[Active Orders]\n";
    for (const auto& pair : orders) {                 // 遍历所有订单
        const MyOrder* order = pair.second.get();     // 从 unique_ptr 里拿出原始指针，方便访问字段
        std::cout << "ID=" << order->id
                  << " "
                  << (order->side == Side::Buy ? "BUY" : "SELL")
                  << " "
                  << order->price
                  << " x " << order->quantity
                  << " filled=" << order->filled
                  << "\n";
    }
}

/*
place_order(...)

cancel(...)

handle_fill(...)

print_active_orders()

下单时，生成一个新 id，把订单放进 map 里
撤单时，把订单状态改成取消，然后从 map 删除
收到成交时，更新已成交量
没成交完 → PartiallyFilled
成交完 → Filled，然后从 map 删除
最后可以把还没结束的订单打印出来

*/