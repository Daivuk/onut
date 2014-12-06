#pragma once
#include <d3d11.h>

#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

namespace onut {
	class Window;

	class Renderer {
	public:
		Renderer(Window& window);

		void					clear(const Vector4& color = { .25f, .5f, 1, 1 });

		void					beginFrame();
		void					endFrame();

		ID3D11Device*			getDevice();
		ID3D11DeviceContext*	getDeviceContext();

		POINT					getResolution();

	private:
		void					createDevice(Window& window);
		void					createRenderTarget();

		// Device stuff
		IDXGISwapChain*			m_swapChain = nullptr;
		ID3D11Device*			m_device = nullptr;
		ID3D11DeviceContext*	m_deviceContext = nullptr;

		// Render target
		ID3D11RenderTargetView* m_renderTargetView = nullptr;
		D3D11_TEXTURE2D_DESC	m_backBufferDesc;
	};
}
