#pragma once
#include "Common.hpp"
#include <wrl/client.h>

namespace Mystic
{
	class D3DRenderer
	{
	public:
		explicit D3DRenderer();
		~D3DRenderer() noexcept;
		D3DRenderer(D3DRenderer const&) = delete;
		D3DRenderer(D3DRenderer&&) = delete;
		D3DRenderer& operator=(D3DRenderer const&) = delete;
		D3DRenderer& operator=(D3DRenderer&&) = delete;

		void BeginFrame();
		void EndFrame();

		void PreResize();
		void PostResize();

		void WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	private:
		IDXGISwapChain* m_Swapchain{};
		comptr<ID3D11Device> m_Device;
		ID3D11DeviceContext* m_Context{};
	};

	inline std::unique_ptr<D3DRenderer> g_D3DRenderer;
}
