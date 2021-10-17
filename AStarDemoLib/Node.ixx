/* node.ixx - Node implementation
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
export module Node;

import <memory>;
import <iostream>;

export namespace AStarLib {

	/**
	 * This class represents nodes in the path
	 */
	export class Node final
	{
	public:
		explicit Node(int row, int col) noexcept;

		void set_parent(std::shared_ptr<Node> parent) noexcept { m_parent = parent; }
		std::shared_ptr<Node> get_parent() noexcept { return m_parent; }

		// accessor functions
		int row() const noexcept { return m_row; }
		int col() const noexcept { return m_col; }
		double cost() const noexcept { return m_cost; }
		double estimation() const noexcept { return m_estimation; }
		double total_cost() const noexcept { return m_cost + m_estimation; }

		// setters
		void set_cost(double cost) noexcept { m_cost = cost; }
		void set_estimation(double estimation) noexcept { m_estimation = estimation; }

		bool operator== (const Node& other) const noexcept;
		bool operator!= (const Node& other) const noexcept;

		void write_contents(std::ostream& output) const;

	private:
		int m_row, m_col;
		double m_cost;
		double m_estimation;
		std::shared_ptr<Node> m_parent;
	};
};

// make the standard C++ library available on the local namespace
using namespace std;

// also to reduce typing
using namespace AStarLib;


Node::Node(int row, int col) noexcept : m_row(row), m_col(col), m_cost(0.0), m_estimation(0.0), m_parent(nullptr)
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
	output << "(" << m_col << ", " << m_row << ", " << m_cost << ", " << m_estimation << ") ";
}
