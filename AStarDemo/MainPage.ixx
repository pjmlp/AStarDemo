#include "pch.h"
#include "MainPage.h"
#include "MainPage.g.cpp"

#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Storage.Pickers.h>
#include <winrt/Windows.UI.Xaml.Input.h>

#include "AStarViewModel.h"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;

namespace winrt::AStarDemo::implementation
{
    MainPage::MainPage()
    {
        modelView = winrt::make<AStarDemo::implementation::AStarViewModel>();
        InitializeComponent();

        ClearBtn().Click({ &modelView, &AStarDemo::AStarViewModel::ClearMap_Click });
        GoBtn().Click({ &modelView, &AStarDemo::AStarViewModel::Search_Click });
        StopBtn().Click({ &modelView, &AStarDemo::AStarViewModel::Stop_Click });
        MapCanvas().DoubleTapped({ &modelView, &AStarDemo::AStarViewModel::MapImage_DoubleTapped });
    }

    AStarDemo::AStarViewModel MainPage::MainViewModel()
    {
        return modelView;
    }

    /**
     * Presents a file chooser whent he user clicks the respecite application button.
     */
    winrt::fire_and_forget MainPage::OpenMap_Click(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args)
    {
        // to reduce typing
        using namespace winrt::Windows::Storage;
        using namespace winrt::Windows::Storage::Pickers;

        FileOpenPicker openPicker;
        openPicker.SuggestedStartLocation(PickerLocationId::DocumentsLibrary);

        openPicker.FileTypeFilter().Clear();
        openPicker.FileTypeFilter().Append(L".txt");

        StorageFile file = co_await openPicker.PickSingleFileAsync();
        if (file.IsAvailable()) {
            modelView.LoadFile(file);
        }
    }

    void MainPage::MapCanvas_Draw(::winrt::Microsoft::Graphics::Canvas::UI::Xaml::ICanvasAnimatedControl const& sender, ::winrt::Microsoft::Graphics::Canvas::UI::Xaml::CanvasAnimatedDrawEventArgs const& args)
    {
        if (modelView != nullptr) {
            modelView.Draw(args.DrawingSession(), sender.Size());
        }

    }

    void MainPage::MapCanvas_CreateResources(::winrt::Microsoft::Graphics::Canvas::UI::Xaml::ICanvasAnimatedControl const&, ::winrt::Microsoft::Graphics::Canvas::UI::CanvasCreateResourcesEventArgs const&)
    {
    }


    void MainPage::Page_Unloaded(IInspectable const&, RoutedEventArgs const&)
    {
        // ensure that the control is properly cleaned
        MapCanvas().RemoveFromVisualTree();
        MapCanvas(nullptr);
    }
}
