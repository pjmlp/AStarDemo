/* MapWidget.h - Widget for rendering the map in Android
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
#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#pragma once

#include <future>
#include <string>
#include <iosfwd>

#include "Map.h"
#include "AStarSolver.h"

// forward declarations
class Render;

/**
 * Widget for rendering the map in Android
 */
class MapWidget
{

public:
    MapWidget();
    ~MapWidget();

    void startSearch();
	void stopSearch();
    void startSearch(int x0, int y0, int x1, int y1);
    bool loadMap(const std::string& pathname);
	bool loadMap(std::wistream& fd);

	void clearMap();

	bool is_running() const;

    void paint(Render& painter);
    void mouseDoubleClickEvent(float x, float y);

    void enable_gridlines ();
    void disable_gridlines ();

private:
    Map map;
    AStarSolver solver;
    bool show_gridlines;
    int marginx, marginy;
    int dx, dy;
	bool running;

    void draw_grid(Render& painter) const;
    void draw_map(Render& painter) const;
    void draw_nodes(Render& painter) const;


    // Handle for the A* background processing.
    std::future<AStarSolver::NodePtr> backTask;
};

#endif // MAPWIDGET_H
