/* AStarSolver.h - A* solver algorithm interface
* Copyright (C) 2014 Paulo Pinto
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


#ifndef ASTARSOLVER_H
#define ASTARSOLVER_H

#pragma once

#include <vector>
#include <memory>

// forward declarations
class Node;
class Map;


/**
 * Searchs for a possible path between two given points by using the A* algorithm.
 */
class AStarSolver
{
public:
    using NodePtr = std::shared_ptr<Node> ;
	AStarSolver(Map& map) noexcept;

    NodePtr find(NodePtr start, NodePtr goal);

private:
	Map& m_map;
    

    double movement_cost(const Node& from, const Node& to) const noexcept;
	double estimate(const Node& current, const Node& goal) const noexcept;
    void sucessors(NodePtr current, const Node& goal, std::vector<NodePtr>& neighbours);
};

#endif //ASTARSOLVER_H
