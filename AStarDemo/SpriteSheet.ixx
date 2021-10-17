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

/**
 * @brief Handles the rendering of tiles and sprites loaded from a sprite sheet
 */
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


SpriteSheet::SpriteSheet(const CanvasBitmap& bitmap, float2 spriteSize, float2 origin) :bitmap(bitmap), spriteSize(spriteSize), origin(origin)
{
    spritesPerRow = static_cast<int>(bitmap.Size().Width / spriteSize.x);
}

concurrency::task<SpriteSheet*> SpriteSheet::LoadAsync(CanvasDevice device, const winrt::hstring& filename, float2 spriteSize, float2 origin)
{
    CanvasBitmap img = co_await CanvasBitmap::LoadAsync(device, filename);

    co_return new SpriteSheet(img, spriteSize, origin);
}

void SpriteSheet::Draw(const CanvasSpriteBatch& spriteBatch, int sprite, float2 position, float4 tint)
{
    if (sprite < 0)
        return;

    spriteBatch.DrawFromSpriteSheet(bitmap, position, GetSourceRect(sprite), tint, origin, 0, float2::one(), CanvasSpriteFlip::None);
}

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