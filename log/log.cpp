/**
 * log.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Playing around with boost::log.
 * Remember that this requires a compiled module! See the top-level CMakeLists.txt.
 */

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

// For convenience:
namespace logging = boost::log;

int main() {
    // Trivial logging to console:
    BOOST_LOG_TRIVIAL(debug) << "Starting up...";

    // Change the logging level.
    logging::core::get()->set_filter(
            logging::trivial::severity >= logging::trivial::info
    );

    // This should not print.
    BOOST_LOG_TRIVIAL(debug) << "It would be a bug to see me.";
}