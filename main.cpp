#ifndef DEBUG
#pragma GCC optimize(2)
#pragma GCC optimize(3, "Ofast", "inline")
#endif

#include "global.h"
#include "test/test.h"

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
#ifdef DEBUG
    complexTest();
    constantTest();
    bigIntegerTest();
#endif
    return 0;
}
