/**
 * example67_2.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <functional>
#include <iostream>

int main() {
    // This is largely just to show that signals2 is much more advanced than pure lambdas.
    std::function<void()> f;
    f = []{std::cout << "Hello, World!" << std::endl;};
    f();
}