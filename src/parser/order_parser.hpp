#pragma once

#include <optional>
#include <string>

#include "parser/order_types.hpp"

struct ParseResult {
    std::optional<OrderInput> order;
    std::string error;
};

ParseResult parse_line(const std::string& line);
