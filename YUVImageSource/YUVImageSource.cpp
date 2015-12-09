#include "pch.h"
#include "YUVImageSource.h"

using namespace YUVImageSource;
using namespace Platform;

static ID3D11DeviceContext* d3d_device_context = nullptr;

static ID3D11Device* d3d_device = nullptr;

static D3D_FEATURE_LEVEL d3d_feature_level;

static ID3D11Texture2D* d3d_texture = nullptr;

static IDXGISurface* d3d_surface = nullptr;

static ID2D1Device* d2d_device = nullptr;

static ID2D1DeviceContext* d2d_device_context = nullptr;

static ID2D1ImageSource* d2d_image_source = nullptr;

bool createD3DDevice() 
{
	D3D_FEATURE_LEVEL d3d_feature_levels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	HRESULT result = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
		0,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		d3d_feature_levels,
		ARRAYSIZE(d3d_feature_levels),
		D3D11_SDK_VERSION,
		&d3d_device,
		&d3d_feature_level,
		&d3d_device_context
		);

	return (result == S_OK) ? true : false;
}

bool createD3DTexture(void* bytes, int width, int height)
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

	desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;

	desc.CPUAccessFlags = 0;

	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data;

	data.pSysMem = bytes;

	data.SysMemPitch = width;

	HRESULT result = d3d_device->CreateTexture2D(&desc, &data, &d3d_texture);

	return (result == S_OK) ? true : false;
}

bool createD2DDevice()
{
	IDXGIDevice* dxgiDevice = nullptr;

	HRESULT result = d3d_device->QueryInterface<IDXGIDevice>(&dxgiDevice);

	if (result == S_OK) 
	{
		D2D1_CREATION_PROPERTIES properties;

		properties.debugLevel = D2D1_DEBUG_LEVEL::D2D1_DEBUG_LEVEL_INFORMATION;

		properties.options = D2D1_DEVICE_CONTEXT_OPTIONS::D2D1_DEVICE_CONTEXT_OPTIONS_NONE;

		properties.threadingMode = D2D1_THREADING_MODE::D2D1_THREADING_MODE_SINGLE_THREADED;

		result = D2D1CreateDevice(dxgiDevice, properties, &d2d_device);

		if (result == S_OK)
		{
			D2D1_DEVICE_CONTEXT_OPTIONS options = D2D1_DEVICE_CONTEXT_OPTIONS::D2D1_DEVICE_CONTEXT_OPTIONS_NONE;

			result = d2d_device->CreateDeviceContext(options, &d2d_device_context);
		}
	}

	return (result == S_OK) ? true : false;
}

bool createImageSourceFromDxgi()
{
	ID2D1DeviceContext2* context = nullptr;

	HRESULT result = d2d_device_context->QueryInterface<ID2D1DeviceContext2>(&context);

	if (result == S_OK)
	{
		result = d3d_texture->QueryInterface<IDXGISurface>(&d3d_surface);

		if (result == S_OK)
		{
			result = context->CreateImageSourceFromDxgi(&d3d_surface, 1, DXGI_COLOR_SPACE_TYPE::DXGI_COLOR_SPACE_YCBCR_FULL_G22_NONE_P709_X601, D2D1_IMAGE_SOURCE_FROM_DXGI_OPTIONS::D2D1_IMAGE_SOURCE_FROM_DXGI_OPTIONS_LOW_QUALITY_PRIMARY_CONVERSION, &d2d_image_source);
		}
	}

	return (result == S_OK) ? true : false;
}

ImageSource::ImageSource()
{
	createD3DDevice();

	createD2DDevice();
}

Platform::Boolean ImageSource::createTexture(const Platform::Array<byte>^ bytes, int width, int height)
{
	return createD3DTexture(bytes->Data, width, height);
}

Platform::Boolean ImageSource::createImageSource()
{
	return createImageSourceFromDxgi();
}

// TODO interop ID2D1ImageSource to CanvasVirtualBitmap

void ImageSource::process()
{
	
}