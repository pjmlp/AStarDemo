#include "pch.h"
#include "SpriteSheet.h"

using namespace ::winrt::Windows::Foundation;
using namespace ::winrt::Windows::Foundation::Numerics;
using namespace ::winrt::Microsoft::Graphics::Canvas;


SpriteSheet::SpriteSheet(const canvas::CanvasBitmap& bitmap, float2 spriteSize, float2 origin):bitmap(bitmap), spriteSize(spriteSize), origin(origin)
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