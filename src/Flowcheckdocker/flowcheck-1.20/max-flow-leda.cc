/* This program provides the same maximum-flow functionality as the
   recommended max-flow-boost.cc, but based on the older graph library
   LEDA. In our experience it often runs faster than the BOOST
   version, but recent versions of LEDA are commercial software, while
   the older versions that were free (at least for research use) are
   no longer distributed.

   We've been able to compile this version of the code with Debian's
   G++ version 2.95 and LEDA 4.1. However, you'd probably be better
   off spending your time optimizing the BOOST version. */

#include <iostream>
#include <hash_map>
#include <string>

template<> struct std::hash<long long> {
    std::size_t operator()(long long val) const {
	return static_cast<std::size_t>(val);
    }
};

typedef std::string stdstring;

#include <LEDA/graph.h>
#include <LEDA/graph_map.h>
#include <LEDA/array.h>
#include <LEDA/stack.h>
#include <LEDA/max_flow.h>

static void strict_assert(void *c) { assert(c); }

typedef GRAPH<int, int> weighted_graph;

void flat_flow(weighted_graph &g, node s, node t) {
    edge_array<int> capacity(g);
    edge_array<int> flow(g);

    edge e;
    forall_edges(e, g) {
	capacity[e] = g[e];
    }

    int max_flow = MAX_FLOW_T(g, s, t, capacity, flow);

    std::cerr << "Maximum flow is " << max_flow << " units" << endl;

    forall_edges(e, g) {
	if (flow[e] != 0) {
	    std::cout << g[g.source(e)] << " " << g[g.target(e)] <<
		" " << flow[e] << "/" << capacity[e] << endl;
	}
    }
}

int main(int argc, char **argv) {
    weighted_graph g;
    hash_map<long long, node> nodes;

    nodes[0] = g.new_node(0);
    nodes[-1] = g.new_node(-1);

    edge e_ref = g.new_edge(nodes[0], nodes[-1], 0);

    while (!cin.eof()) {
	long long src, targ;
	int capacity;
	stdstring unused;
	std::cin >> src;
	if (cin.eof())
	    break;
	strict_assert(std::cin >> targ);
	strict_assert(std::cin >> capacity);
	strict_assert(getline(cin, unused)); // ignore remaining fields
	if (!nodes[src]) {
	    nodes[src] = g.new_node(src);
	}
	if (!nodes[targ]) {
	    nodes[targ] = g.new_node(targ);
	}
	g.new_edge(nodes[src], nodes[targ], capacity);
	//std::cout << "Edge from " << src << " to " << targ << endl;
    }

    flat_flow(g, nodes[0], nodes[-1]);
}
