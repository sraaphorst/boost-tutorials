/**
 * example64_4.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Serializing user-defined types with a member function
 */

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <iostream>
#include <sstream>

using namespace boost::archive;

std::stringstream ss;

class animal {
public:
    // Note that you need a default constructor!
    animal() = default;

public:
    animal(int legs) : _legs{legs} {}
    int legs() const { return _legs; }

private:

    // The serialize method is the key to being able to serialize AND deserialize!
    // NOTE: serialize should NEVER be explicitly called!
    // Declare it as private and then let boost::serialization::access be a friend.
    friend class boost::serialization::access;

    template<typename Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & _legs;
    }

    int _legs;
};

void save() {
    text_oarchive oa{ss};
    animal a{4};
    oa << a;
}

void load() {
    text_iarchive ia{ss};

    // NOTE we need a default constructor for the type we are reading!!!
    // We can make it protected as above.
    animal a;

    ia >> a;
    std::cout << a.legs() << std::endl;
}

int main() {
    save();
    load();
}