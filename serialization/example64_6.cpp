/**
 * example64_6.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Serializing strings.
 */

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

/** NOTE THIS INCLUDE **/
/** This allows us to serialize strings **/
#include <boost/serialization/string.hpp>

#include <iostream>
#include <sstream>
#include <string>
#include <utility>

using namespace boost::archive;

std::stringstream ss;

class animal {
public:
    animal(int legs, std::string name)
        : legs_{legs}, name_{std::move(name)} {

    }

    int legs() { return legs_; }
    const std::string &name() const { return name_; }

private:
    animal() = default;

    friend class boost::serialization::access;

    template<typename Archive>
    friend void serialize(Archive &ar, animal &a, const unsigned int version);

    int legs_;
    std::string name_;

public:
    static void save() {
        text_oarchive oa{ss};
        animal a{4, "cat"};
        oa << a;
    }

    static void load() {
        text_iarchive ia{ss};

        animal a;
        ia >> a;

        std::cout << a.name() << " has " << a.legs() << " legs" << std::endl;
    }
};

/** NOTE: since we need to use the actual members, this had to be declared a friend! **/
template<typename Archive>
void serialize(Archive &ar, animal &a, const unsigned int version) {
    // We wouldn't be able to serialize a.name_ without the string include.
    ar & a.legs_ & a.name_;
}

int main() {
    animal::save();
    animal::load();
}