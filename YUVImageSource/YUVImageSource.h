#pragma once

namespace YUVImageSource
{
    public ref class ImageSource sealed
    {
    public:

		ImageSource();

		Platform::Boolean createTexture(const Platform::Array<byte>^ bytes, int width, int height);

		Platform::Boolean createImageSource();

		void process();

    };
}
