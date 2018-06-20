/**
 * example64_5.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Serializing with a free-standing function.
 * There may be situations where you can't modify an existing class to add serialize().
 * Hence, we must pass, as the second parameter, a reference to the object we wish to
 * serialize.
 */

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <iostream>
#include <sstream>

using namespace boost::archive;

std::stringstream ss;

struct animal {
    int legs_;

    animal() = default;
    animal(int legs) : legs_{legs} {}
    int legs() const { return legs_; }
};


// Here is our serialize method independent of the class.
template <typename Archive>
void serialize(Archive &ar, animal &a, const unsigned int version) {
    // NOTE: ar & a.legs() fails here!
    // We MUST be able to access essential member variables of a class from the outside.m
    ar & a.legs_;
}

void save() {
    text_oarchive oa{ss};
    animal a{4};
    oa << a;
}

void load() {
    text_iarchive ia{ss};
    animal a{};
    ia >> a;
    std::cout << a.legs() << std::endl;
}

int main() {
    save();
    load();
}