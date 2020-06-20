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


#include "Map.h"
#include "Node.h"
#include "AStarSolver.h"

#include "Logger.h"

#include "AStarViewModel.h"
#if __has_include("AStarViewModel.g.cpp")
#include "AStarViewModel.g.cpp"
#endif


// to simplify some typing
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Storage;
using namespace winrt::Microsoft::Graphics::Canvas;
using namespace winrt::Windows::UI;

namespace winrt::AStarDemo::implementation
{
    /**
     * Helper member template for setting fields with INotifyPropertyChanged support.
     * Naturally a clever solution with operator=() is possible, but this is just a basic application.
     * @param field the field to change the current value
     * @param value the new value to assign to the field
     * @param fieldname the fieldname to use in the notification.
     */
    template<typename T>
    void AStarViewModel::ChangeFieldValue(T& field, T value, winrt::hstring const& fieldname)
    {
        field = value;
        NotifyPropertyChanged(fieldname);
    }

    AStarViewModel::AStarViewModel(): goButtonEnabled(false), map(), solver(map), running(false)
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

    /**
     * @brief Loads the requested filename from the filesystem.
     */
    IAsyncAction AStarViewModel::LoadFile(StorageFile const& file)
    {
        winrt::hstring data = co_await FileIO::ReadTextAsync(file);
        std::wstring str(data.data());
        std::wistringstream buffer(str);

        if (loadMap(buffer))
        {
            ChangeFieldValue(goButtonEnabled, true, L"GoButtonEnabled");
        }
    }

    /**
     * @brief Draws the current state into the drawing surface, with the provided size.
     */
    void AStarViewModel::Draw(CanvasDrawingSession const& renderTarget, Size const& size)
    {
        if (renderTarget != nullptr) {
            if (map.columns() > 0 && map.rows() > 0) {
                dx = size.Width / map.columns();
                marginx = (static_cast<int>(size.Width) % map.columns()) / 2;
                dy = size.Height / map.rows();
                marginy = (static_cast<int>(size.Height) % map.rows()) / 2;

                // render the background
                renderTarget.Clear(Colors::White());

                draw_map(renderTarget, size);
            }
        }
    }

    /**
     *  @brief stops the current search if any and clears the loaded map.
     */
    void  AStarViewModel::ClearMap_Click(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args)
    {
        stopSearch();
        map.clear();
    }

    /**
     *  @brief Triggers a new search, if none is currently running.
     */
    void  AStarViewModel::Search_Click(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args)
    {
        ChangeFieldValue(goButtonEnabled, false, L"GoButtonEnabled");

        startSearch();
    }

    /**
     *  @brief Stops the current search, if one is taking place.
     */
    void  AStarViewModel::Stop_Click(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args)
    {
        if (running) {
            stopSearch();
            ChangeFieldValue(goButtonEnabled, true, L"GoButtonEnabled");
        }
    }

    /**
     *  @brief Handles double click action.
     */
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

    /**
     * @brief Support member function for the INotifyPropertyChanged UWP interface.
     * @param fieldname The name of field to notify to the bindings
     */
    void AStarViewModel::NotifyPropertyChanged(winrt::hstring const& fieldname)
    {
        propertyChanged(*this, Windows::UI::Xaml::Data::PropertyChangedEventArgs{ fieldname });
    }

    /**
     * @brief Support member function for the INotifyPropertyChanged UWP interface. Stores the notification token.
     * @param token The notification token for the UI binding.
     * @returns The current notification token for the UI binding, after updating it.
     */
    winrt::event_token AStarViewModel::PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& token)
    {
        return propertyChanged.add(token);
    }

    /**
     * @brief Support member function for the INotifyPropertyChanged UWP interface. Stores the notification token.
     * @param token The notification token for the UI binding.
     */
    void AStarViewModel::PropertyChanged(winrt::event_token const& token)
    {
        propertyChanged.remove(token);
    }

    /**
    *  @brief Starts the background processing for the A* search with the currently loaded map.
    */
    void AStarViewModel::startSearch()
    {
        auto startPos = map.get_start();
        auto endPos = map.get_end();

        running = true;

        backTask = std::async(std::launch::async, [this, startPos, endPos]() {
            // Initialize the required data

            auto start = std::make_shared<Node>(startPos.first, startPos.second);
            auto end = std::make_shared<Node>(endPos.first, endPos.second);

            // now find the result
            auto res = solver.find(start, end);
            map.add_path(res);
            this->stopSearch();
            return res;
        });
    }

    /**
     *   @brief Stops the current search, if any.
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

    /**
     *  MapRender::draw_map Draws the real map.
     * @param painter
     */
    void AStarViewModel::draw_map(CanvasDrawingSession const& painter, Size size) const
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

                Rect rect(marginx + (col * dx), marginy + (row * dy), dx, dy);
                Color color = ColorHelper::FromArgb(255, r, g, b);
                painter.FillRectangle(rect, color);
            }
        }
    }
}
