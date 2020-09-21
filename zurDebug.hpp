#include <cstdio>
#include <iostream>
#include <iomanip>
#define DEBUGGING_ENABLED true
#define DEBUG(fmt, ...) if(DEBUGGING_ENABLED) printf(fmt, ##__VA_ARGS__); printf("\n")
#define DEBUG_PRINT_REGISTER(x) if(DEBUGGING_ENABLED) std::cout << std::hex << static_cast<int>(x) << std::endl;