// Based on the C# sample available at
// https://github.com/microsoft/Win2D/blob/master/samples/ExampleGallery/SpriteSheets/SpriteSheet.cs

#ifndef TILESSHEET_H
#define TILESSHEET_H

#pragma once

#include <winrt/Microsoft.Graphics.Canvas.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Numerics.h>

#include <pplawait.h>

// simplify namespace accesses, while avoiding to pollute global namespace
namespace numeric = winrt::Windows::Foundation::Numerics;
namespace canvas = winrt::Microsoft::Graphics::Canvas;

/**
 * @brief Handles the rendering of tiles and sprites loaded from a sprite sheet  
 */
class SpriteSheet final
{
public:
    SpriteSheet(const canvas::CanvasBitmap& bitmap, numeric::float2 spriteSize, numeric::float2 origin);
    ~SpriteSheet() = default;

    numeric::float2 SpriteSize() { return spriteSize; }

    void Draw(const canvas::CanvasSpriteBatch& spriteBatch, int sprite, numeric::float2 position, numeric::float4 tint);

    ::winrt::Windows::Foundation::Rect GetSourceRect(int sprite);

    static concurrency::task<SpriteSheet*> LoadAsync(canvas::CanvasDevice device, const winrt::hstring& filename, numeric::float2 spriteSize, numeric::float2 origin);

private:
    canvas::CanvasBitmap bitmap;
    int spritesPerRow;
    numeric::float2 origin;
    numeric::float2 spriteSize;
};

#endif /* TILESSHEET_H */