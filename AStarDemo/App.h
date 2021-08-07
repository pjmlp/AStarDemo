#pragma once
#include "App.xaml.g.h"

namespace winrt::AStarDemo::implementation
{
    struct App : AppT<App>
    {
        App();

        void OnLaunched(const winrt::Windows::ApplicationModel::Activation::LaunchActivatedEventArgs&);
        void OnSuspending(const winrt::Windows::Foundation::IInspectable&, const winrt::Windows::ApplicationModel::SuspendingEventArgs&);
        void OnNavigationFailed(const winrt::Windows::Foundation::IInspectable&, const winrt::Windows::UI::Xaml::Navigation::NavigationFailedEventArgs&);
    };
}
