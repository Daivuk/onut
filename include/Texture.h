#pragma once
#include <d3d11.h>
#include <memory>

namespace onut {
	class Texture {
	public:
		static std::shared_ptr<Texture> createDynamic(const POINT& size);
		static std::shared_ptr<Texture> createFromFile(const char* in_filename, bool in_generateMipmaps = false);
		static std::shared_ptr<Texture> createFromData(const POINT& size, const unsigned char* in_pData, bool in_generateMipmaps = false);

		Texture() {}
		virtual ~Texture();

		void							bind();
		ID3D11ShaderResourceView*		getResource() const { return m_pTextureView; }
		POINT							getSize() const { return m_size; }

	private:
		ID3D11Texture2D*			m_pTexture = nullptr;
		ID3D11ShaderResourceView*	m_pTextureView = nullptr;
		POINT						m_size;
	};
}
