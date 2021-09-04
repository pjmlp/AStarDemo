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
using namespace winrt::Windows::Foundation::Numerics;
using namespace winrt::Windows::Storage;
using namespace winrt::Microsoft::Graphics::Canvas;
using namespace winrt::Windows::UI;
using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::UI::Xaml;
using namespace winrt::Windows::UI::Xaml::Data;
using namespace winrt::Windows::UI::Xaml::Input;
using namespace winrt::Windows::System;


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
    void AStarViewModel::ChangeFieldValue(T& field, T value, const winrt::hstring& fieldname)
    {
        field = value;
        NotifyPropertyChanged(fieldname);
    }

    AStarViewModel::AStarViewModel(): goButtonEnabled(false), mouseActive(false), map(), solver(map), running(false), dx(0), dy(0), marginx(0), marginy(0), tiles(nullptr), startMapX(0), startMapY(0)
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
    IAsyncAction AStarViewModel::LoadFile(const StorageFile& file)
    {
        winrt::hstring data = co_await FileIO::ReadTextAsync(file);
        std::wstring str(data.data());
        std::wistringstream buffer(str);

        if (LoadMap(buffer))
        {
            ChangeFieldValue(goButtonEnabled, true, L"GoButtonEnabled");
        }
    }

    /**
     * @brief Draws the current state into the drawing surface, with the provided size.
     */
    void AStarViewModel::Draw(const CanvasDrawingSession& renderTarget, const Size& size)
    {
        if (renderTarget != nullptr) {
            if (map.columns() > 0 && map.rows() > 0) {
                dx = static_cast<int>(size.Width / map.columns());
                marginx = (static_cast<int>(size.Width) % map.columns()) / 2;
                dy = static_cast<int>(size.Height / map.rows());
                marginy = (static_cast<int>(size.Height) % map.rows()) / 2;

                // Win2D
                tilesPerRow = static_cast<int>(size.Width / map.tilesWidth());
                tilesPerHeight = static_cast<int>(size.Width / map.tilesHeigth());

                // render the background
                renderTarget.Clear(Colors::White());

                DrawMap(renderTarget);
            }
        }
    }

    /**
     *  @brief stops the current search if any and clears the loaded map.
     */
    void  AStarViewModel::ClearMap_Click(const IInspectable&, const RoutedEventArgs&)
    {
        StopSearch();
        map.clear();
    }

    /**
     *  @brief Triggers a new search, if none is currently running.
     */
    void  AStarViewModel::Search_Click(const IInspectable&, const RoutedEventArgs&)
    {
        ChangeFieldValue(goButtonEnabled, false, L"GoButtonEnabled");

        StartSearch();
    }

    /**
     *  @brief Stops the current search, if one is taking place.
     */
    void  AStarViewModel::Stop_Click(const IInspectable&, const RoutedEventArgs&)
    {
        if (running) {
            StopSearch();
            ChangeFieldValue(goButtonEnabled, true, L"GoButtonEnabled");
        }
    }

    /**
     *  @brief Handles double click action.
     */
    void AStarViewModel::MapImage_DoubleTapped(const IInspectable&, const DoubleTappedRoutedEventArgs& args)
    {
        auto point = args.GetPosition(nullptr);
        auto col = static_cast<int>((point.X - marginx) / map.tilesWidth());
        auto row = static_cast<int>((point.Y - marginy) / map.tilesHeigth());

        auto startPos = map.get_start();
        auto endPos = map.get_end();

        if (startPos.first == -1 && startPos.second == -1) {
            map.set_pos(row + startMapY, col + startMapY, Map::CellType::START);
        }
        else if (endPos.first == -1 && endPos.second == -1) {
            map.set_pos(row + startMapY, col + startMapY, Map::CellType::END);
        }
    }

    /**
     *  @brief Handles the mouse being pressed down event.
     */
    void AStarViewModel::MapImage_PointerPressed(const IInspectable&, const PointerRoutedEventArgs&)
    {
        mouseActive = true;
    }
    
    /**
     *  @brief Handles the mouse being moved event.
     */
    void AStarViewModel::MapImage_PointerMoved(const IInspectable& sender, const PointerRoutedEventArgs& args)
    {
        static winrt::Windows::Foundation::Point lastPoint{};

        if (mouseActive) {
            winrt::Windows::UI::Xaml::UIElement elem = sender.as<winrt::Windows::UI::Xaml::UIElement>();
            winrt::Windows::Foundation::Point pos = args.GetCurrentPoint(elem).Position();

            float deltaX = lastPoint.X - pos.X;
            float deltaY = lastPoint.Y - pos.Y;

            startMapX = startMapX + deltaX / map.tilesWidth();
            startMapY = startMapY + deltaY / map.tilesHeigth();


            lastPoint = pos;
        }
    }
    
    /**
     *  @brief Handles the mouse being released event.
     */
    void AStarViewModel::MapImage_PointerReleased(const IInspectable&, const PointerRoutedEventArgs&)
    {
        mouseActive = false;
    }

    /**
     *  @brief Handles the mouse being pressed down event.
     */
    void AStarViewModel::MapImage_KeyDown(const IInspectable&, const KeyEventArgs& e)
    {
        switch (e.VirtualKey()) {
        case VirtualKey::Up:
            if (startMapY > 0) {
                startMapY--;
            }
            break;
        case VirtualKey::Down:
            if (startMapY < tilesPerHeight && startMapY  < map.rows() - 1) {
                startMapY++;
            }
            break;
        case VirtualKey::Left:
            if (startMapX > 0) {
                startMapX--;
            }
            break;
        case VirtualKey::Right:
            if (startMapX < tilesPerRow && startMapX < map.columns() - 1) {
                startMapX++;
            }
            break;
        }
    }

    /**
     * @brief Support member function for the INotifyPropertyChanged UWP interface.
     * @param fieldname The name of field to notify to the bindings
     */
    void AStarViewModel::NotifyPropertyChanged(const winrt::hstring& fieldname)
    {
        propertyChanged(*this, Windows::UI::Xaml::Data::PropertyChangedEventArgs{ fieldname });
    }

    /**
     * @brief Support member function for the INotifyPropertyChanged UWP interface. Stores the notification token.
     * @param token The notification token for the UI binding.
     * @returns The current notification token for the UI binding, after updating it.
     */
    winrt::event_token AStarViewModel::PropertyChanged(const PropertyChangedEventHandler& token)
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
    void AStarViewModel::StartSearch()
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
            map.add_path(res.get());
            this->StopSearch();
            return res;
        });
    }

    /**
     *   @brief Stops the current search, if any.
     */
    void AStarViewModel::StopSearch()
    {
        running = false;
    }


    /**
     *   @brief loads a new map into the application.
     */
    bool AStarViewModel::LoadMap(std::wistream& fd)
    {
        LogInfo("loading map");
        // Just in case another search is ongoing
        if (backTask.valid()) {
            LogInfo("task sleeping");
            backTask.wait();
        }

        LogInfo("Loading file stream");
         
        return map.load(fd);
    }

    /**
     *  @brief Loads the images required for the level being drawn.
     * @param device the Win2D to draw into.
     */
    IAsyncAction AStarViewModel::LoadImages(const CanvasDevice& device)
    {
        auto ptr = co_await SpriteSheet::LoadAsync(device, L"Assets/Tiles.png", float2(32, 32), float2::zero());
        if (ptr != nullptr) {
            tiles = std::unique_ptr<SpriteSheet>(ptr);
        }


        co_return;
    }

    /**
     * @brief MapRender::draw_map Draws the real map.
     * @param device the Win2D to draw into.
     */
    void AStarViewModel::DrawMap(const CanvasDrawingSession& painter) const
    {
        if (tiles != nullptr) {
            CanvasSpriteBatch sprites = painter.CreateSpriteBatch();

            for (int row = 0; row < tilesPerHeight && row + startMapY < map.rows(); ++row) {
                for (int col = 0; col < tilesPerRow && col + startMapX < map.columns(); ++col) {
                    int spriteId = MapToSpriteId(map.at(row + startMapY, col + startMapX));

                    float2 dest{
                        static_cast<float>(col * map.tilesWidth()),
                        static_cast<float>(row * map.tilesHeigth())
                    };
                    tiles->Draw(sprites, spriteId, dest, float4::one());
                }
            }
        }
    }

    /**
     * @brief Converts the enumeration numeric value into the appropriate sprite index from the
     * current sprite sheet.
     * 
     * @param cell The cell value of the map position
     * @return The corresponding sprint index value
     */
    int AStarViewModel::MapToSpriteId(Map::CellType cell) const {
        switch (cell) {
        case Map::CellType::FREE:
            return 0;

        case Map::CellType::BLOCKED:
            return 1;
            break;

        case Map::CellType::VISITED:
            return 7;
            break;

        case Map::CellType::NODE_PATH:
            return 6;
            break;

        case Map::CellType::START:
            return 5;
            break;

        case Map::CellType::END:
            return 4;
            break;

        default:
            throw new std::logic_error("Missing mapping");
        }
        
    }
}
