/* MainPage.h - Application main window.
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

#ifndef MAINPAGE_H
#define MAINPAGE_H


#pragma once

#include "MainPage.g.h"

namespace winrt::AStarDemo::implementation
{
    struct MainPage : MainPageT<MainPage>
    {
        MainPage();

        AStarDemo::AStarViewModel MainViewModel();

        winrt::fire_and_forget OpenMap_Click(const winrt::Windows::Foundation::IInspectable& sender, const winrt::Windows::UI::Xaml::RoutedEventArgs& args);
        void Page_Unloaded(const winrt::Windows::Foundation::IInspectable& sender, const winrt::Windows::UI::Xaml::RoutedEventArgs& e);
        void MapCanvas_CreateResources(const winrt::Microsoft::Graphics::Canvas::UI::Xaml::ICanvasAnimatedControl& sender, const winrt::Microsoft::Graphics::Canvas::UI::CanvasCreateResourcesEventArgs& args);
        void MapCanvas_Draw(const winrt::Microsoft::Graphics::Canvas::UI::Xaml::ICanvasAnimatedControl& sender, const winrt::Microsoft::Graphics::Canvas::UI::Xaml::CanvasAnimatedDrawEventArgs& args);

    private:
        AStarDemo::AStarViewModel modelView;

        winrt::Windows::Foundation::IAsyncAction LoadImages(const winrt::Microsoft::Graphics::Canvas::CanvasDevice& device);
    };
}

namespace winrt::AStarDemo::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}

#endif /* MAINPAGE_H */