/**
 * example64_3.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Serializizing with a stringstream
 */

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <iostream>
#include <sstream>

using namespace boost::archive;

std::stringstream ss;

void save() {
    text_oarchive oa{ss};
    int i = 1;
    oa << i;
}

void load() {
    text_iarchive ia{ss};
    int i = 0;
    ia >> i;
    std::cout << i << std::endl;
}

int main() {
    save();
    load();
}