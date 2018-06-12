/**
 * bfs_findpaths.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Finding the shortest paths.
 */

#include <array>
#include <iostream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/named_function_params.hpp>
#include <boost/graph/visitors.hpp>

#include "graph_common.h"

int main() {
    unsigned long constexpr v = 8;
    auto g = createCn_ve(v);

    std::array<int, v> predecessors;
    predecessors[0] = 0;

    boost::breadth_first_search(g, 0,
        boost::visitor(
                boost::make_bfs_visitor(
                        boost::record_predecessors(predecessors.begin(), boost::on_tree_edge{}))));

    for (auto i=0; i < predecessors.size(); ++i)
        std::cout << i << " has predecessor " << predecessors[i] << std::endl;

    return 0;
}
