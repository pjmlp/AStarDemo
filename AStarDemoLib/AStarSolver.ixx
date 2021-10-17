/* AStarSolver.ixx - A* solver algorithm implementation
 * Copyright (C) 2021 Paulo Pinto
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */
export module AStarSolver;

import <memory>;
import <unordered_set>;
import <queue>;
import <vector>;
import <algorithm>;
import <string>;
import <functional>;
import <cassert>;
import <format>;

import Node;
import Map;
import Logger;

export namespace AStarLib {

    /**
     * Searchs for a possible path between two given points by using the A* algorithm.
     */
    export class AStarSolver
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
}

// Uncomment if debugging information for the solver is desired
//#define DEBUG_ASTAR_SOLVER

// make the standard C++ library available on the local namespace
using namespace std;

// also to reduce typing
using namespace AStarLib;

// Helper functions for the ClosedType
size_t hash_func(const AStarSolver::NodePtr& n)
{
    return n->col() + static_cast<size_t>(n->row());
}

bool equal_func(const AStarSolver::NodePtr& lhs, const AStarSolver::NodePtr& rhs)
{
    return (lhs->col() == rhs->col()) && (lhs->row() == rhs->row());
}

bool heap_comparator(const AStarSolver::NodePtr& lhs, const AStarSolver::NodePtr& rhs) noexcept
{
    return lhs->total_cost() > rhs->total_cost();
}

// Helper type definitons
typedef vector<AStarSolver::NodePtr> SucessorsType;
typedef vector<AStarSolver::NodePtr> OpenType;
typedef unordered_set<AStarSolver::NodePtr, function<decltype(hash_func)>, function<decltype(equal_func)>> ClosedType;


/**
* Debuging function. I did not declare it as
* private members, so that I don't need to expose the implementation.
*/
template<typename T>
void write_data(ostream& output, const string& name, const T& data)
{
    LogInfo(std::format("Start {}", name));
    for (shared_ptr<Node> next_node : data) {
        if (next_node != nullptr) {
            next_node->write_contents(output);
        }
    }
    LogInfo(std::format("\nEnd {}", name));
}



AStarSolver::AStarSolver(Map& map) noexcept : m_map(map)
{
}

/**
 * A* search function
 * The caller is responsible for cleaning the memory allocated for the
 * path. The arguments are assumed to be allocated on the heap. [[gsl::suppress(lifetime.3)]]
 *
 * @param start where to start searching from
 * @param goal   the target destination
 * @return null if nothing was found, the reversed path otherwise.
 */
AStarSolver::NodePtr AStarSolver::find(NodePtr start, NodePtr goal)
{
    SucessorsType neighbours;
    OpenType open_list;
    ClosedType closed_list(50, hash_func, equal_func);

    open_list.push_back(start);
    make_heap(open_list.begin(), open_list.end(), heap_comparator);

    while (open_list.size() > 0) {
        // Get the top element from the Open list
        auto current = open_list.front();
        assert(current != nullptr);

        pop_heap(open_list.begin(), open_list.end(), heap_comparator);
        open_list.pop_back();
        closed_list.insert(current);

        m_map.visit(current->row(), current->col());

#ifdef DEBUG_ASTAR_SOLVER
        current->write_contents();
        write_data("OPEN", open_list);
        write_data("CLOSED", closed_list);
#endif // DEBUG_ASTAR_SOLVER

        // have we found our destination?
        if (*current == *goal) {
            return current;
        }
        else {
            // no, then keep on searching
            sucessors(current, *goal, neighbours);


            while (neighbours.size() > 0) {
                auto next_node = neighbours.back();
                neighbours.pop_back();

                auto search_closed = closed_list.find(next_node);
                if (search_closed != closed_list.end()) {
                    continue;
                }


                const double cost = current->cost() + movement_cost(*current, *next_node);

                auto openFound = std::find_if(std::begin(open_list), std::end(open_list), [current](const NodePtr succ) noexcept {
                    return *current == *succ;
                    });
                if (openFound != std::end(open_list)) {
                    auto n = (*openFound);
                    if (cost < n->cost()) {
                        n->set_cost(cost);
                        make_heap(std::begin(open_list), std::end(open_list), heap_comparator); // need to rebalance the queue
                    }
                }
                else {
                    next_node->set_cost(cost);
                    open_list.push_back(next_node);
                    push_heap(std::begin(open_list), std::end(open_list), heap_comparator);
                }
            }
        }
    }

    return nullptr;
}


/**
 * Searches all the sucessor nodes from the current state.
 * @param current the node to generate the sucessors
 * @param goal the end position that we want to reach
 * @param neighbours the list of valid sucessor nodes. It is the caller's responsability
 * to delete them.
 */
void AStarSolver::sucessors(NodePtr current, const Node& goal, vector<NodePtr>& neighbours)
{
    const int col_min = max(current->col() - 1, 0);
    const int col_max = min(current->col() + 2, m_map.columns() - 1);

    const int row_min = max(current->row() - 1, 0);
    const int row_max = min(current->row() + 2, m_map.rows() - 1);

    for (int row = row_min; row < row_max; ++row) {
        for (int col = col_min; col < col_max; ++col) {
            // avoid using the current node or crossing walls
            if (!(row == current->row() && col == current->col()) &&
                (m_map.at(row, col) != Map::CellType::BLOCKED)) {

                // Not clear how to make VC++ happy. Apparently make_shared() is to be avoided, although
                // push_back expects a shared_ptr.
                [[gsl::suppress(lifetime.5)]]
                auto neighbour = make_shared<Node>(row, col);

                neighbour->set_parent(current);
                neighbour->set_estimation(estimate(*current, goal));
                neighbours.push_back(neighbour);
            }
        }
    }
}

/**
 * Cost function for reaching the current state
 */
double AStarSolver::movement_cost(const Node& from, const Node& to) const noexcept
{
    // make the diagonals cost a bit more than horizontal/vertical deplacements
    const double dx = abs(from.col() - to.col());
    const double dy = abs(from.row() - to.row());
    return (dx + dy) < 2.0 ? 1.0 : 1.5;
}


/**
 * Heuristic function
 */
double AStarSolver::estimate(const Node& current, const Node& goal) const noexcept
{
    // estimate using Manhattan distance with diagonals
    const double dx = abs(current.col() - goal.col());
    const double dy = abs(current.row() - goal.row());
    return sqrt(dx * dx + dy * dy);
}
