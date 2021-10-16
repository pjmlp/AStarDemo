/* map.ixx - Map implementation
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
export module Map;

import <cassert>;
import <string>;
import <memory>;
import <mutex>;
import <utility>;
import <vector>;
import <iostream>;
import <sstream>;
import <fstream>;

import Node;

/**
 * Class to represent the maps used for the A* algorithm.
 */
export class Map final
{
public:
    enum class CellType { FREE, BLOCKED, VISITED, NODE_PATH, START, END };

    explicit Map() noexcept;
    explicit Map(int rows, int cols);


    bool load(std::wistream& fd);

    void clear() noexcept;

    int columns() const noexcept { return mapCols; }

    int rows() const noexcept { return mapRows; }

    int tilesWidth() const noexcept { return tileWidth; }

    int tilesHeigth() const noexcept { return tileHeigth; }

    std::wstring tilesetFilename() const noexcept { return tileset; }

    // Declared as inline member function so that we get the abstraction
    // without speed penalty. 
    [[gsl::suppress(bounds.4)]]
    CellType at(int row, int col) const {
        // already bound checked, hence disabling the check above
        assert((col >= 0 && col < mapCols) && (row >= 0 && row < mapRows));
        std::lock_guard<std::mutex> lock(m_map_mutex);
        return m_map[row][col];
    }

    [[gsl::suppress(bounds.4)]]
    void set_pos(int row, int col, CellType cell) {
        // already bound checked, hence disabling the check above
        assert((col >= 0 && col < mapCols) && (row >= 0 && row < mapRows));
        std::lock_guard<std::mutex> lock(m_map_mutex);
        m_map[row][col] = cell;
        if (cell == CellType::START) {
            start = std::make_pair(row, col);
        }
        else if (cell == CellType::END) {
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
    std::wstring tileset;
};

// make the standard C++ library available on the local namespace
using namespace std;


/**
* Constructs the map by setting its contents to empty.
*/
Map::Map() noexcept : start{ -1, -1 }, end{ -1, -1 }, mapRows{ 0 }, mapCols{ 0 }
{
    clear();
}

Map::Map(int rows, int cols) : start{ -1, -1 }, end{ -1, -1 }, mapRows{ rows }, mapCols{ cols }
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
    const wstring version = L"AStarv20";

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
void Map::add_path(Node* path)
{
    lock_guard<std::mutex> lock(m_map_mutex);
    auto current = path;

    bool first = true;
    while (current != nullptr) {
        if (first) {
            m_map.at(current->row()).at(current->col()) = CellType::END;
            first = false;
        }
        else if (current->get_parent() == nullptr) {
            m_map.at(current->row()).at(current->col()) = CellType::START;
        }
        else {
            m_map.at(current->row()).at(current->col()) = CellType::NODE_PATH;
        }
        current = current->get_parent().get();
    }
}

