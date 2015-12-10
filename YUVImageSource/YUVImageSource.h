#pragma once

namespace YUVImageSource
{
    public ref class ImageSource sealed
    {
    public:

		ImageSource();

		Platform::Boolean createTexture(const Platform::Array<byte>^ bytes, int width, int height);

		Platform::Boolean createImageSource();

		Microsoft::Graphics::Canvas::CanvasDevice^ getCanvasDevice();

		Microsoft::Graphics::Canvas::CanvasVirtualBitmap^ getCanvasVirtualBitmap();

    };
}
