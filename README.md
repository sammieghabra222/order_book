# Order Book C++ Project

Minimal starting point to parse order inputs from stdin or a text file. Build with clang++ (no external deps):

```bash
clang++ -std=c++23 -Wall -Wextra -pedantic -Isrc src/main.cpp src/parser/order_parser.cpp -o order_book
# or: make
```

## Run
- Interactive (stdin): `./order_book` then type orders.
- From file: `./order_book path/to/orders.txt` (see `examples/sample_orders.txt`).

Input lines:
```
LIMIT AAPL BUY 195.25 100
MARKET AAPL SELL 50
```
Lines starting with `#` are ignored.
