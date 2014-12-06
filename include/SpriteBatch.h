#pragma once
#include "SimpleMath.h"
#include "Texture.h"

#include <memory>

namespace onut {
	using namespace DirectX::SimpleMath;

	class SpriteBatch {
	public:
		SpriteBatch();
		virtual ~SpriteBatch();

		void begin();
		void drawRect(std::shared_ptr<Texture> pTexture, const Vector4& rect, const Vector4 colors[4]);
		void drawRect(std::shared_ptr<Texture> pTexture, const Vector4& rect, const Vector4& color = Vector4::One);
		void drawRect(std::shared_ptr<Texture> pTexture, const Vector4& rect, const Vector4& uvs, const Vector4& color = Vector4::One);
		void end();

	private:
		struct SVertexP2T2C4 {
			Vector2 position;
			Vector2 texCoord;
			Vector4 color;
		};

		static const int MAX_SPRITE_COUNT = 300;

		void flush();

		ID3D11Buffer*				m_pVertexBuffer = nullptr;
		ID3D11Buffer*				m_pIndexBuffer = nullptr;
		D3D11_MAPPED_SUBRESOURCE	m_pMappedVertexBuffer;

		std::shared_ptr<Texture>	m_pTexWhite = nullptr;
		std::shared_ptr<Texture>	m_pTexture = nullptr;

		static const unsigned int	m_stride = sizeof(SVertexP2T2C4);
		static const unsigned int	m_offset = 0;
		unsigned int				m_spriteCount = 0;

		bool						m_isDrawing = false;
	};
}
