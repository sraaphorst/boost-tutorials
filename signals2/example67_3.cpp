/**
 * example67_3.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <boost/signals2.hpp>
#include <iostream>

using namespace boost::signals2;

int main() {
    signal<void()> s;
    s.connect([]{std::cout << "Hello, ";});
    s.connect([]{std::cout << "world!" << std::endl;});
    s();
}