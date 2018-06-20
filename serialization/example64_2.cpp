/**
 * example64_2.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Using boost::archive::text_iarchive
 */

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <iostream>
#include <fstream>

using namespace boost::archive;

void save() {
    std::ofstream file{"archive.txt"};
    text_oarchive oa{file};
    int i = 1;
    oa << 1;
}

void load() {
    std::ifstream file{"archive.txt"};
    text_iarchive ia{file};
    int i = 0;
    ia >> i;
    std::cout << "i=" << i << std::endl;
}


int main() {
    save();
    load();
}