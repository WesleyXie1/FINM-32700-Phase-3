#pragma once
#include <string>
#include <vector>
#include <fstream>  // std::ifstream file(filename)
#include <sstream>  // std::istringstream iss(line);
#include <iostream> 

/*
sample_feed.txt
   ↓
load_feed()
   ↓
vector<FeedEvent>
   ↓
main.cpp 
*/

// 定义  一条 feed event 属于哪种类型
enum class FeedType {
    BID,
    ASK,
    EXECUTION,
    UNKNOWN
};

// 一条解析后的事件对象长什么样 文件里的一行，读进程序后，就变成一个 FeedEvent
struct FeedEvent {
    FeedType type = FeedType::UNKNOWN;
    double price = 0.0;
    int quantity = 0;
    int order_id = -1;

// 把一条事件打印成可读格式
    void print() const {
        switch (type) {
            case FeedType::BID:
                std::cout << "[BID] " << price << " x " << quantity << "\n";
                break;
            case FeedType::ASK:
                std::cout << "[ASK] " << price << " x " << quantity << "\n";
                break;
            case FeedType::EXECUTION:
                std::cout << "[EXECUTION] Order " << order_id << " filled: " << quantity << "\n";
                break;
            default:
                std::cout << "[UNKNOWN]\n";
        }
    }
};


// 把文件里所有事件都读出来  装进一个事件列表里返回
std::vector<FeedEvent> load_feed(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<FeedEvent> events;

    if (!file.is_open()) {
        std::cerr << "Error: could not open file " << filename << "\n";
        return events;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "BID") {
            double price;
            int qty;
            if (iss >> price >> qty) {
                events.push_back({FeedType::BID, price, qty, -1});
            }
        } else if (type == "ASK") {
            double price;
            int qty;
            if (iss >> price >> qty) {
                events.push_back({FeedType::ASK, price, qty, -1});
            }
        } else if (type == "EXECUTION") {
            int order_id;
            int filled;
            if (iss >> order_id >> filled) {
                events.push_back({FeedType::EXECUTION, 0.0, filled, order_id});
            }
        } else {
            std::cerr << "Unknown event type: " << line << "\n";
        }
    }

    return events;
}