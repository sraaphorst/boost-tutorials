/**
 * Example64_1.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Using boost::archive::text_oarchive
 */

#include <boost/archive/text_oarchive.hpp>
#include <iostream>

using namespace boost::archive;

int main() {
    text_oarchive oa{std::cout};
    int i = 1;
    oa << 1;
}