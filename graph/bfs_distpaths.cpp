/**
 * bfs_distpaths.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <array>
#include <iostream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/named_function_params.hpp>
#include <boost/graph/visitors.hpp>

#include "graph_common.h"

int main() {
    int constexpr v = 9;
    auto g = createCn_ep(v);

    std::array<int, v> distances;
    std::array<int, v> predecessors;
    predecessors[0] = 0;

    boost::breadth_first_search(g, 0,
        boost::visitor(
                boost::make_bfs_visitor(
                        // We need more visitors, so we nest them in a pair. Even more must be further nested.
                        std::make_pair(
                                boost::record_distances(distances.begin(), boost::on_tree_edge{}),
                                boost::record_predecessors(predecessors.begin(), boost::on_tree_edge{})))));

    for (int i=0; i < v; ++i)
        std::cout << "Vertex " << i << " has predecessor " << predecessors[i] << " and distance " << distances[i] << std::endl;

    return 0;
}