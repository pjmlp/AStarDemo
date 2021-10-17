/* AStarViewModel.h - ViewModel for the common UI code
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

#ifndef ASTARVIEWMODEL_H
#define ASTARVIEWMODEL_H

#pragma once

import AStarLib;

#include "AStarViewModel.g.h"

#include <future>
#include <string>
#include <iosfwd>

#include "SpriteSheet.h"

namespace winrt::AStarDemo::implementation
{
    struct AStarViewModel : AStarViewModelT<AStarViewModel>
    {
        AStarViewModel();

        bool GoButtonEnabled();
        void GoButtonEnabled(bool value);

        winrt::Windows::Foundation::IAsyncAction LoadFile(const winrt::Windows::Storage::StorageFile& file);
        void Draw(const winrt::Microsoft::Graphics::Canvas::CanvasDrawingSession& renderTarget, const winrt::Windows::Foundation::Size& size);
        void  ClearMap_Click(const winrt::Windows::Foundation::IInspectable& sender, const winrt::Windows::UI::Xaml::RoutedEventArgs& args);
        void  Search_Click(const winrt::Windows::Foundation::IInspectable& sender, const winrt::Windows::UI::Xaml::RoutedEventArgs& args);
        void  Stop_Click(const winrt::Windows::Foundation::IInspectable& sender, const winrt::Windows::UI::Xaml::RoutedEventArgs& args);
        void MapImage_DoubleTapped(const winrt::Windows::Foundation::IInspectable& sender, const winrt::Windows::UI::Xaml::Input::DoubleTappedRoutedEventArgs& args);
        void MapImage_PointerPressed(const winrt::Windows::Foundation::IInspectable& sender, const winrt::Windows::UI::Xaml::Input::PointerRoutedEventArgs& args);
        void MapImage_PointerMoved(const winrt::Windows::Foundation::IInspectable& sender, const winrt::Windows::UI::Xaml::Input::PointerRoutedEventArgs& args);
        void MapImage_PointerReleased(const winrt::Windows::Foundation::IInspectable& sender, const winrt::Windows::UI::Xaml::Input::PointerRoutedEventArgs& args);
        void MapImage_KeyDown(const winrt::Windows::Foundation::IInspectable& sender, const winrt::Windows::UI::Core::KeyEventArgs& e);

        winrt::event_token PropertyChanged(const winrt::Windows::UI::Xaml::Data::PropertyChangedEventHandler& value);
 
        void PropertyChanged(const winrt::event_token& token);

        void NotifyPropertyChanged(const winrt::hstring& fieldname);

        winrt::Windows::Foundation::IAsyncAction LoadImages(const winrt::Microsoft::Graphics::Canvas::CanvasDevice& device);

    private:
        bool goButtonEnabled;
        bool mouseActive;

        template<typename T>
        void ChangeFieldValue(T& field, T value, winrt::hstring const& fieldname);
        winrt::event<winrt::Windows::UI::Xaml::Data::PropertyChangedEventHandler> propertyChanged;

        AStarLib::Map map;
        AStarLib::AStarSolver solver;
        int marginx, marginy;
        int dx, dy;
        bool running;
        int startMapX, startMapY;
        int tilesPerRow, tilesPerHeight;

        void DrawMap(const winrt::Microsoft::Graphics::Canvas::CanvasDrawingSession& painter) const;
        int MapToSpriteId(AStarLib::Map::CellType cell) const;

        void StartSearch();
        void StopSearch();
        bool LoadMap(std::wistream& fd);

        std::unique_ptr<SpriteSheet> tiles;

        // Handle for the A* background processing.
        std::future<AStarLib::AStarSolver::NodePtr> backTask;
    };
}

namespace winrt::AStarDemo::factory_implementation
{
    struct AStarViewModel : AStarViewModelT<AStarViewModel, implementation::AStarViewModel>
    {
    };
}

#endif /* ASTARVIEWMODEL_H */