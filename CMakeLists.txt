cmake_minimum_required(VERSION 3.10)
project(BoostTutorial)

set(CMAKE_CXX_STANDARD 17)

# ***** NOTE *****
# This is absolutely REQUIRED for logging to work, as it has a compiled component.
# Alternatively, you could do:
#
# #define BOOST_LOG_DYN_LINK 1
#
# in the files where you plan on using logging.
# Also note the special find_package and link_directories for Boost below.
add_definitions(-DBOOST_LOG_DYN_LINK)

find_package(Threads)
find_package(Boost 1.67 COMPONENTS log system serialization REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

add_subdirectory(graph)
add_subdirectory(serialization)
add_subdirectory(signals2)