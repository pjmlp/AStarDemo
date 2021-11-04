// Based on the C# sample available at
// https://github.com/microsoft/Win2D/blob/master/samples/ExampleGallery/SpriteSheets/SpriteSheet.cs
module;

#include <winrt/Microsoft.Graphics.Canvas.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Numerics.h>

#include <pplawait.h>

export module SpriteSheet;

// simplify namespace accesses
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Foundation::Numerics;
using namespace winrt::Microsoft::Graphics::Canvas;

/// <summary>
/// Handles the rendering of tiles and sprites loaded from a sprite sheet
/// </summary>
export class SpriteSheet final
{
public:
    SpriteSheet(const CanvasBitmap& bitmap, float2 spriteSize, float2 origin);
    ~SpriteSheet() = default;

    float2 SpriteSize() { return spriteSize; }

    void Draw(const CanvasSpriteBatch& spriteBatch, int sprite, float2 position, float4 tint);

    ::winrt::Windows::Foundation::Rect GetSourceRect(int sprite);

    static concurrency::task<SpriteSheet*> LoadAsync(CanvasDevice device, const winrt::hstring& filename, float2 spriteSize, float2 origin);

private:
    CanvasBitmap bitmap;
    int spritesPerRow;
    float2 origin;
    float2 spriteSize;
};

/// <summary>
/// Constructs the sprite sheet using the given bitmap and sprite size.
/// </summary>
/// <param name="bitmap">The sprite sheet atlas</param>
/// <param name="spriteSize">The rectangular size</param>
/// <param name="origin">Where to start reading the sprite data</param>
SpriteSheet::SpriteSheet(const CanvasBitmap& bitmap, float2 spriteSize, float2 origin) :bitmap(bitmap), spriteSize(spriteSize), origin(origin)
{
    spritesPerRow = static_cast<int>(bitmap.Size().Width / spriteSize.x);
}

/// <summary>
/// Loads a specific image file asynchronously.
/// </summary>
/// <param name="device">The device owning the image</param>
/// <param name="filename">The file to read the image from</param>
/// <param name="spriteSize">The rectangular size</param>
/// <param name="origin"></param>
/// <returns>A new instance for the sprite sheet. Needs to be released by the caller</returns>
concurrency::task<SpriteSheet*> SpriteSheet::LoadAsync(CanvasDevice device, const winrt::hstring& filename, float2 spriteSize, float2 origin)
{
    CanvasBitmap img = co_await CanvasBitmap::LoadAsync(device, filename);

    //TODO: find out how to make it work with std::unique_ptr alongside concurrency::task
    co_return new SpriteSheet(img, spriteSize, origin);
}

/// <summary>
/// Draws the provided sprite id at the given position with a specific tint overlay.
/// </summary>
/// <param name="spriteBatch">Sprite batching target.</param>
/// <param name="sprite">The sprite index on the sprite sheet</param>
/// <param name="position">Where to draw on the target surface</param>
/// <param name="tint">The overlay tint</param>
void SpriteSheet::Draw(const CanvasSpriteBatch& spriteBatch, int sprite, float2 position, float4 tint)
{
    if (sprite < 0)
        return;

    spriteBatch.DrawFromSpriteSheet(bitmap, position, GetSourceRect(sprite), tint, origin, 0, float2::one(), CanvasSpriteFlip::None);
}

/// <summary>
/// Retrieves the rectangle area for the given sprite.
/// </summary>
/// <param name="sprite">The sprint index</param>
/// <returns>The rectangle area for the sprite data on the source image</returns>
Rect SpriteSheet::GetSourceRect(int sprite)
{
    int row = sprite / spritesPerRow;
    int column = sprite % spritesPerRow;

    return {
        static_cast<float>((spriteSize.x * column)),
        static_cast<float>((spriteSize.y * row)),
        static_cast<float>(spriteSize.x),
        static_cast<float>(spriteSize.y) };
}