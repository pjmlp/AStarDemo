/* MainPage.cpp - Application main window.
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

    // helper variables to keep track of the loading process.
    static bool loadedLevel = false;
    static bool isLoading = false;

    void MainPage::MapCanvas_Draw(const ICanvasAnimatedControl& sender, const CanvasAnimatedDrawEventArgs& args)
    {
        if (modelView != nullptr && modelView.LoadedMap()) {
            if (loadedLevel) {
                modelView.Draw(args.DrawingSession(), sender.Size());
            }
            else if (!isLoading) {
                isLoading = true;
                LoadImages(MapCanvas().Device());
            }
        }

    }

    void MainPage::MapCanvas_CreateResources(const ICanvasAnimatedControl& sender, const CanvasCreateResourcesEventArgs& args)
    {
        // Nothing to do here, resources are created when the level is loaded.
    }

    IAsyncAction MainPage::LoadImages(const CanvasDevice& device)
    {
        bool spriteBatchSupported = CanvasSpriteBatch::IsSupported(device);
        if (!spriteBatchSupported) {
             co_return;
        }

        if (modelView != nullptr) {
            co_await modelView.LoadImages(device);

            loadedLevel = true;
            isLoading = false;
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