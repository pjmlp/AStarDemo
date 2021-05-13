module;

#include <vector>
#include <memory>

export module AStarSolver;

import Node;
import Map;

/**
 * Searchs for a possible path between two given points by using the A* algorithm.
 */
class AStarSolver
{
public:
    using NodePtr = std::shared_ptr<Node>;
    AStarSolver(Map& map) noexcept;

    NodePtr find(NodePtr start, NodePtr goal);

private:
    Map& m_map;


    double movement_cost(const Node& from, const Node& to) const noexcept;
    double estimate(const Node& current, const Node& goal) const noexcept;
    void sucessors(NodePtr current, const Node& goal, std::vector<NodePtr>& neighbours);
};