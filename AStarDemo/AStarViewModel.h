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

#include "Map.h"
#include "AStarSolver.h"

// forward declarations
class Render;

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

        void NotifyPropertyChanged(winrt::hstring const& field);

    private:
        bool goButtonEnabled;

        winrt::event<Windows::UI::Xaml::Data::PropertyChangedEventHandler> propertyChanged;

        Map map;
        AStarSolver solver;
        bool show_gridlines;
        int marginx, marginy;
        int dx, dy;
        bool running;

        void draw_grid(Render& painter) const;
        void draw_map(Render& painter) const;
        void draw_nodes(Render& painter) const;

        void startSearch();
        void stopSearch();
        void startSearch(int x0, int y0, int x1, int y1);
        bool loadMap(const std::string& pathname);
        bool loadMap(std::wistream& fd);

        void clearMap();

        bool is_running() const;

        void paint(Render& painter);
        void mouseDoubleClickEvent(float x, float y);

        void enable_gridlines();
        void disable_gridlines();


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