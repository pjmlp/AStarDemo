/* AStarViewModel.cpp - ViewModel for the common UI code
* Copyright (C) 2020 Paulo Pinto
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

#include "pch.h"
#include <sstream>

#include "Map.h"

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

#include "Logger.h"

#include <winrt/Windows.Storage.h>
#include <winrt/Windows.UI.Xaml.Input.h>

#include "AStarViewModel.h"
#if __has_include("AStarViewModel.g.cpp")
#include "AStarViewModel.g.cpp"
#endif



using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Storage;

namespace winrt::AStarDemo::implementation
{
    AStarViewModel::AStarViewModel(): goButtonEnabled(false), map(), solver(map), show_gridlines(false), running(false)
    {

    }

    bool AStarViewModel::GoButtonEnabled()
    {
        return goButtonEnabled;
    }

    void AStarViewModel::GoButtonEnabled(bool value)
    {
        goButtonEnabled = value;
    }

    IAsyncAction AStarViewModel::LoadFile(::winrt::Windows::Storage::StorageFile const& file)
    {
        winrt::hstring data = co_await FileIO::ReadTextAsync(file);
        std::wstring str(data.data());
        std::wistringstream buffer(str);

        if (loadMap(buffer))
        {
            goButtonEnabled = true;
            NotifyPropertyChanged(L"GoButtonEnabled");
        }
    }

    void AStarViewModel::Draw(::winrt::Microsoft::Graphics::Canvas::CanvasDrawingSession const& renderTarget, ::winrt::Windows::Foundation::Size const& size)
    {
        if (renderTarget != nullptr) {
            if (map.columns() > 0 && map.rows() > 0) {
                dx = size.Width / map.columns();
                marginx = (static_cast<int>(size.Width) % map.columns()) / 2;
                dy = size.Height / map.rows();
                marginy = (static_cast<int>(size.Height) % map.rows()) / 2;

                // render the background
                renderTarget.Clear(::winrt::Windows::UI::Colors::White());

                draw_map(renderTarget, size);

                if (show_gridlines) {
                    draw_grid(renderTarget, size);
                }
            }
        }
    }

    /**
     *  stops the current search if any and clears the loaded map.
     */
    void  AStarViewModel::ClearMap_Click(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args)
    {
        stopSearch();
        map.clear();
    }

    void  AStarViewModel::Search_Click(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args)
    {
        goButtonEnabled = false;
        NotifyPropertyChanged(L"GoButtonEnabled");

        startSearch();
    }

    void  AStarViewModel::Stop_Click(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args)
    {
        if (running) {
            stopSearch();
            goButtonEnabled = true;
            NotifyPropertyChanged(L"GoButtonEnabled");
        }
    }

    void AStarViewModel::MapImage_DoubleTapped(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::Input::DoubleTappedRoutedEventArgs const& args)
    {
        auto point = args.GetPosition(nullptr);
        auto col = (point.X - marginx) / dx;
        auto row = (point.Y - marginy) / dy;

        auto startPos = map.get_start();
        auto endPos = map.get_end();

        if (startPos.first == -1 && startPos.second == -1) {
            map.set_pos(row, col, Map::CellType::START);
        }
        else if (endPos.first == -1 && endPos.second == -1) {
            map.set_pos(row, col, Map::CellType::END);
        }
    }

    void AStarViewModel::NotifyPropertyChanged(winrt::hstring const& field)
    {
        propertyChanged(*this, Windows::UI::Xaml::Data::PropertyChangedEventArgs{ field });
    }

    winrt::event_token AStarViewModel::PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
    {
        return propertyChanged.add(handler);
    }

    void AStarViewModel::PropertyChanged(winrt::event_token const& token)
    {
        propertyChanged.remove(token);
    }

    /**
 *  AStarViewModel::startSearch
 *
 * Starts the background processing for the A* search with
 * the currently loaded map.
 */
    void AStarViewModel::startSearch()
    {
        auto startPos = map.get_start();
        auto endPos = map.get_end();

        startSearch(startPos.first, startPos.second, endPos.first, endPos.second);
    }

    /**
     *  AStarViewModel::startSearch
     *
     * Starts the background processing for the A* search with
     * the currently loaded map.
     */
    void AStarViewModel::startSearch(int x0, int y0, int x1, int y1)
    {
        running = true;

        backTask = std::async(std::launch::async, [this, x0, y0, x1, y1]() {
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
    void AStarViewModel::stopSearch()
    {
        running = false;
    }


    /**
    *  loads a new map into the application.
    */
    bool AStarViewModel::loadMap(std::wistream& fd)
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

    void AStarViewModel::enable_gridlines()
    {
        show_gridlines = true;
    }

    void AStarViewModel::disable_gridlines()
    {
        show_gridlines = false;
    }

    /**
     *  MapRender::draw_grid Draws the map grid around the map cells.
     * @param painter
     */
    void AStarViewModel::draw_grid(::winrt::Microsoft::Graphics::Canvas::CanvasDrawingSession const& painter, ::winrt::Windows::Foundation::Size size) const
    {
        // now do the vertical lines
        for (float x = 0.0f; x < size.Width; x += dx) {
            painter.DrawLine(x, 0.0f, x, size.Height, ::winrt::Windows::UI::Colors::Black());
        }

        // now do the horizontal lines
        for (float y = 0.0f; y < size.Height; y += dy) {
            painter.DrawLine(0.0f, y, size.Width, y, ::winrt::Windows::UI::Colors::Black());
        }
    }

    /**
     *  MapRender::draw_map Draws the real map.
     * @param painter
     */
    void AStarViewModel::draw_map(::winrt::Microsoft::Graphics::Canvas::CanvasDrawingSession const& painter, ::winrt::Windows::Foundation::Size size) const
    {

        // now draw the real map contents
        int r = 0, g = 0, b = 0;
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

                ::winrt::Windows::Foundation::Rect rect(marginx + (col * dx), marginy + (row * dy), dx, dy);
                ::winrt::Windows::UI::Color color = ::winrt::Windows::UI::ColorHelper::FromArgb(255, r, g, b);
                painter.FillRectangle(rect, color);
            }
        }
    }
}
