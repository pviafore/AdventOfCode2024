#include <algorithm>
#include <functional>
#include <ranges>
#include <set>
#include <stdexcept>
#include <map>
#include <vector>
namespace graph {

    class GraphException : public std::runtime_error{
    public:
        GraphException(const std::string& msg) : std::runtime_error(msg) {}
    };

    struct Unused{};

    template<typename Node, typename Edge=Unused>
    class Graph {
    public:
        Graph() {}
        using NodeType = Node;
        void addEdge(Node source, Node dest, Edge edge){
            adjacencyList.emplace(std::pair{std::move(source), std::move(dest)}, std::move(edge));
        } 

        void addEdge(Node source, Node dest){
            addEdge(std::move(source), std::move(dest), {});
        }

        void removeNode(const Node& toBeRemoved) {
            for(auto [n, e]: getIncomingEdges(toBeRemoved)){
                adjacencyList.erase({n, toBeRemoved});
            }
            for(auto [n, e]: getOutgoingEdges(toBeRemoved)){
                adjacencyList.erase({toBeRemoved, n});
            }
        }

        bool contains(const Node& source, const Node& dest){
            return adjacencyList.contains({source, dest});
        }

        std::vector<Node> getNodes() const {
            auto nodes = std::views::transform(adjacencyList, [](auto p){ return p.first.first; }) | std::ranges::to<std::set<Node>>();
            auto destinations = std::views::transform(adjacencyList, [](auto p){ return p.first.second; });
            nodes.insert(destinations.begin(), destinations.end());
            return std::vector<Node>{nodes.begin(), nodes.end()};
        }

        bool empty() const {
            return adjacencyList.empty();
        }

        using NodeEdgePairs = std::vector<std::pair<Node, Edge>>;
        NodeEdgePairs getIncomingEdges(const Node& node) const{
            return adjacencyList |
                std::views::filter([&node](auto p){ return p.first.second == node; }) |
                std::views::transform([](auto p) { return std::pair(p.first.first, p.second); }) |
                std::ranges::to<NodeEdgePairs>();
        }

       NodeEdgePairs getOutgoingEdges(const Node& node) const{
            return adjacencyList |
                std::views::filter([&node](auto p){ return p.first.first == node; }) |
                std::views::transform([](auto p) { return std::pair(p.first.second, p.second);}) |
                std::ranges::to<NodeEdgePairs>();
        }

        bool doesPathExist(const Node& n1, const Node& n2) const{
            return BFS(n1, [&n2](const auto& n){return n == n2; }).has_value();
        }

        std::optional<Node> BFS(const Node& startingNode, std::function<bool(const Node& n)> predicate) const{
            std::set<Node> seen;
            std::set<Node> candidates{ startingNode};
            while(!candidates.empty()){
                auto node = *candidates.begin();
                std::print("{}", node);
                if(predicate(node)) {
                    return node;
                }
                seen.insert(node);
                candidates.erase(candidates.begin());
                for(auto [n, _]: getOutgoingEdges(node)){
                    if(!seen.contains(n)){
                        candidates.insert(n);
                    }
                }
            }
            return {};
        }
        
    private:
        std::multimap<std::pair<Node, Node>, Edge> adjacencyList;
    };

    template <typename Graph>
    std::vector<typename Graph::NodeType> topologicalSort(Graph graph) {
        std::vector<typename Graph::NodeType> out;
        auto nodes = graph.getNodes();
        while(!nodes.empty()){
            auto nodesToBeRemoved = nodes | 
                std::views::filter([&graph](auto n){ return graph.getIncomingEdges(n).size() == 0; }) |
                std::ranges::to<std::vector<typename Graph::NodeType>>();
            if(nodesToBeRemoved.empty() && !graph.empty()){
                throw GraphException("Could not topologically sort - a cycle was detected");
            }
            out.insert(out.end(), nodesToBeRemoved.begin(), nodesToBeRemoved.end());
            for(auto n: nodesToBeRemoved){
                graph.removeNode(n);
                if(std::ranges::contains(nodes, n)){

                    nodes.erase(std::ranges::find(nodes, n));
                }
            }
        }
        return out;
    }
}