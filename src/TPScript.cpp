#include "Game.hpp"
#include "TPScript.h"
#include "UI/UIManager.hpp"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "TeleportManager.h"

namespace Mystic
{
	bool TPScript::IsInitialized()
	{
		return m_Initialized;
	}

	ScriptType TPScript::GetType()
	{
		return ScriptType::D3D;
	}

	void TPScript::Initialize()
	{
		m_Initialized = true;
	}

	void TPScript::Destroy()
	{
	}

	void TPScript::Tick()
	{
		if (g_UiManager->m_Opened && g_Settings.TPWindowActivated)
		{
			ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
			if (ImGui::Begin("Teleport Manager", false, (ImGuiWindowFlags_AlwaysAutoResize)))
			{
				static char str0[15] = "";
				ImGui::Text("15 Chars Max");
				ImGui::InputText("Teleport Name", str0, IM_ARRAYSIZE(str0));
				ImGui::Separator();
				if (ImGui::Button("Save"))
				{
					g_Settings.TPWindowActivated = false;
					g_Logger->Info(str0);
					g_TeleportManager->AddTeleport(str0, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::GET_PLAYER_INDEX()), true));

				}
				ImGui::SameLine();
				if (ImGui::Button("Cancel"))
				{
					g_Settings.TPWindowActivated = false;
				}
			}
			ImGui::End();
		}
	}
}
