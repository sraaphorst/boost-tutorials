/**
 * graph2.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <algorithm>
#include <iostream>
#include <iterator>
#include <tuple>
#include <vector>

#include <boost/log/trivial.hpp>
#include <boost/graph/adjacency_list.hpp>

#include "graph_common.h"

int main() {
    auto g = createCn_e(4);

    // Print the graph.
    std::cout << "Vertices: ";
    auto[vit, vend] = boost::vertices(g);
    std::copy(vit, vend, vout);
    std::cout << std::endl;

    std::cout << "Edges: ";
    auto[eit, eend] = boost::edges(g);
    std::copy(eit, eend, eout);
    std::cout << std::endl;

    // Now show all the vertices adjacent to topLeft.
    std::cout << "Vertices adjacent to topLeft: ";
    auto[vit2, vend2] = boost::adjacent_vertices(topLeft, g);
    std::copy(vit2, vend2, vout);
    std::cout << std::endl;

    // Show the out edges of topLeft.
    // NOTE: this uses an out_edge_iterator, and not an edge_iterator!
    // They still reference vertex descriptors.
    std::cout << "Out edges of topLeft:";
    auto [oit, oend] = boost::out_edges(topLeft, g);
    for (; oit != oend; ++oit)
        std::cout << " " << boost::target(*oit, g) << " (source: " << boost::source(*oit, g) << ")";
    std::cout << std::endl;

    return 0;
}