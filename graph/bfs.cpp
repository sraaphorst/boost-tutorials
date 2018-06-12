/**
 * bfs.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Visit the points from inside to outside using BFS.
 */

#include <algorithm>
#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/named_function_params.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/array.hpp>

#include "graph_common.h"

int main() {
    int constexpr v = 8;

    auto g = createCn_ep(v);

    // Create an array to hold the distances calculated from the BFS.
    // BFS doesn't set the values of the array as it goes: it just increases them: hence, they must be initialized.
    // We can use boost arrays, STL arrays, or C-style arrays.
    int *carray = new int[v];
    std::fill(carray, carray+v, 0);
    std::array<int, v> array{{0}};
    boost::array<int, v> distances{{0}};

    // Note: breadth_first_visit is similar, but requires user to colour vertices white beforehand.
    // This could be useful, for example, if we knew only a small part of the graph would be reached.
    boost::breadth_first_search(g, topLeft,
            // Behaviour of algorithm depends on visitors, i.e. objects whose member functions
            // are called when a vertex is visited.
                                boost::visitor(
                                        boost::make_bfs_visitor(
                                                // Boost provides this helper function to create the visitor.
                                                // A property map and a tag must also be passed.
                                                // Property maps store properties for vertices or edges.
                                                // In this case, distances is the property map.
                                                boost::record_distances(
                                                        carray,
                                                        //array.begin(),
                                                        //distances.begin()
                                                        // There are different events, and the second param to
                                                        // record distances specifies the events the visitor
                                                        // should be notified of.
                                                        // Boost.Graph defines tags that are empty classes to give
                                                        // event names. The tag boost::on_tree_edge specifies that
                                                        // a distance should be recorded when a new vertex is found.
                                                        // Events depend on the algorithm. Check the documentation.
                                                                        boost::on_tree_edge{}))));


    // Note: boost::record_distances is algorithm-independent, so it needs an adapter to bind an algorithm and a
    // visitor. This is done by boost::make_bfs_visitor, which creates the adapter, and that returns the visitor as
    // expected by boost::breadth_first_search.

    // The visitor defines member functions that fit the events that the algorithm supports.
    // In this case, the visitor returned by make_bfs_visitor defines the method tree_edge.
    // If a visitor that is defined with the tag on_tree_edge is passed to make_bfs_visitor,
    // then the visitor is notified when tree_edge() is called.

    // Thus, we can use visitors with different algorithms without those visitors having to define all the
    // member functions expected by all the algorithms.

    // Events can be found here:
    // https://www.boost.org/doc/libs/1_55_0/libs/graph/doc/breadth_first_visit.html
    // and include:
    // examine_vertex, invoked on each vertex removed from queue
    // examine_edge, invoked after vertex is removed from queue
    // tree_edge: invoked (as well as examine_edge) if edge is a tree edge
    // discover_vertex: invoked first time vertex is discovered
    // non_tree_edge
    // gray_target: means vertex is in queue
    // black_target: vertex no longer in queue
    // finish_vertex: after all out edges of u have been examined and all vertices adjacent to u discovered

    // One variant of BFS expects every parameter will be passed.
    // The one we used only expects named parameters you're interested in to be passed.

    auto oiter = std::ostream_iterator<int>(std::cout, " ");
    std::copy(carray, carray+v, oiter);
    std::cout << std::endl;
    std::copy(array.begin(), array.end(), oiter);
    std::cout << std::endl;
    std::copy(distances.begin(), distances.end(), oiter);
    std::cout << std::endl;
    delete[] carray;
    return 0;
}

