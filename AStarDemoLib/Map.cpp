/* map.cpp - Map implementation
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
#include <sstream>
#include <fstream>
#include <string>
#include <cstring>

#include "Logger.h"
#include "Node.h"
#include "Map.h"

// make the standard C++ library available on the local namespace
using namespace std;

/**
* Constructs the map by setting its contents to empty.
*/
Map::Map() noexcept: start{ -1, -1 }, end{ -1, -1 }, mapRows{ 0 }, mapCols{ 0 }
{
	clear();
}

Map::Map(int rows, int cols):start { -1, -1 }, end{ -1, -1 }, mapRows{ rows }, mapCols{ cols }
{
	m_map.resize(mapRows);
	for (auto& col : m_map) {
		col.resize(mapCols);
	}
}
/**
* Constructs the map by loading it from the specified filename
* @param filename The filename where to load the data from.
* @return false if there was an error loading the file
*/
bool Map::load(std::wistream& fd)
{
	const size_t MaxRow = 3;
	const wstring version{ L"AStarv20" };

	size_t row = 0;
	wstring str;

	while (!fd.eof()) {
		if (row == 0) {
			fd >> str;
			if (str != version) {
				return false;
			}
		}
		else if (row == 1) {
			fd >> tileWidth >> tileHeigth >> tileset;
		}
		else if (row == 2) {
			fd >> mapRows >> mapCols;
			m_map.resize(mapRows);
			for (auto& col : m_map) {
				col.resize(mapCols);
			}
		}
		else {
			fd >> str;

			for (size_t i = 0; i < mapCols - 1; ++i) {
				if (str.at(i) == '.') {
					m_map.at(row - MaxRow).at(i) = CellType::FREE;
				}
				else {
					m_map.at(row - MaxRow).at(i) = CellType::BLOCKED;
				}
			}
		}
		++row;
	}

	return true;
}

/**
* Clears the map contents
*/
[[gsl::suppress(bounds.4)]] // The for loop already takes care, no need for double check.
void Map::clear() noexcept
{
	for (int row = 0; row < m_map.size(); ++row) {
		for (int col = 0; col < m_map[row].size(); ++col) {
			m_map[row][col] = CellType::FREE;
		}
	}

	start = { -1, -1 };
	end = { -1, -1 };
}

/**
 * To be used as a debugging function. It shows the loaded map
 */
void Map::dump_map()
{
	for (int row = 0; row < mapRows; ++row) {
        for (int col = 0; col < mapCols; ++col) {
            switch (m_map.at(row).at(col)) {
            case CellType::FREE:
                    cout << '.';
                    break;

            case CellType::BLOCKED:
                cout << '*';
                break;

            case CellType::NODE_PATH:
                cout << 'x';
                break;

            case CellType::VISITED:
                cout << '=';
                break;

            case CellType::START:
                cout << 'O';
                break;

            case CellType::END:
                cout << 'X';
                break;
            }
        }

		cout << endl;
	}
}


/**
 * Updates the map information adding the path, from the end to the
 * begining. This is most likely not the best way to do it, but it
 * provides a quick way given the exercise allowed duration.
 *
 * @param path the end of the found path
 */
void Map::add_path(Node * path)
{
    lock_guard<std::mutex> lock(m_map_mutex);
	auto current = path;

    bool first = true;
	while (current != nullptr) {
        if (first) {
            m_map.at(current->row()).at(current->col()) = CellType::END;
			first = false;
        } else if (current->get_parent() == nullptr) {
			m_map.at(current->row()).at(current->col()) = CellType::START;
        } else {
			m_map.at(current->row()).at(current->col()) = CellType::NODE_PATH;
        }
		current = current->get_parent().get();
	}
}

