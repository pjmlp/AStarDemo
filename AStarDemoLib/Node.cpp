/* node.cpp - Node implementation
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

#include <iostream>

#include "Node.h"


// make the standard C++ library available on the local namespace
using namespace std;


Node::Node(int row, int col) noexcept: m_row(row), m_col(col), m_cost(0.0), m_estimation(0.0), m_parent(nullptr)
{

}
/**
 * Assume that two nodes for the same cell are the same node.
 * @param other node to compare
 */
bool Node::operator== (const Node& other) const noexcept
{
	if (this != &other) {
		return (m_col == other.m_col) && (m_row == other.m_row);
	}
	else {
		return true;
	}
}

/**
 * Assume that two nodes for the same cell are the same node.
 * @param other node to compare
 */
bool Node::operator!= (const Node& other) const noexcept
{
	return !(*this == other);
}

/**
 * Writes the node contents to the standard output
 * @param output a stream used to capture the node contents
 */
void Node::write_contents(std::ostream& output) const
{
	output << "(" << m_col << ", " << m_row << ", " << m_cost << ", " <<  m_estimation<< ") ";
}
