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

#include "AStarViewModel.g.h"

#include <future>
#include <string>
#include <iosfwd>

#include "SpriteSheet.h"
#include "Map.h"
#include "AStarSolver.h"

namespace winrt::AStarDemo::implementation
{
    struct AStarViewModel : AStarViewModelT<AStarViewModel>
    {
        AStarViewModel();

        bool GoButtonEnabled();
        void GoButtonEnabled(bool value);

        winrt::Windows::Foundation::IAsyncAction LoadFile(::winrt::Windows::Storage::StorageFile const& file);
        void Draw(::winrt::Microsoft::Graphics::Canvas::CanvasDrawingSession const& renderTarget, ::winrt::Windows::Foundation::Size const& size);
        void  ClearMap_Click(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);
        void  Search_Click(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);
        void  Stop_Click(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);
        void MapImage_DoubleTapped(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::Input::DoubleTappedRoutedEventArgs const& args);

        winrt::event_token PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& value);
        void PropertyChanged(winrt::event_token const& token);

        void NotifyPropertyChanged(winrt::hstring const& fieldname);

        winrt::Windows::Foundation::IAsyncAction LoadImages(::winrt::Microsoft::Graphics::Canvas::CanvasDevice const& device);

    private:
        bool goButtonEnabled;

        template<typename T>
        void ChangeFieldValue(T& field, T value, winrt::hstring const& fieldname);
        winrt::event<Windows::UI::Xaml::Data::PropertyChangedEventHandler> propertyChanged;

        Map map;
        AStarSolver solver;
        int marginx, marginy;
        int dx, dy;
        bool running;

        void drawMap(::winrt::Microsoft::Graphics::Canvas::CanvasDrawingSession const& painter) const;

        void startSearch();
        void stopSearch();
        bool loadMap(std::wistream& fd);

        std::unique_ptr<SpriteSheet> tiles;

        // Handle for the A* background processing.
        std::future<AStarSolver::NodePtr> backTask;
    };
}

namespace winrt::AStarDemo::factory_implementation
{
    struct AStarViewModel : AStarViewModelT<AStarViewModel, implementation::AStarViewModel>
    {
    };
}

#endif /* ASTARVIEWMODEL_H */