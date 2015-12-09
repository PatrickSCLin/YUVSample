#pragma once

namespace YUVImageSource
{
    public ref class ImageSource sealed
    {
    public:

		ImageSource();

		Platform::Boolean createDevice();

		Platform::Boolean createTexture2D(const Platform::Array<byte>^ bytes, int width, int height);

		Platform::Boolean getSurfaceFromTexture2D();

		Platform::Boolean getImageSourceFromSurface();
    };
}
