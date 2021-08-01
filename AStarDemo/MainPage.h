﻿#pragma once

#include "MainPage.g.h"

namespace winrt::AStarDemo::implementation
{
    struct MainPage : MainPageT<MainPage>
    {
        MainPage();

        AStarDemo::AStarViewModel MainViewModel();

        winrt::fire_and_forget OpenMap_Click(::winrt::Windows::Foundation::IInspectable const& sender, ::winrt::Windows::UI::Xaml::RoutedEventArgs const& args);
        void Page_Unloaded(IInspectable const& sender, ::winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void MapCanvas_CreateResources(::winrt::Microsoft::Graphics::Canvas::UI::Xaml::ICanvasAnimatedControl const& sender, ::winrt::Microsoft::Graphics::Canvas::UI::CanvasCreateResourcesEventArgs const& args);
        void MapCanvas_Draw(::winrt::Microsoft::Graphics::Canvas::UI::Xaml::ICanvasAnimatedControl const& sender, ::winrt::Microsoft::Graphics::Canvas::UI::Xaml::CanvasAnimatedDrawEventArgs const& args);

    private:
        AStarDemo::AStarViewModel modelView;

        ::winrt::Windows::Foundation::IAsyncAction LoadImages(const ::winrt::Microsoft::Graphics::Canvas::CanvasDevice& device);
    };
}

namespace winrt::AStarDemo::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}
