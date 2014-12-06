#include "Renderer.h"
#include "Window.h"

namespace onut {
	Renderer::Renderer(Window& window) {
		createDevice(window);
		createRenderTarget();
	}

	void Renderer::createDevice(Window& window) {
		// Define our swap chain
		DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = window.getHandle();
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.Windowed = true;

		// Create the swap chain, device and device context
		auto result = D3D11CreateDeviceAndSwapChain(
			nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
			nullptr, 0, D3D11_SDK_VERSION,
			&swapChainDesc, &m_swapChain,
			&m_device, nullptr, &m_deviceContext);

		// Check for error
		if (result != S_OK) {
			MessageBox(nullptr, L"D3D11CreateDeviceAndSwapChain", L"Error", MB_OK);
			exit(0);
		}
	}

	void Renderer::createRenderTarget() {
		ID3D11Texture2D* backBuffer;
		auto result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
		if (result != S_OK) {
			MessageBox(nullptr, L"m_swapChain->GetBuffer", L"Error", MB_OK);
			exit(0);
		}
		result = m_device->CreateRenderTargetView(backBuffer, nullptr, &m_renderTargetView);
		if (result != S_OK) {
			MessageBox(nullptr, L"m_device->CreateRenderTargetView", L"Error", MB_OK);
			exit(0);
		}

		backBuffer->GetDesc(&m_backBufferDesc);
		backBuffer->Release();
	}

	void Renderer::beginFrame() {
		// Bind render target
		m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, nullptr);

		// Set viewport
		auto viewport = CD3D11_VIEWPORT(0.f, 0.f, (float)m_backBufferDesc.Width, (float)m_backBufferDesc.Height);
		m_deviceContext->RSSetViewports(1, &viewport);
	}

	void Renderer::endFrame() {
		// Swap the buffer!
		m_swapChain->Present(0, 0);
	}

	ID3D11Device* Renderer::getDevice() {
		return m_device;
	}

	ID3D11DeviceContext* Renderer::getDeviceContext() {
		return m_deviceContext;
	}

	void Renderer::clear(const Vector4& color) {
		m_deviceContext->ClearRenderTargetView(m_renderTargetView, &color.x);
	}
}
