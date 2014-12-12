#include "LodePNG.h"
#include "onut.h"
#include "Texture.h"

#include <cassert>
#include <vector>

namespace onut {
	std::shared_ptr<Texture> Texture::createDynamic(const POINT& size) {
		ID3D11Texture2D* pTexture = NULL;
		ID3D11ShaderResourceView* pTextureView = NULL;
		auto pRet = std::make_shared<Texture>();

		D3D11_TEXTURE2D_DESC desc;
		desc.Width = size.x;
		desc.Height = size.y;
		desc.MipLevels = desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;

		auto pDevice = getRenderer()->getDevice();
		assert(pDevice->CreateTexture2D(&desc, NULL, &pTexture) == S_OK);
		assert(pDevice->CreateShaderResourceView(pTexture, NULL, &pTextureView) == S_OK);

		pRet->m_size = size;
		pRet->m_pTextureView = pTextureView;
		pRet->m_pTexture = pTexture;

		return pRet;
	}

	std::shared_ptr<Texture> Texture::createFromFile(const std::string& filename, bool generateMipmaps) {
		Texture* pRet = NULL;

		std::vector<unsigned char> image; //the raw pixels (holy crap that must be slow)
		unsigned int w, h;
		assert(!lodepng::decode(image, w, h, filename));
		POINT size{ w, h };
		byte* pData = &(image[0]);
		ULONG len = size.x * size.y;

		// Pre multiplied
		for (ULONG i = 0; i < len; ++i, pData += 4) {
			pData[0] = pData[0] * pData[3] / 255;
			pData[1] = pData[1] * pData[3] / 255;
			pData[2] = pData[2] * pData[3] / 255;
		}

		return createFromData(size, &(image[0]), generateMipmaps);
	}

	std::shared_ptr<Texture> Texture::createFromData(const POINT& size, const unsigned char* in_pData, bool in_generateMipmaps) {
		ID3D11Texture2D* pTexture = NULL;
		ID3D11ShaderResourceView* pTextureView = NULL;
		auto pRet = std::make_shared<Texture>();

		// Manually generate mip levels
		bool allowMipMaps = true;
		UINT w2 = 1;
		UINT h2 = 1;
		while (w2 < (UINT)size.x) w2 *= 2;
		if (size.x != w2) allowMipMaps = false;
		while (h2 < (UINT)size.y) h2 *= 2;
		if (size.y != h2) allowMipMaps = false;
		unsigned char* pMipMaps = NULL;
		int mipLevels = 1;
		D3D11_SUBRESOURCE_DATA* mipsData = NULL;
		allowMipMaps = allowMipMaps && in_generateMipmaps;
		if (allowMipMaps) {
			UINT biggest = max(w2, h2);
			UINT w2t = w2;
			UINT h2t = h2;
			UINT totalSize = w2t * h2t * 4;
			while (!(w2t == 1 && h2t == 1)) {
				++mipLevels;
				w2t /= 2;
				if (w2t < 1) w2t = 1;
				h2t /= 2;
				if (h2t < 1) h2t = 1;
				totalSize += w2t * h2t * 4;
			}
			pMipMaps = new byte[totalSize];
			memcpy(pMipMaps, in_pData, size.x * size.y * 4);

			mipsData = new D3D11_SUBRESOURCE_DATA[mipLevels];

			w2t = w2;
			h2t = h2;
			totalSize = 0;
			int mipTarget = mipLevels;
			mipLevels = 0;
			byte* prev;
			byte* cur;
			while (mipLevels != mipTarget) {
				prev = pMipMaps + totalSize;
				mipsData[mipLevels].pSysMem = prev;
				mipsData[mipLevels].SysMemPitch = w2t * 4;
				mipsData[mipLevels].SysMemSlicePitch = 0;
				totalSize += w2t * h2t * 4;
				cur = pMipMaps + totalSize;
				w2t /= 2;
				if (w2t < 1) w2t = 1;
				h2t /= 2;
				if (h2t < 1) h2t = 1;
				++mipLevels;
				if (mipLevels == mipTarget) break;
				int accum;

				// Generate the mips
				int multX = w2 / w2t;
				int multY = h2 / h2t;
				for (UINT y = 0; y < h2t; ++y) {
					for (UINT x = 0; x < w2t; ++x) {
						for (UINT k = 0; k < 4; ++k) {
							accum = 0;
							accum += prev[(y * multY * w2 + x * multX) * 4 + k];
							accum += prev[(y * multY * w2 + (x + multX / 2) * multX) * 4 + k];
							accum += prev[((y + multY / 2) * multY * w2 + x * multX) * 4 + k];
							accum += prev[((y + multY / 2) * multY * w2 + (x + multX / 2) * multX) * 4 + k];
							cur[(y * w2t + x) * 4 + k] = accum / 4;
						}
					}
				}

				w2 = w2t;
				h2 = h2t;
			}
		}

		D3D11_TEXTURE2D_DESC desc;
		desc.Width = size.x;
		desc.Height = size.y;
		desc.MipLevels = mipLevels;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = (pMipMaps) ? pMipMaps : in_pData;
		data.SysMemPitch = size.x * 4;
		data.SysMemSlicePitch = 0;

		auto pDevice = getRenderer()->getDevice();
		assert(pDevice->CreateTexture2D(&desc, (mipsData) ? mipsData : &data, &pTexture) == S_OK);
		assert(pDevice->CreateShaderResourceView(pTexture, NULL, &pTextureView) == S_OK);

		pTexture->Release();
		if (pMipMaps) delete[] pMipMaps;
		if (mipsData) delete[] mipsData;

		pRet->m_size = size;
		pRet->m_pTextureView = pTextureView;

		return pRet;
	}

	Texture::~Texture() {
		if (m_pTextureView) m_pTextureView->Release();
		m_pTextureView = nullptr;
		m_pTexture = nullptr;
	}

	void Texture::bind() {
	}
}
