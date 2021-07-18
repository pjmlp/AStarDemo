export module Map;

import <cassert>;
import <string>;
import <memory>;
import <mutex>;
import <utility>;
import <vector>;
import <iosfwd>;

import Node;

/**
 * Class to represent the maps used for the A* algorithm.
 */
export class Map
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

    std::string tilesetFilename() const noexcept { return tileset; }

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
    std::string tileset;
};