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
