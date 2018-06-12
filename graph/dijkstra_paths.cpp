/**
 * dijskstra_paths.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <array>
#include <iostream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/named_function_params.hpp>

#include "graph_common.h"

int main() {
    int constexpr v = 8;
    std::array<int, v> directions;
    std::array<int, v> weights{{1, 3, 2, 4, 3, 1, 2, 3}};

    weighted_graph_t g = createCn_weighted(weights);

    // Note that we don't need to use visitors here, as Dijkstra's doesn't visit points.
    // It looks for shortest paths, and thus it's not necessary to think of events or visitors:
    // You just need a container to store predecessors.
    // Here the algorithm expects the named parameter edge_weight_t, and we pass the contained
    // directions with predecessor_map. This is a helper function which expects a pointer or an iterator
    // to the beginning of an array.
    boost::dijkstra_shortest_paths(g, 0, boost::predecessor_map(directions.begin()));

    for (auto i = 0; i < v; ++i)
        std::cout << i << ": " << directions[i] << std::endl;

    return 0;
}