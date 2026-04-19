# FINM 32700 Phase 3

## Overview
This project builds a local order book and core trading infrastructure in C++.

It includes:
- a `MarketSnapshot` class to maintain bid/ask levels
- an `OrderManager` class to track placed orders and fills
- a feed parser to read simulated market events from `sample_feed.txt`
- a main strategy loop that reacts to market updates and execution events

## Files
- `market_snapshot.h`
- `market_snapshot.cpp`
- `order_manager.h`
- `order_manager.cpp`
- `feed_parser.h`
- `main.cpp`
- `sample_feed.txt`
- `output.log`

## Architecture
`MarketSnapshot` stores bids and asks in maps and provides access to the best bid and best ask.

`OrderManager` stores active orders in a map using unique order IDs. It supports order placement, cancellation, fill handling, and active order printing.

`main.cpp` loads the feed, processes each event, updates the market snapshot, updates order states, and places simple strategy orders based on top-of-book conditions.

## Memory Safety
This project uses `std::unique_ptr` for:
- `PriceLevel`
- `MyOrder`

There is no manual `new` or `delete`.  
When a price level or order is removed from its map, memory is automatically released.

