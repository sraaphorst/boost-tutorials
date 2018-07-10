/**
* play.cpp
*
* By Sebastian Raaphorst, 2018.
*/

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graphviz.hpp>
#include <iostream>
#include <vector>
#include <tuple>
using namespace std;


struct VertexInfo {
    int vertexIdx;
    bool visited;
    bool essential;
};
struct VertexInfoPropertyTag {
    using kind = boost::vertex_property_tag;
    static const std::size_t num;
};
const std::size_t VertexInfoPropertyTag::num = (std::size_t)&VertexInfoPropertyTag::num;

struct EdgeInfo {
    int weight = 1;
    bool carved = false;
    bool wrapped = false;
};
struct EdgeInfoPropertyTag {
    using kind = boost::edge_property_tag;
    static const std::size_t num;
};
const std::size_t EdgeInfoPropertyTag::num = (std::size_t)&EdgeInfoPropertyTag::num;

using vertex_info_prop_type = boost::property<VertexInfoPropertyTag, VertexInfo>;
using edge_info_prop_type = boost::property<EdgeInfoPropertyTag, EdgeInfo>;

using Maze = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, vertex_info_prop_type, edge_info_prop_type>;
using Vertex = Maze::vertex_descriptor;
using vertices_size_type = boost::graph_traits<Maze>::vertices_size_type;
using Edge = Maze::edge_descriptor;

int main() {
    Maze g;

    // Make a toroidal graph because why not?
    constexpr int W = 10;
    constexpr int H = 5;
    constexpr int INRANGE = W * H; // positions 0 through 49 are valid

    // This will be the code that causes looping around the torus in the model.
    const auto normalize = [&](int &x, int &y) {
        while (x < 0) x += W;
        while (y < 0) y += H;
    };

    const auto ranker = [&](int x, int y) {
        normalize(x, y);
        return (y % H) * W + (x % W);
    };

    const auto vertex_prop_maker = [&](int x, int y) {
        normalize(x, y);
        return VertexInfo{ranker(x,y), false, x == 0 || x == W || y == 0 || y == H};
    };

    const auto edge_prop_maker = [&](int x1, int y1, int x2, int y2) {
        bool wrapped = (x1 < 0 || x1 >= W || x2 < 0 || x2 >= W || y1 < 0 || y1 >= H || y2 < 0 || y2 >= H);
        return EdgeInfo{1, false, wrapped};
    };


    const auto vertex = [&](Vertex v) -> VertexInfo& {
        auto p1 = boost::vertex(v, g);
        VertexInfo &p = boost::get(VertexInfoPropertyTag(), g, p1);
        return p;
    };

    const auto edge = [&g](Vertex v1, Vertex v2) -> EdgeInfo& {
        auto [p1, p2] = boost::edge(v1, v2, g);
        EdgeInfo &p = boost::get(EdgeInfoPropertyTag(), g, p1);
        return p;
    };

    vector<int> delta = {-1, 0, 1};

    vector<pair<int, int>> deltas;
    deltas.reserve(8);
    for (const auto dx: delta)
        for (const auto dy: delta)
            if (dx != 0 || dy != 0)
                deltas.emplace_back(make_pair(dx, dy));

    for (auto y = 0; y < H; ++y) {
        for (auto x = 0; x < W; ++x) {
            const auto props = vertex_prop_maker(x, y);
            const Vertex v = boost::add_vertex(props, g);
        }
    }

    // Now add the edges.
    for (auto y = 0; y < H; ++y) {
        for (auto x = 0; x < W; ++x) {
            const Vertex v = ranker(x,y);
            for (const auto [dx, dy]: deltas) {
                const Vertex w = ranker(x + dx, y + dy);
                const auto props = edge_prop_maker(x, y, x + dx, y + dy);
                boost::add_edge(v, w, props, g);
            }
        }
    }


    for (auto [vit, vend] = boost::vertices(g); vit != vend; ++vit) {
        const auto &v = *vit;
        const auto &vi = vertex(v);
        cout << "vertex " << v << " should be " << vi.vertexIdx
             << ", visited=" << vi.visited << ", essential: "
             << vi.essential << endl;

    }

    cout << endl << endl;
    for (auto [eit, eend] = boost::edges(g); eit != eend; ++eit) {
        const auto &e = *eit;
        const Vertex v1 = e.m_source;
        const Vertex v2 = e.m_target;
        if ((v1 + v2) % 2 == 0) {
            auto &ep = edge(v1, v2);
            ep.carved = true;
            ep.weight += v1 % v2;
        }

        const auto &ep = edge(v1, v2);
        cout << v1 << " -> " << v2 << " has weight=" << ep.weight << ", carved=" << ep.carved << ", wrap=" << ep.wrapped << endl;
    }
    boost::write_graphviz(cout, g);
}