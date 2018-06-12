/**
 * graph.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * NOTE: This fails under g++7! Why?
 */
#include <iostream>
#include <iomanip>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/graph/adjacency_list.hpp>

#include "graph_common.h"

namespace logging = boost::log;

int main() {
    BOOST_LOG_TRIVIAL(debug) << "Debugging on.";

    // Create a graph.
    auto g = createCn_ve(4);

    // Access vertices with vertices(g).
    // Returns a pair<adjacency_list<>::vertex_iterator, same>.
    auto print_vertices = [](const graph_t &g) {
        std::cout << "Vertices: ";
        auto[vsBegin, vsEnd] = vertices(g);
        std::copy(vsBegin, vsEnd, vout);
        std::cout << std::endl;
    };

    // Lambda to calculate graph size.
    auto graph_size = [](const graph_t &g) {
        auto[vsBegin, vsEnd] = vertices(g);
        return std::distance(vsBegin, vsEnd);
    };

    std::cout << "The graph has " << graph_size(g) << " vertices." << std::endl;
    print_vertices(g);


    // Iterate over the edges with edges(g), which returns a pair<adjacency_list<>::edge_iterator>.
    auto print_edges = [](const graph_t &g) {
        auto[esBegin, esEnd] = edges(g);
        std::cout << "Edges: ";
        std::copy(esBegin, esEnd, eout);
        std::cout << std::endl;
    };
    print_edges(g);

    BOOST_LOG_TRIVIAL(info) << "Modifying the graph...";

    // Note that if vertices don't exist, they are automatically added. Let's add a 5th to make a house shape.
    // These are heavy, just for fun.
    //BOOST_LOG_TRIVIAL(info) << "Adding two edges to a vertex not explicitly added to G." << endl;
    std::cout.setf(std::ios::boolalpha);
    auto[edge0, success0] = boost::add_edge(3, 4, g);
    std::cout << "Adding edge " << edge0 << ": " << success0 << std::endl;
    auto[edge1, success1] = boost::add_edge(2, 4, g);
    std::cout << "Adding edge " << edge1 << ": " << success1 << std::endl;
    print_vertices(g);
    print_edges(g);

    return 0;
}