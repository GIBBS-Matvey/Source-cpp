#include <iostream>
#include <vector>
#include <forward_list>
#include <string>


class Visitor {
protected:
    std::vector<int> v_call_order;
    std::vector<std::string> color;
    int vertex_number;
public:

    Visitor(int v_number) : vertex_number(v_number), color(v_number, "white") {
        v_call_order.reserve(v_number);
        for (int i = 0; i < v_number; ++i) {
            v_call_order.push_back(i);
        }
    }

    const std::vector<int>& get_order_vector() {
        return v_call_order;
    }

    const std::string& get_color(int v) {
        return color[v];
    }

    void start_vertex(int v) {}

    void discover_vertex(int v) {
        color[v] = "gray";
    }

    void finish_vertex(int v) {
        color[v] = "black";
    }
};



class Top_sort_visitor : public Visitor {
private:
    std::forward_list<int> sorted_v;
public:
    Top_sort_visitor(int v_number) : Visitor(v_number) {}

    void discover_vertex(int v) {
        color[v] = "gray";
    }

    void finish_vertex(int v) {
        sorted_v.push_front(v);
        color[v] = "black";
    }

    std::forward_list<int> get_sorted_v() {
        return sorted_v;
    }
};



class Scc_visitor : public Visitor {
private:
    std::vector<int> scc;
    int cur_scc;
    
public:
    Scc_visitor(int v_number, std::vector<int>& call_order) : Visitor(v_number), scc(v_number) {
        for (int i = 0; i < v_number; ++i) {
            v_call_order[i] = call_order[i];
        }
        cur_scc = -1;
    }

    void start_vertex(int v) {
        ++cur_scc;
    }

    void discover_vertex(int v) {
        color[v] = "gray";
        scc[v] = cur_scc;
    }

    void finish_vertex(int v) {
        color[v] = "black";
    }

    std::pair<int, std::vector<int>> get_kosaraju_pair() const {
        return {cur_scc + 1, scc};
    }
};



class Graph_ {
private:
    std::vector<std::vector<int>> adj;
    int vertex_number;
public:
    Graph_(int v_number) : vertex_number(v_number), adj(v_number, std::vector<int>()) {}

    void add_edge(int from, int to) {
        adj[from].push_back(to);
    }

    template<typename Visitor>
    void dfs(Visitor& vis) {
        for (int v : vis.get_order_vector()) {
            if (vis.get_color(v) == "white") {
                vis.start_vertex(v);
                dfs_visit(v, vis);
            }
        }
    }

    template<typename Visitor>
    void dfs_visit(int vertex, Visitor& visitor) {
        visitor.discover_vertex(vertex);
        for (int nei : adj[vertex]) {
            if (visitor.get_color(nei) == "white") {
                dfs_visit(nei, visitor);
            }
        }
        visitor.finish_vertex(vertex);
    }

    std::vector<int> top_sort() {
        Top_sort_visitor top_sort_vis(vertex_number);
        dfs<Top_sort_visitor>(top_sort_vis);

        std::vector<int> sorted_v;
        sorted_v.reserve(vertex_number);
        for (int v : top_sort_vis.get_sorted_v()) {
            sorted_v.push_back(v);
        }
        return sorted_v;
    }

    Graph_ get_transposed() const {
        Graph_ trans_graph(vertex_number);
        for (int i = 0; i < vertex_number; ++i) {
            for (int nei : adj[i]) {
                trans_graph.add_edge(nei, i);
            }
        }
        return trans_graph;
    }

    std::pair<int, std::vector<int>> Kosaraju() {
        std::vector<int> sorted_vertexes = top_sort();
        Scc_visitor scc_visitor(vertex_number, sorted_vertexes);
        get_transposed().dfs<Scc_visitor>(scc_visitor);
        return scc_visitor.get_kosaraju_pair();
    }

};


int main() {
    size_t vertex_number;
    std::cin >> vertex_number;

    size_t edge_number;
    std::cin >> edge_number;

    Graph_ graph(vertex_number);

    for (size_t i = 0; i < edge_number; ++i) {
        int from, to;
        std::cin >> from >> to;
        graph.add_edge(from - 1, to - 1);
    }

    std::pair<int, std::vector<int>> kos_pair = graph.Kosaraju();
    std::cout << kos_pair.first << '\n';
    for (int scc_num : kos_pair.second) {
        std::cout << scc_num + 1 << ' ';
    }
    
    return 0;
}
