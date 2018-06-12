/**
 * disjkstra_userdef.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Dijksta's shortest path algorithm with user-defined parameters.
 */

#include <random>
#include <array>
#include <iostream>
#include <string>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/named_function_params.hpp>

int main() {
    unsigned long constexpr v = 8;

    // We use a user-defined class, edge_properties, rather than a predefined property.
    std::array<std::pair<int, int>, v> edges;
    for (auto i = 0; i < v; ++i)
        edges[i] = std::make_pair(i, (i + 1) % v);

    // We'll colour the vertices for the hell of it.
    struct vertex_properties {
        std::string vcolour;
    };
    // The property we will use and define by hand instead of using edge_weight_t
    struct edge_properties {
        double weight;
        std::string ecolour;
    };

    // Now assign weights and, just for the hell of it, colours randomly.
    auto colours = std::array<std::string, 6> {"red", "orange", "yellow", "green", "blue", "violet"};

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> colour_dist(0, 5);
    std::uniform_real_distribution<double> unif(0, 100);

    using graph = boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS,
            vertex_properties, edge_properties>;

    graph g{edges.begin(), edges.end(), v};
    {
        auto[vit, vend] = boost::vertices(g);
        for (; vit != vend; ++vit)
            g[*vit].vcolour = colours[colour_dist(gen)];
        auto[eit, eend] = boost::edges(g);
        for (; eit != eend; ++eit) {
            g[*eit].ecolour = colours[colour_dist(gen)];
            g[*eit].weight = unif(gen);
        }
    }

    std::array<int, v> directions;
    boost::dijkstra_shortest_paths(g, 0, boost::predecessor_map(directions.begin()).weight_map(boost::get(&edge_properties::weight, g)));

    auto [eit,eend] = boost::edges(g);
    std::cout << "Weights:" << std::endl;
    for (; eit != eend; ++eit) {
        std::cout << "Edge (" << eit->m_source << "," << eit->m_target << ") = " << g[*eit].weight << " and " << g[*eit].ecolour << std::endl;
    }
    auto [vit,vend] = boost::vertices(g);
    for (auto i=0; i < v; ++i)
        std::cout << "Vertex " << i << " (" << g[*(vit+i)].vcolour << ") has predecessor " << directions[i] << std::endl;
    return 0;
}