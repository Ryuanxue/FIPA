#include <boost/config.hpp>
#include <iostream>
#include <string>
#include <tr1/unordered_map>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/adjacency_list.hpp>

struct myhash {
    std::size_t operator()(long long val) const {
        return static_cast<std::size_t>(val);
    }
};

using namespace std;
using namespace std::tr1;

static void strict_assert(void *c) { assert(c); }

int main(int argc, char **argv) {
    using namespace boost;

    typedef adjacency_list_traits<vecS, vecS, directedS> MyTraits;
    typedef adjacency_list<vecS, vecS, directedS,
	property<vertex_name_t, long long>,
	property<edge_capacity_t, long long,
	property<edge_residual_capacity_t, long long,
	property<edge_reverse_t, MyTraits::edge_descriptor> > > >
	MyGraph;

    MyGraph g;

    property_map<MyGraph, vertex_name_t>::type
	vindex = get(vertex_name, g);
    property_map<MyGraph, edge_capacity_t>::type 
	capacity = get(edge_capacity, g);
    property_map<MyGraph, edge_reverse_t>::type 
	rev = get(edge_reverse, g);
    property_map<MyGraph, edge_residual_capacity_t>::type 
	residual_capacity = get(edge_residual_capacity, g);
    
    unordered_map<long long, MyTraits::vertex_descriptor, myhash> nodes;
    
    nodes[0] = add_vertex(g);
    vindex[nodes[0]] = 0;
    nodes[-1] = add_vertex(g);
    vindex[nodes[-1]] = -1;

    while (!cin.eof()) {
        long long src, targ;
        long long cap;
	std::string unused;
        std::cin >> src;
        if (cin.eof())
            break;
        strict_assert(std::cin >> targ);
        strict_assert(std::cin >> cap);
	strict_assert(getline(cin, unused)); // ignore remaining fields
        if (!nodes[src]) {
            nodes[src] = add_vertex(g);
	    vindex[nodes[src]] = src;
        }
        if (!nodes[targ]) {
            nodes[targ] = add_vertex(g);
	    vindex[nodes[targ]] = targ;
        }
	MyTraits::edge_descriptor e, er;
	bool ok;
	boost::tie(e, ok) = add_edge(nodes[src], nodes[targ], g);
	assert(ok);
	boost::tie(er, ok) = add_edge(nodes[targ], nodes[src], g);
	assert(ok);
	capacity[e] = cap;
	capacity[er] = 0;
	rev[e] = er;
	rev[er] = e;

        //std::cout << "Edge from " << src << " to " << targ << endl;
    }

    long long max_flow = push_relabel_max_flow(g, nodes[0], nodes[-1]);

    std::cerr << "Maximum flow is " << max_flow << " units" << endl;
	
    graph_traits<MyGraph>::vertex_iterator u_iter, u_end;
    graph_traits<MyGraph>::out_edge_iterator ei, e_end;
    for (boost::tie(u_iter, u_end) = vertices(g); u_iter != u_end; ++u_iter)
      for (boost::tie(ei, e_end) = out_edges(*u_iter, g); ei != e_end; ++ei)
	if (capacity[*ei] > 0) {
	    int flow = capacity[*ei] - residual_capacity[*ei];
	    if (flow != 0) {
		std::cout << vindex[*u_iter] << " " 
			  << vindex[target(*ei, g)] <<
		    " " << flow << "/" << capacity[*ei] << endl;
	    }
	}

    return 0;
}
