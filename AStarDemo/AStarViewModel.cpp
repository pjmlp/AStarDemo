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
#include "MapWidget.h"
#include "WindowsRender.h"

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
    AStarViewModel::AStarViewModel(): goButtonEnabled(false)
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

        if (widget.loadMap(buffer))
        {
            goButtonEnabled = true;
            NotifyPropertyChanged(L"GoButtonEnabled");
        }
    }

    void AStarViewModel::Draw(::winrt::Microsoft::Graphics::Canvas::CanvasDrawingSession const& renderTarget, ::winrt::Windows::Foundation::Size const& size)
    {
        if (renderTarget != nullptr) {
            WindowsRender render(renderTarget, size);
            widget.paint(render);
        }
    }

    void  AStarViewModel::ClearMap_Click(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args)
    {
        widget.clearMap();
    }

    void  AStarViewModel::Search_Click(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args)
    {
        goButtonEnabled = false;
        NotifyPropertyChanged(L"GoButtonEnabled");

        widget.startSearch();
    }

    void  AStarViewModel::Stop_Click(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args)
    {
        if (widget.is_running()) {
            widget.stopSearch();
            goButtonEnabled = true;
            NotifyPropertyChanged(L"GoButtonEnabled");
        }
    }

    void AStarViewModel::MapImage_DoubleTapped(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::Input::DoubleTappedRoutedEventArgs const& args)
    {
       auto point = args.GetPosition(nullptr);
        widget.mouseDoubleClickEvent(point.X, point.Y);
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
}
