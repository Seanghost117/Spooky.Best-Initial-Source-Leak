#include "D3DRenderer.hpp"
#include "Game.hpp"
#include "UI/UIManager.hpp"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Mystic
{
	D3DRenderer::D3DRenderer():
		m_Swapchain(g_GameVariables->m_Swapchain)
	{
		if (SUCCEEDED(m_Swapchain->GetDevice(__uuidof(ID3D11Device), (void**)(&m_Device))))
		{
			m_Device.Attach(static_cast<ID3D11Device*>(m_Device.Get()));
		}
		else {
			g_Logger->Error("Failed to get D3D Device");
		}
		m_Device->GetImmediateContext(&m_Context);

		ImGui::CreateContext();
		ImGui_ImplDX11_Init(m_Device.Get(), m_Context);
		ImGui_ImplWin32_Init(g_GameVariables->m_GameWindow);

		auto&& style = ImGui::GetStyle();
		style.Alpha = 0.95f;                               // Global alpha applies to everything in ImGui
		style.WindowPadding = ImVec2(10, 10);              // Padding within a window
		style.WindowMinSize = ImVec2(100, 100);            // Minimum window size
		style.WindowRounding = 0.0f;                       // Radius of window corners rounding. Set to 0.0f to have rectangular windows
		style.WindowTitleAlign = ImVec2(0.0f, 0.5f);       // Alignment for title bar text
		style.FramePadding = ImVec2(5, 5);                 // Padding within a framed rectangle (used by most widgets)
		style.FrameRounding = 0.0f;                        // Radius of frame corners rounding. Set to 0.0f to have rectangular frames (used by most widgets).
		style.ItemSpacing = ImVec2(5, 5);                  // Horizontal and vertical spacing between widgets/lines
		style.ItemInnerSpacing = ImVec2(4, 4);             // Horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label)
		style.TouchExtraPadding = ImVec2(0, 0);            // Expand reactive bounding box for touch-based system where touch position is not accurate enough. Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. So don't grow this too much!
		style.IndentSpacing = 21.0f;                       // Horizontal spacing when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2).
		style.ColumnsMinSpacing = 6.0f;                    // Minimum horizontal spacing between two columns
		style.ScrollbarSize = 16.0f;                       // Width of the vertical scrollbar, Height of the horizontal scrollbar
		style.ScrollbarRounding = 9.0f;                    // Radius of grab corners rounding for scrollbar
		style.GrabMinSize = 10.0f;                         // Minimum width/height of a grab box for slider/scrollbar
		style.GrabRounding = 0.0f;                         // Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);        // Alignment of button text when button is larger than text.
		style.DisplayWindowPadding = ImVec2(22, 22);       // Window positions are clamped to be is_visible within the display area by at least this amount. Only covers regular windows.
		style.DisplaySafeAreaPadding = ImVec2(4, 4);       // If you cannot see the edge of your screen (e.g. on a TV) increase the safe area padding. Covers popups/tooltips as well regular windows.
		style.AntiAliasedLines = true;                     // Enable anti-aliasing on lines/borders. Disable if you are really short on CPU/GPU.
		style.CurveTessellationTol = 1.25f;                // Tessellation tolerance. Decrease for highly tessellated curves (higher quality, more polygons), increase to reduce qual
		style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.00f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.90f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.0f, 0.3f, 0.70f, 0);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.40f, 0.40f, 0.55f, 0.80f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.40f, 0.40f, 0.80f, 0.30f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.80f, 0.40f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.80f, 0.50f, 0.50f, 0.40f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 0.60f, 0.90f, 0.50f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.40f, 0.00f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.70f, 0.20f, 0.00f, 0.83f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.40f, 0.40f, 0.90f, 0.45f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.45f, 0.45f, 0.90f, 0.80f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.53f, 0.53f, 0.87f, 0.80f);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.70f, 0.60f, 0.60f, 1.00f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.90f, 0.70f, 0.70f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
	}

	D3DRenderer::~D3DRenderer() noexcept
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	void D3DRenderer::BeginFrame()
	{
		if (g_UiManager->m_Opened)
		{
			ImGui::GetIO().MouseDrawCursor = true;
			ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
		}
		else
		{
			ImGui::GetIO().MouseDrawCursor = false;
			ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void D3DRenderer::EndFrame()
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	void D3DRenderer::PreResize()
	{
		ImGui_ImplDX11_InvalidateDeviceObjects();
	}

	void D3DRenderer::PostResize()
	{
		ImGui_ImplDX11_CreateDeviceObjects();
	}

	void D3DRenderer::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (g_UiManager->m_Opened && msg == WM_KEYUP && wParam == 'X')
			g_Settings.m_LockMouse = !g_Settings.m_LockMouse;

		if (ImGui::GetCurrentContext())
			ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
	}
}
