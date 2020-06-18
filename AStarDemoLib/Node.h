/* node.h - Node definition
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
#ifndef NODE_H
#define NODE_H

#pragma once

#include <memory>
#include <iosfwd>

/**
 * This class represents nodes in the path
 */
class Node final
{
public:
    Node(int row, int col);

    void set_parent(std::shared_ptr<Node> parent) { m_parent = parent; }
    std::shared_ptr<Node> get_parent() { return m_parent; }

	// accessor functions
	int row() const { return m_row; }
	int col() const { return m_col; }
	double cost() const { return m_cost; }
	double estimation() const { return m_estimation; }
	double total_cost() const { return m_cost + m_estimation; }

	// setters
	void set_cost(double cost) { m_cost = cost; }
	void set_estimation(double estimation) { m_estimation = estimation; }

	bool operator== (const Node& other) const;
	bool operator!= (const Node& other) const;

	void write_contents(std::ostream& output) const;

private:
	int m_row, m_col;
	double m_cost;
	double m_estimation;
	std::shared_ptr<Node> m_parent;
};

#endif // NODE_H
