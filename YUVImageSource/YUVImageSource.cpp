#include "pch.h"
#include "YUVImageSource.h"

using namespace YUVImageSource;
using namespace Platform;

static ID3D11DeviceContext* d3dcontext = nullptr;

static ID3D11Device* d3ddevice = nullptr;

static ID3D11Texture2D* texture = nullptr;

static IDXGISurface* surface = nullptr;

static ID2D1Device* d2ddevice = nullptr;

static ID2D1DeviceContext* d2dcontext = nullptr;

static D3D_FEATURE_LEVEL level[] = { 
	D3D_FEATURE_LEVEL_10_0,
	D3D_FEATURE_LEVEL_10_1,
	D3D_FEATURE_LEVEL_11_0,
	D3D_FEATURE_LEVEL_11_1,
};

ImageSource::ImageSource()
{

}

Platform::Boolean ImageSource::createDevice()
{
	HRESULT result = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&d3ddevice,
		level,
		&d3dcontext
		);

	return (result == S_OK) ? true : false;
}

Platform::Boolean ImageSource::createTexture2D(const Platform::Array<byte>^ bytes, int width, int height)
{
	D3D11_TEXTURE2D_DESC desc;

	desc.Width = width;

	desc.Height = height;

	desc.MipLevels = 1;

	desc.ArraySize = 1;

	desc.Format = DXGI_FORMAT::DXGI_FORMAT_NV12;

	desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;

	desc.SampleDesc.Count = 1;

	desc.SampleDesc.Quality = 0;

	desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET;

	desc.CPUAccessFlags = 0;

	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data;

	data.pSysMem = bytes->Data;

	data.SysMemPitch = width;

	HRESULT result = d3ddevice->CreateTexture2D(&desc, &data, &texture);

	return (result == S_OK) ? true : false;
}

Platform::Boolean ImageSource::getSurfaceFromTexture2D()
{
	HRESULT result = texture->QueryInterface<IDXGISurface>(&surface);

	return (result == S_OK) ? true : false;
}

Platform::Boolean ImageSource::getImageSourceFromSurface() 
{
	D2D1_CREATION_PROPERTIES properties;

	properties.debugLevel = D2D1_DEBUG_LEVEL::D2D1_DEBUG_LEVEL_NONE;

	properties.options = D2D1_DEVICE_CONTEXT_OPTIONS::D2D1_DEVICE_CONTEXT_OPTIONS_NONE;

	properties.threadingMode = D2D1_THREADING_MODE::D2D1_THREADING_MODE_SINGLE_THREADED;

	HRESULT result = D2D1CreateDeviceContext(surface, &properties, &d2dcontext);

	return (result == S_OK) ? true : false;
}