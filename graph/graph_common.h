/**
 * graph_common.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <array>
#include <vector>

#include <boost/log/trivial.hpp>
#include <boost/graph/adjacency_list.hpp>

// See: https://gist.github.com/daviddoria/3a95428c85057c2b404ef9193083a6c3
/**
   template <class OutEdgeListS = vecS, // a Sequence or an AssociativeContainer
        class VertexListS = vecS, // a Sequence or a RandomAccessContainer
        class DirectedS = directedS,
        class VertexProperty = no_property,
        class EdgeProperty = no_property,
        class GraphProperty = no_property,
        class EdgeListS = listS>
 */
using graph_t = boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS>;

// We store no properties for vertices, but on edges, we specify a property, i.e. weight, which is in int.
// We can store other properties on vertices and edges.
using weighted_graph_t = boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS,
        boost::no_property, boost::property<boost::edge_weight_t, int>>;

// Here is an unused definition of a directed graph with coloured vertices and edges, and weighted edges.
// We wrap the edge colouring and weighting in a pair.
// Things like edge_weight_t and vertex_color_t are tags provided by Boost as they are frequently used.
// Properties are <tag, type>.
using graph_coloured_vertices_and_edges_weighted_edges = boost::adjacency_list<boost::setS, boost::vecS, boost::directedS,
        boost::property<boost::vertex_color_t, int>, std::pair<boost::property<boost::edge_color_t, int>, boost::property<boost::edge_weight_t, int>>>;

using vertex_t = graph_t::vertex_descriptor;
using edge_t   = graph_t::edge_descriptor;

auto vout = std::ostream_iterator<vertex_t>(std::cout, " ");
auto eout = std::ostream_iterator<edge_t>(std::cout, " ");

enum vertex_name {
    topLeft, topRight, bottomLeft, bottomRight
};

/** Initialize a graph by adding vertices and then edges. **/
graph_t createCn_ve(const int n) {
    graph_t g;

    // Add the vertices.
    std::vector< vertex_t > v;
    for (auto i=0; i < n; ++i)
        v.emplace_back(boost::add_vertex(g));

    // Add the edges.
    for (auto i=0; i < n; ++i) {
        auto [_, success] = boost::add_edge(v[i], v[(i+1)%n], g);
        if (!success)
            BOOST_LOG_TRIVIAL(fatal) << "Could not create edge.";
    }

    return g;
}

/** Initialize a graph just by adding edges. Boost will create the vertices as needed. **/
graph_t createCn_e(const int n) {
    graph_t g;
    for (int i=0; i < n; ++i) {
        auto [_, success] = boost::add_edge(i, (i+1) % n, g);
        if (!success)
            BOOST_LOG_TRIVIAL(fatal) << "Could not create edge.";
    }

    return g;
}

/** Initialize a graph just by adding pairs of vertices as edges. **/
graph_t createCn_ep(const int n) {
    // Create the vector of pairs.
    std::vector<std::pair<int, int>> e;
    for (auto i=0; i < n; ++i)
        e.emplace_back(i, (i+1)%n);

    // Now create the graph all at once.
    // The third parameter is the number of vertices: this allows for disconnected vertices.
    return graph_t(e.begin(), e.end(), n);
}

/** Initialize a graph with weights **/
template<unsigned long N>
weighted_graph_t createCn_weighted(std::array<int, N> &weights) {
    std::array<std::pair<int, int>, N> edges;
    for (auto i=0; i < N; ++i)
        edges[i] = std::make_pair(i, (i+3) % N);

    return weighted_graph_t(edges.begin(), edges.end(), weights.begin(), N);
}