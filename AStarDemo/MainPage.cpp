#include "pch.h"
#include "MainPage.h"
#include "MainPage.g.cpp"

#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Storage.Pickers.h>
#include <winrt/Windows.UI.Xaml.Input.h>

#include "AStarViewModel.h"

using namespace winrt;
using namespace winrt::Microsoft::Graphics::Canvas;
using namespace winrt::Microsoft::Graphics::Canvas::UI;
using namespace winrt::Microsoft::Graphics::Canvas::UI::Xaml;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::UI::Xaml;




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
        MapCanvas().PointerPressed({ &modelView, &AStarDemo::AStarViewModel::MapImage_PointerPressed });
        MapCanvas().PointerReleased({ &modelView, &AStarDemo::AStarViewModel::MapImage_PointerReleased });
        MapCanvas().PointerMoved({ &modelView, &AStarDemo::AStarViewModel::MapImage_PointerMoved });

        Window::Current().CoreWindow().KeyDown({ &modelView, &AStarDemo::AStarViewModel::MapImage_KeyDown });
    }

    AStarDemo::AStarViewModel MainPage::MainViewModel()
    {
        return modelView;
    }

    /**
     * Presents a file chooser whent he user clicks the respecite application button.
     */
    winrt::fire_and_forget MainPage::OpenMap_Click(const IInspectable& sender, const RoutedEventArgs& args)
    {
        // to reduce typing
        using namespace winrt::Windows::Storage;
        using namespace winrt::Windows::Storage::Pickers;

        FileOpenPicker openPicker;
        openPicker.SuggestedStartLocation(PickerLocationId::DocumentsLibrary);

        openPicker.FileTypeFilter().Clear();
        openPicker.FileTypeFilter().Append(L".txt");

        StorageFile file = co_await openPicker.PickSingleFileAsync();
        if (file != nullptr && file.IsAvailable()) {
            modelView.LoadFile(file);
        }
    }

    void MainPage::MapCanvas_Draw(const ICanvasAnimatedControl& sender, const CanvasAnimatedDrawEventArgs& args)
    {
        if (modelView != nullptr) {
            modelView.Draw(args.DrawingSession(), sender.Size());
        }

    }

    void MainPage::MapCanvas_CreateResources(const ICanvasAnimatedControl& sender, const CanvasCreateResourcesEventArgs& args)
    {
        bool spriteBatchSupported = CanvasSpriteBatch::IsSupported(sender.Device());
        if (!spriteBatchSupported) {
            return;
        }

        args.TrackAsyncAction(LoadImages(sender.Device()));
    }

    IAsyncAction MainPage::LoadImages(const CanvasDevice& device)
    {
        if (modelView != nullptr) {
            co_await modelView.LoadImages(device);
        }

        co_return;
    }


    void MainPage::Page_Unloaded(const IInspectable&, const RoutedEventArgs&)
    {
        // ensure that the control is properly cleaned
        MapCanvas().RemoveFromVisualTree();
        MapCanvas(nullptr);
    }
}