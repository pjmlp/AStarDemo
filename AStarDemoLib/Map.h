/* map.h - Map definition
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
#ifndef MAP_H
#define MAP_H

#pragma once

#include <cassert>
#include <string>
#include <memory>
#include <mutex>
#include <utility>
#include <vector>
#include <iosfwd>

// forward declarations
class Node;

/**
 * Class to represent the maps used for the A* algorithm.
 */
class Map
{
public:
    enum class CellType {FREE, BLOCKED, VISITED, NODE_PATH, START, END};

    explicit Map() noexcept;
    explicit Map(int rows, int cols);


	bool load(std::wistream& fd);

	void clear() noexcept;

	int columns() const noexcept { return mapCols;  }

	int rows() const noexcept { return mapRows; }

    int tilesWidth() const noexcept { return tileWidth; }

    int tilesHeigth() const noexcept { return tileHeigth; }

    std::string tilesetFilename() const noexcept { return tileset; }

	// Declared as inline member function so that we get the abstraction
	// without speed penalty. 
    [[gsl::suppress(bounds.4)]]
	CellType at (int row, int col) const {
        // already bound checked, hence disabling the check above
        assert((col >= 0 && col < mapCols) && (row >= 0 && row < mapRows));
        std::lock_guard<std::mutex> lock(m_map_mutex);
        return m_map[row][col]; 
    }

    [[gsl::suppress(bounds.4)]]
    void set_pos (int row, int col, CellType cell) {
        // already bound checked, hence disabling the check above
        assert((col >= 0 && col < mapCols) && (row >= 0 && row < mapRows));
        std::lock_guard<std::mutex> lock(m_map_mutex);
        m_map[row][col] = cell;
        if (cell == CellType::START) {
            start = std::make_pair(row, col);
        } else if (cell == CellType::END) {
            end = std::make_pair(row, col);
        }
    }

    [[gsl::suppress(bounds.4)]]
    void visit(int row, int col) {
        assert((col >= 0 && col < mapCols) && (row >= 0 && row < mapRows));
        std::lock_guard<std::mutex> lock(m_map_mutex);
        m_map[row][col] = CellType::VISITED;
    }

    void dump_map();
    void add_path(Node* path);

    const std::pair<int, int>& get_start() const noexcept { return start; }
    const std::pair<int, int>& get_end() const noexcept { return end; }

private:
	std::vector<std::vector<CellType>> m_map;
    mutable std::mutex m_map_mutex;
    std::pair<int, int> start, end;
	int mapRows, mapCols;
    int tileWidth, tileHeigth;
    std::string tileset;
};

#endif // MAP_H
