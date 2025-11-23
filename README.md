# Order Book Learning Project

Minimal starting point to parse order inputs from stdin. Build with clang++ (no external deps):

```bash
clang++ -std=c++23 -Wall -Wextra -pedantic -Isrc src/main.cpp src/parser/order_parser.cpp -o order_book
# or: make
```

Run and type orders like:
```
LIMIT AAPL BUY 195.25 100
MARKET AAPL SELL 50
```
Lines starting with `#` are ignored.
