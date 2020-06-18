/* MapWidget.cpp - Widget for rendering the map in Android
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

#include <cstdlib>
#include <cstring>
#include <cassert>
#include <string>
#include <memory>
#include <thread>


#include "Render.h"
#include "Map.h"
#include "Node.h"
#include "AStarSolver.h"

#include "MapWidget.h"

#include "Logger.h"

using namespace std;

MapWidget::MapWidget()
	: map(), solver(map), show_gridlines(false), running(false)
{
}

MapWidget::~MapWidget()
{
    // nothing to destruct
}

/**
 *  MapWidget::startSearch
 *
 * Starts the background processing for the A* search with
 * the currently loaded map.
 */
void MapWidget::startSearch()
{
    auto startPos = map.get_start();
    auto endPos = map.get_end();

    startSearch(startPos.first, startPos.second, endPos.first, endPos.second);
}

/**
 *  MapWidget::startSearch
 *
 * Starts the background processing for the A* search with
 * the currently loaded map.
 */
void MapWidget::startSearch(int x0, int y0, int x1, int y1)
{
	running = true;

	backTask = std::async(std::launch::async, [this, x0, y0, x1, y1](){
        // Initialize the required data

        auto start = std::make_shared<Node>(x0, y0);
        auto end = std::make_shared<Node>(x1, y1);

        // now find the result
        auto res = solver.find(start, end);
        map.add_path(res);
		this->stopSearch();
        return res;
    });


}

/**
 *  Stops the current search, if any.
 */
void MapWidget::stopSearch()
{
	running = false;
}


/**
 * @returns the current execution state.
 */
bool MapWidget::is_running() const
{
	return running;
}

/**
*  loads a new map into the application.
*/
bool MapWidget::loadMap(const string& pathname)
{
    if (pathname != "") {
    	LogInfo("loading map");
        // Just in case another search is ongoing
        if (backTask.valid()) {
        	LogInfo("task sleeping");
            backTask.wait();
        }
        std::string msg = "Loading ";
        msg += pathname;
        LogInfo(msg);

        return map.load(pathname);
       // map.set_pos(cstartPos.x(), cstartPos.y(), Map::CellType::START);
       // map.set_pos(cendPos.x(), cendPos.y(), Map::CellType::END);
    }
    return false;
}

bool MapWidget::loadMap(std::wistream& fd)
{
	LogInfo("loading map");
	// Just in case another search is ongoing
	if (backTask.valid()) {
		LogInfo("task sleeping");
		backTask.wait();
	}
	std::string msg = "Loading file stream";
	LogInfo(msg);

	return map.load(fd);
}

/**
 *  stops the current search if any and clears the loaded map.
 */
void MapWidget::clearMap()
{
	stopSearch();
	map.clear();
}

void MapWidget::paint(Render& painter)
{
	if (map.columns() > 0 && map.rows() > 0) {
		dx = painter.width() / map.columns();
		marginx = (static_cast<int>(painter.width()) % map.columns()) / 2;
		dy = painter.height() / map.rows();
		marginy = (static_cast<int>(painter.height()) % map.rows()) / 2;

		// render the background
		painter.fill_rect(0, 0, painter.width(), painter.height(), 255, 255, 255);

		draw_map(painter);

		if (show_gridlines) {
			draw_grid(painter);
		}
	}
}

void MapWidget::enable_gridlines ()
{
	show_gridlines = true;
}

void MapWidget::disable_gridlines ()
{
	show_gridlines = false;
}

/**
 *  MapRender::mouseDoubleClickEvent
 * @param event
 */
void MapWidget::mouseDoubleClickEvent(float x, float y)
{
    auto col = (x - marginx) / dx;
    auto row = (y - marginy) / dy;

	auto startPos = map.get_start();
	auto endPos = map.get_end();

	if (startPos.first == -1 && startPos.second == -1) {
		map.set_pos(row, col, Map::CellType::START);
	}
	else if (endPos.first == -1 && endPos.second == -1) {
		map.set_pos(row, col, Map::CellType::END);
	}
}


/**
 *  MapRender::draw_grid Draws the map grid around the map cells.
 * @param painter
 */
void MapWidget::draw_grid(Render& painter) const
{
    // now do the vertical lines
    int w = static_cast<int>(painter.width());
    int h = static_cast<int>(painter.height());

    for (int x = 0; x < w; x += dx) {
        painter.draw_line(x, 0, x, h);
    }

    // now do the horizontal lines
    for (int y = 0; y < h; y += dy) {
        painter.draw_line(0, y, w, y);
    }
}

/**
 *  MapRender::draw_map Draws the real map.
 * @param painter
 */
void MapWidget::draw_map(Render& painter) const
{

    // now draw the real map contents
    int r=0, g=0, b=0;
    for (int row = 0; row < map.rows(); ++row) {
        for (int col = 0; col < map.columns(); ++col) {
            switch (map.at(row, col)) {
            case Map::CellType::FREE:
                r = 255; g = 255; b = 255;
                break;

            case Map::CellType::BLOCKED:
                r = 0; g = 255; b = 0;
                break;

            case Map::CellType::VISITED:
            	r = 0; g = 255; b = 255;
                break;

            case Map::CellType::NODE_PATH:
            	r = 255; g = 0; b = 255;
                break;

            case Map::CellType::START:
            	r = 255; g = 0; b = 0;
                break;

            case Map::CellType::END:
            	r = 0; g = 0; b = 255;
                break;
            }

            painter.fill_rect(marginx + (col * dx), marginy + (row * dy), dx, dy, r, g, b);
        }
    }
}
