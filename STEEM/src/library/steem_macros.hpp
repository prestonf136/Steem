#pragma once

#include <cassert>
#include <iostream>

// if you're wondering why this exsits I might want to change the behavior of ASSERT in the future
#define ST_ASSERT(expr) assert(expr);
#define ST_LOG(...) std::cout << __VA_ARGS__ << std::endl;
