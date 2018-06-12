/**
 * graph3.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <array>
#include <utility>
#include <iostream>

#include <boost/graph/adjacency_list.hpp>

#include "graph_common.h"

int main() {
    // Create a graph by specifying a list of pairs representing edges.
    graph_t g = createCn_ep(4);

    std::cout << "Number of vertices: " << boost::num_vertices(g) << std::endl;
    std::cout << "Number of edges:    " << boost::num_edges(g) << std::endl;

    return 0;
}
