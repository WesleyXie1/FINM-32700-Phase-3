#include "market_snapshot.h"

void MarketSnapshot::update_bid(double price, int qty) {
    if (qty == 0) {
        bids.erase(price);
    } else {
        bids[price] = std::make_unique<PriceLevel>(price, qty);
    }
}

void MarketSnapshot::update_ask(double price, int qty) {
    if (qty == 0) {
        asks.erase(price);
    } else {
        asks[price] = std::make_unique<PriceLevel>(price, qty);
    }
}

const PriceLevel* MarketSnapshot::get_best_bid() const {
    if (bids.empty()) return nullptr;
    return bids.begin()->second.get();
}

const PriceLevel* MarketSnapshot::get_best_ask() const {
    if (asks.empty()) return nullptr;
    return asks.begin()->second.get();
}

void MarketSnapshot::print_best_bid() const {
    const PriceLevel* best = get_best_bid();
    if (best) {
        std::cout << "[Market] Best Bid: " << best->price << " x " << best->quantity << "\n";
    } else {
        std::cout << "[Market] No Bid\n";
    }
}

void MarketSnapshot::print_best_ask() const {
    const PriceLevel* best = get_best_ask();
    if (best) {
        std::cout << "[Market] Best Ask: " << best->price << " x " << best->quantity << "\n";
    } else {
        std::cout << "[Market] No Ask\n";
    }
}