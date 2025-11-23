#pragma once

#include <iosfwd>

#include "engine/order_router.hpp"

// Run the REPL once over the provided input stream. When interactive is true,
// prompts are printed to std::cout.
void repl(std::istream& input, OrderRouter& router, bool interactive);
