#include "Common.hpp"

#ifdef Mystic_DEBUG
#  pragma comment(lib, "MinHook-Debug.lib")
#else
#  pragma comment(lib, "MinHook-Release.lib")
#endif

namespace Mystic
{
	namespace
	{
		std::uint32_t g_HookFrameCount{};
	}

	void Hooks::SetWarningMessage(char* entryHeader, char* entryLine1, int instructionalKey, char* entryLine2, BOOL p4, Any p5, Any* p6, Any* p7, BOOL background)
	{
		if (SCRIPT::GET_HASH_OF_THIS_SCRIPT_NAME() == MISC::GET_HASH_KEY("shop_controller"))
		{
			return;
		}
		return static_cast<decltype(&SetWarningMessage)>(g_Hooking->m_OriginalSetWarningMessage)(entryHeader, entryLine1, instructionalKey, entryLine2, p4, p5, p6, p7, background);
	}

	int Hooks::GetNumOfEvents(int eventgroup)
	{
		if (g_Running && g_HookFrameCount != *g_GameVariables->m_FrameCount)
		{
			g_HookFrameCount = *g_GameVariables->m_FrameCount;
			g_ScriptManager->OnGameTick();
		}
		return static_cast<decltype(&GetNumOfEvents)>(g_Hooking->m_OriginalGetNumOfEvents)(eventgroup);
	}

	const char* Hooks::GetLabelText(void* unk, const char* label)
	{
		if (g_Running)
			if (auto text = g_CustomText->GetText(joaat(label)))
				return text;

		return static_cast<decltype(&GetLabelText)>(g_Hooking->m_OriginalGetLabelText)(unk, label);
	}

#pragma warning (push)
#pragma warning (disable: 4244)


#pragma warning (pop)

	void Hooks::NetworkEventHandler(__int64 a1, CNetGamePlayer* a2, __int64 a3, unsigned __int16 a4, int a5, int a6, __int64 a7, __int64 a8)
	{
		auto ptr = Memory::pattern("66 83 FA ? 73 13 0F B7 C2 4C 89 84 C1 ? ? ? ? 4C 89 8C C1").count(1).get(0).get<char>(0);
		auto name_offset = *(std::int32_t*)(ptr + 21);
		auto eventName = reinterpret_cast<char*>(*reinterpret_cast<__int64*>(a1 + 8 * a4 + name_offset));
		
		g_Logger->Info("------------------");
		g_Logger->Info(eventName);
		g_Logger->Info(a2->playerInfo->PlayerName);
		g_Logger->Info("------------------");

		return static_cast<decltype(&NetworkEventHandler)>(g_Hooking->m_OriginalNetworkEventHandler)(a1, a2, a3, a4, a5, a6, a7, a8);
	}

	int Hooks::TriggerScriptEvent(bool unk0, uint64_t* args, int argCount, int bitFlags)
	{
		return static_cast<decltype(&TriggerScriptEvent)>(g_Hooking->m_OriginalTriggerScriptEvent)(unk0, args, argCount, bitFlags);
	}
	bool Hooks::ScriptEventHandler(std::int64_t NetEventStruct, std::int64_t CNetGamePlayer)
	{
		auto args = reinterpret_cast<std::int64_t*>(NetEventStruct + 0x70);
		auto SenderID = *reinterpret_cast<std::int8_t*>(CNetGamePlayer + 0x2D);
		const auto ScriptEventHash = args[0];

		if (SenderID < 32)
		{
			switch (ScriptEventHash)
			{
			case 713068249:
				if (g_MenuScript->f_antiTransError)
				{

					if (args[2] == -1695772783)
					{
						if (g_MenuScript->f_notifyEvents)
							Utils::notifyBlocked("Event: Transaction Error Event\nFrom: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID) + "\nStatus: Blocked");
						return false;
						if (g_MenuScript->f_redirectEvents)
						{
							SCRIPT::TRIGGER_SCRIPT_EVENT(1, args, sizeof(args), 1 << SenderID);
							Utils::notifyAboveMap("Redirecting Transaction Error Event \nTo: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID));
						}
					}

				}
				break;
			case 552065831:
				if (g_MenuScript->f_antiSoundspam)
				{

					if (g_MenuScript->f_notifyEvents)
						Utils::notifyBlocked("Event: Sound Spam Event\nFrom: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID) + "\nStatus: Blocked");
					return false;
					if (g_MenuScript->f_redirectEvents)
					{
						SCRIPT::TRIGGER_SCRIPT_EVENT(1, args, sizeof(args), 1 << SenderID);
						Utils::notifyAboveMap("Redirecting Sound Spam Event \nTo: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID));
					}

				}
				break;
			case -125347541:
				if (g_MenuScript->f_antiTPApt || g_MenuScript->f_antiInfLoading)
				{
					if (g_MenuScript->f_notifyEvents)
						Utils::notifyBlocked("Event: Teleport Event\nFrom: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID) + "\nStatus: Blocked");
					if (g_MenuScript->f_redirectEvents)
					{
						SCRIPT::TRIGGER_SCRIPT_EVENT(1, args, sizeof(args), 1 << SenderID);
						Utils::notifyAboveMap("Redirecting Teleport Event \nTo: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID));
					}
					return false;
				}
				break;
			case -615431083:
				if (g_MenuScript->f_antiotr)
				{

					if (g_MenuScript->f_notifyEvents)
						Utils::notifyBlocked("Event: Remote OTR Event\nFrom: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID) + "\nStatus: Blocked");

					return false;
					if (g_MenuScript->f_redirectEvents)
					{
						SCRIPT::TRIGGER_SCRIPT_EVENT(1, args, sizeof(args), 1 << SenderID);
						Utils::notifyAboveMap("Redirecting Remote OTR Event \nTo: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID));
					}
				}
				break;
			case -1049031463:
				if (g_MenuScript->f_antibanner)
				{

					if (g_MenuScript->f_notifyEvents)
						Utils::notifyBlocked("Event: Transaction/Banner Spam\nFrom: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID) + "\nStatus: Blocked");

					return false;
					if (g_MenuScript->f_redirectEvents)
					{
						SCRIPT::TRIGGER_SCRIPT_EVENT(1, args, sizeof(args), 1 << SenderID);
						Utils::notifyAboveMap("Redirecting Transaction/Banner Event \nTo: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID));
					}
				}
			case 1385453222:
				if (g_MenuScript->f_antibounty)
				{

					if (g_MenuScript->f_notifyEvents)
						Utils::notifyBlocked("Event: Bounty\nFrom: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID) + "\nStatus: Blocked");
					if (g_MenuScript->f_redirectEvents)
					{
						SCRIPT::TRIGGER_SCRIPT_EVENT(1, args, sizeof(args), 1 << SenderID);
						Utils::notifyAboveMap("Redirecting Bounty Event \nTo: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID));
					}
					return false;

				}
			case 693546501:
				if (g_MenuScript->f_antiKick)
				{

					if (g_MenuScript->f_notifyEvents)
						Utils::notifyBlocked("Event: Kick Event [1] \nFrom: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID) + "\nStatus: Blocked");

					return false;
					if (g_MenuScript->f_redirectEvents)
					{
						Functions::kick_to_new_lobby();
						Utils::notifyAboveMap("Redirecting Kick Event \nTo: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID));
					}
				}
				break;
			case 149365611:
				if (g_MenuScript->f_antiKick)
				{

					if (g_MenuScript->f_notifyEvents)
						Utils::notifyBlocked("Event: Kick Event [2] \nFrom: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID) + "\nStatus: Blocked");

					return false;
					if (g_MenuScript->f_redirectEvents)
					{
						Functions::kick_to_new_lobby();
						Utils::notifyAboveMap("Redirecting Kick Event \nTo: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID));
					}
				}
				break;
			case -941739545:
				if (g_MenuScript->f_antiKick)
				{

					if (g_MenuScript->f_notifyEvents)
						Utils::notifyBlocked("Event: Kick Event [3] \nFrom: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID) + "\nStatus: Blocked");

					return false;
					if (g_MenuScript->f_redirectEvents)
					{
						Functions::kick_to_new_lobby();
						Utils::notifyAboveMap("Redirecting Kick Event \nTo: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID));
					}
				}
				break;
			case -1941292498:
				if (g_MenuScript->f_antiKick)
				{

					if (g_MenuScript->f_notifyEvents)
						Utils::notifyBlocked("Event: Kick Event [4] \nFrom: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID) + "\nStatus: Blocked");

					return false;
					if (g_MenuScript->f_redirectEvents)
					{
						Functions::kick_to_new_lobby();
						Utils::notifyAboveMap("Redirecting Kick Event \nTo: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID));
					}
				}
				break;
			case 1953937033:
				if (g_MenuScript->f_antiKick)
				{

					if (g_MenuScript->f_notifyEvents)
						Utils::notifyBlocked("Event: Kick Event [5] \nFrom: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID) + "\nStatus: Blocked");

					return false;
					if (g_MenuScript->f_redirectEvents)
					{
						Functions::kick_to_new_lobby();
						Utils::notifyAboveMap("Redirecting Kick Event \nTo: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID));
					}
				}
				break;
			case 297770348:
				if (g_MenuScript->f_antiKick)
				{

					if (g_MenuScript->f_notifyEvents)
						Utils::notifyBlocked("Event: Kick Event [6] \nFrom: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID) + "\nStatus: Blocked");

					return false;
					if (g_MenuScript->f_redirectEvents)
					{
						Functions::kick_to_new_lobby();
						Utils::notifyAboveMap("Redirecting Kick Event \nTo: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID));
					}
				}
				break;
			case -815817885:
				if (g_MenuScript->f_antiKick)
				{

					if (g_MenuScript->f_notifyEvents)
						Utils::notifyBlocked("Event: Kick Event [7] \nFrom: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID) + "\nStatus: Blocked");

					return false;
					if (g_MenuScript->f_redirectEvents)
					{
						Functions::kick_to_new_lobby();
						Utils::notifyAboveMap("Redirecting Kick Event \nTo: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID));
					}
				}
				break;
			case -498955166:
				if (g_MenuScript->antiCamReset)
				{

					if (args[2] == -1432407380)
					{
						if (g_MenuScript->f_notifyEvents)
							Utils::notifyBlocked("Event: Camera Spam Event\nFrom: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID) + "\nStatus: Blocked");
						return false;
						if (g_MenuScript->f_redirectEvents)
						{
							SCRIPT::TRIGGER_SCRIPT_EVENT(1, args, sizeof(args), 1 << SenderID);
							Utils::notifyAboveMap("Redirecting Cam Reset Event \nTo: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID));
						}
					}

				}

			case 20218675039:
				if (g_MenuScript->f_antiKick)
				{

					if (g_MenuScript->f_notifyEvents)
						Utils::notifyBlocked("Event: Kick Event [8] \nFrom: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID) + "\nStatus: Blocked");

					return false;
					if (g_MenuScript->f_redirectEvents)
					{
						Functions::kick_to_new_lobby();
						Utils::notifyAboveMap("Redirecting Kick Event \nTo: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID));
					}
				}
				break;

			case -1424895288:
				if (g_MenuScript->f_antiKick)
				{

					if (g_MenuScript->f_notifyEvents)
						Utils::notifyBlocked("Event: Kick Event [9] \nFrom: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID) + "\nStatus: Blocked");

					return false;
					if (g_MenuScript->f_redirectEvents)
					{
						Functions::kick_to_new_lobby();
						Utils::notifyAboveMap("Redirecting Kick Event \nTo: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID));
					}
				}
				break;
			case -1264063129:
				if (g_MenuScript->f_antiKick)
				{

					if (g_MenuScript->f_notifyEvents)
						Utils::notifyBlocked("Event: Kick Event [10] \nFrom: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID) + "\nStatus: Blocked");

					return false;
					if (g_MenuScript->f_redirectEvents)
					{
						Functions::kick_to_new_lobby();
						Utils::notifyAboveMap("Redirecting Kick Event \nTo: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID));
					}
				}
				break;
			case 423635655:
				if (g_MenuScript->f_antiKick)
				{

					if (g_MenuScript->f_notifyEvents)
						Utils::notifyBlocked("Event: Kick Event [11] \nFrom: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID) + "\nStatus: Blocked");

					return false;
					if (g_MenuScript->f_redirectEvents)
					{
						Functions::kick_to_new_lobby();
						Utils::notifyAboveMap("Redirecting Kick Event \nTo: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID));
					}
				}
				break;
			case 400031869:
				if (g_MenuScript->f_antiKick)
				{

					if (g_MenuScript->f_notifyEvents)
						Utils::notifyBlocked("Event: Kick Event [12] \nFrom: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID) + "\nStatus: Blocked");

					return false;
					if (g_MenuScript->f_redirectEvents)
					{
						Functions::kick_to_new_lobby();
						Utils::notifyAboveMap("Redirecting Kick Event \nTo: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID));
					}
				}
				break;

			case -531496987:
				if (g_MenuScript->f_antiKick)
				{

					if (g_MenuScript->f_notifyEvents)
						Utils::notifyBlocked("Event: Kick Event [13] \nFrom: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID) + "\nStatus: Blocked");

					return false;
					if (g_MenuScript->f_redirectEvents)
					{
						Functions::kick_to_new_lobby();
						Utils::notifyAboveMap("Redirecting Kick Event \nTo: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID));
					}
				}
				break;
			case 1062544784:
				if (g_MenuScript->f_antiKick)
				{

					if (g_MenuScript->f_notifyEvents)
						Utils::notifyBlocked("Event: Kick Event [14] \nFrom: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID) + "\nStatus: Blocked");

					return false;
					if (g_MenuScript->f_redirectEvents)
					{
						Functions::kick_to_new_lobby();
						Utils::notifyAboveMap("Redirecting Kick Event \nTo: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID));
					}
				}
				break;
			case -803645682:
				if (g_MenuScript->f_antiKick)
				{

					if (g_MenuScript->f_notifyEvents)
						Utils::notifyBlocked("Event: Kick Event [15] \nFrom: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID) + "\nStatus: Blocked");

					return false;
					if (g_MenuScript->f_redirectEvents)
					{
						Functions::kick_to_new_lobby();
						Utils::notifyAboveMap("Redirecting Kick Event \nTo: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID));
					}
				}
				break;
			case -771910813:
				if (g_MenuScript->f_antiKick)
				{

					if (g_MenuScript->f_notifyEvents)
						Utils::notifyBlocked("Event: Kick Event [16] \nFrom: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID) + "\nStatus: Blocked");

					return false;
					if (g_MenuScript->f_redirectEvents)
					{
						Functions::kick_to_new_lobby();
						Utils::notifyAboveMap("Redirecting Kick Event \nTo: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID));
					}
				}
				break;
			case 1476326089:
				if (g_MenuScript->f_antiKick)
				{

					if (g_MenuScript->f_notifyEvents)
						Utils::notifyBlocked("Event: Kick Event [17] \nFrom: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID) + "\nStatus: Blocked");

					return false;
					if (g_MenuScript->f_redirectEvents)
					{
						Functions::kick_to_new_lobby();
						Utils::notifyAboveMap("Redirecting Kick Event \nTo: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID));
					}
				}
				break;
			case 1800294560:
				if (g_MenuScript->f_antiKick)
				{

					if (g_MenuScript->f_notifyEvents)
						Utils::notifyBlocked("Event: Kick Event [18] \nFrom: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID) + "\nStatus: Blocked");

					return false;
					if (g_MenuScript->f_redirectEvents)
					{
						Functions::kick_to_new_lobby();
						Utils::notifyAboveMap("Redirecting Kick Event \nTo: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID));
					}
				}
				break;
			case 1209128713:
				if (g_MenuScript->f_antiKick)
				{

					if (g_MenuScript->f_notifyEvents)
						Utils::notifyBlocked("Event: Kick Event [19] \nFrom: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID) + "\nStatus: Blocked");

					return false;
					if (g_MenuScript->f_redirectEvents)
					{
						Functions::kick_to_new_lobby();
						Utils::notifyAboveMap("Redirecting Kick Event \nTo: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID));
					}
				}
				break;
			case -123645928:
				if (g_MenuScript->f_antiKick)
				{

					if (g_MenuScript->f_notifyEvents)
						Utils::notifyBlocked("Event: Kick Event [20] \nFrom: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID) + "\nStatus: Blocked");

					return false;
					if (g_MenuScript->f_redirectEvents)
					{
						Functions::kick_to_new_lobby();
						Utils::notifyAboveMap("Redirecting Kick Event \nTo: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID));
					}
				}
				break;
			case -332594529:
				if (g_MenuScript->f_antiKick)
				{

					if (g_MenuScript->f_notifyEvents)
						Utils::notifyBlocked("Event: Kick Event [21] \nFrom: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID) + "\nStatus: Blocked");

					return false;
					if (g_MenuScript->f_redirectEvents)
					{
						Functions::kick_to_new_lobby();
						Utils::notifyAboveMap("Redirecting Kick Event \nTo: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID));
					}
				}
				break;
			case -163616977:
				if (g_MenuScript->f_antiRemoteWanted)
				{

					if (g_MenuScript->f_notifyEvents)
						Utils::notifyBlocked("Event: Remove Wanted Event\nFrom: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID) + "\nStatus: Blocked");

					return false;

				}
				break;
			case -348418057:
				if (g_MenuScript->f_antiSendToMission)
				{

					if (g_MenuScript->f_notifyEvents)
						Utils::notifyBlocked("Event: Send To Job Event\nFrom: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID) + "\nStatus: Blocked");
					if (g_MenuScript->f_redirectEvents)
					{
						SCRIPT::TRIGGER_SCRIPT_EVENT(1, args, sizeof(args), 1 << SenderID);
						Utils::notifyAboveMap("Redirecting Send To Job Event \nTo: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID));
					}
					return false;

				}

				break;
			case -701823896:
				if (g_MenuScript->f_antiCEOKick)
				{

					if (g_MenuScript->f_notifyEvents)
						Utils::notifyBlocked("Event: CEO Kick Event\nFrom: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID) + "\nStatus: Blocked");
					if (g_MenuScript->f_redirectEvents)
					{
						SCRIPT::TRIGGER_SCRIPT_EVENT(1, args, sizeof(args), 1 << SenderID);
						Utils::notifyAboveMap("Redirecting CEO Kick Event \nTo: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID));
					}
					return false;

				}
				break;
			case -327286343:
				if (g_MenuScript->f_antiCEOBan)
				{
					if (g_MenuScript->f_notifyEvents)
						Utils::notifyBlocked("Event: CEO Ban Event\nFrom: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID) + "\nStatus: Blocked");
					if (g_MenuScript->f_redirectEvents)
					{
						SCRIPT::TRIGGER_SCRIPT_EVENT(1, args, sizeof(args), 1 << SenderID);
						Utils::notifyAboveMap("Redirecting CEO Ban Event \nTo: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID));
					}
					return false;
				}
				break;
			case 564131320:
				if (g_MenuScript->f_antiNotifs)
				{
					if (g_MenuScript->f_notifyEvents)
						Utils::notifyBlocked("Event: Notification Event\nFrom: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID) + "\nStatus: Blocked");
					if (g_MenuScript->f_redirectEvents)
					{
						SCRIPT::TRIGGER_SCRIPT_EVENT(1, args, sizeof(args), 1 << SenderID);
						Utils::notifyAboveMap("Redirecting Notification Event \nTo: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID));
					}
					return false;
				}
				break;

			}
		}
		if (g_MenuScript->f_antiAll && SenderID < 32)
		{
			if (g_MenuScript->f_logEvents)
				g_Logger->Info("Blocked Script Event %i from Player %s", ScriptEventHash, PLAYER::GET_PLAYER_NAME(SenderID));
			if (g_MenuScript->f_notifyEvents)
				Utils::notifyBlocked("Event: " + (std::to_string(ScriptEventHash)) + "\nFrom: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID) + "\nStatus: Blocked");
			return true;
			if (g_MenuScript->f_redirectEvents)
			{
				SCRIPT::TRIGGER_SCRIPT_EVENT(1, args, sizeof(args), 1 << SenderID);
				Utils::notifyAboveMap("Redirecting Event " + (std::to_string(ScriptEventHash)) + "\nTo: " + (std::string)PLAYER::GET_PLAYER_NAME(SenderID));
			}
		}

		return static_cast<decltype(&ScriptEventHandler)>(g_Hooking->m_OriginalScriptEventHandler)(NetEventStruct, CNetGamePlayer);
	}
	bool Hooks::BlockDrops(void* drop, void* unk, CPed* ped)
	{
		if (g_MenuScript->f_blockdrops)
		{
			return false;
		}
		return static_cast<decltype(&BlockDrops)>(g_Hooking->m_OriginalDenyDrops)(drop, unk, ped);
	}
	void Hooks::DisableErrorScreen(char* entryHeader, char* entryLine1, int instructionalKey, char* entryLine2, BOOL p4, Any p5, Any* p6, Any* p7, BOOL background)
	{
		if (SCRIPT::GET_HASH_OF_THIS_SCRIPT_NAME() == MISC::GET_HASH_KEY("shop_controller"))
		{
			return;
		}
		return static_cast<decltype(&DisableErrorScreen)>(g_Hooking->m_OriginalDisableErrorScreen)(entryHeader, entryLine1, instructionalKey, entryLine2, p4, p5, p6, p7, background);
	}
	int Hooks::CensorChat(__int64 chat_menu, const char* user_text, const char** output_text)
	{
		if (g_MenuScript->f_chatrestrict)
		{
			return -1;
		}
		return static_cast<decltype(&CensorChat)>(g_Hooking->m_OriginalCensorChat)(chat_menu, user_text, output_text);
	}
	LRESULT Hooks::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		g_D3DRenderer->WndProc(hWnd, msg, wParam, lParam);
		return static_cast<decltype(&WndProc)>(g_Hooking->m_OriginalWndProc)(hWnd, msg, wParam, lParam);
	}

	HRESULT Hooks::Present(IDXGISwapChain* dis, UINT syncInterval, UINT flags)
	{
		if (g_Running)
		{
			g_D3DRenderer->BeginFrame();
			g_ScriptManager->OnD3DTick();
			g_D3DRenderer->EndFrame();
		}

		return g_Hooking->m_D3DHook.GetOriginal<decltype(&Present)>(PresentIndex)(dis, syncInterval, flags);
	}

	HRESULT Hooks::ResizeBuffers(IDXGISwapChain* dis, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags)
	{
		if (g_Running)
		{
			g_D3DRenderer->PreResize();
			auto hr = g_Hooking->m_D3DHook.GetOriginal<decltype(&ResizeBuffers)>(ResizeBuffersIndex)(dis, bufferCount, width, height, newFormat, swapChainFlags);
			if (SUCCEEDED(hr))
			{
				g_D3DRenderer->PostResize();
			}

			return hr;
		}

		return g_Hooking->m_D3DHook.GetOriginal<decltype(&ResizeBuffers)>(ResizeBuffersIndex)(dis, bufferCount, width, height, newFormat, swapChainFlags);
	}

	Hooking::Hooking():
		m_D3DHook(g_GameVariables->m_Swapchain, 18)
	{

		MH_Initialize();
		MH_CreateHook(g_GameFunctions->m_GetNumOfEvents, &Hooks::GetNumOfEvents, &m_OriginalGetNumOfEvents);
		MH_CreateHook(g_GameFunctions->m_GetLabelText, &Hooks::GetLabelText, &m_OriginalGetLabelText);
		MH_CreateHook(g_GameFunctions->m_TriggerScriptEvent, &Hooks::TriggerScriptEvent, &m_OriginalTriggerScriptEvent);
		MH_CreateHook(g_GameFunctions->m_SetWarningMessage, &Hooks::SetWarningMessage, &m_OriginalSetWarningMessage);
		//MH_CreateHook(g_GameFunctions->m_JoinPattern, &Hooks::JoinPlayer, &m_OriginalJoinPlayer);
		g_Logger->Info("Hooking Cash Drop Handler...");
		MH_CreateHook(g_GameFunctions->m_DenyDrops, &Hooks::BlockDrops, &m_OriginalDenyDrops);
		g_Logger->Info("Hooking Screen Hook...");
		MH_CreateHook(g_GameFunctions->m_DisableErrorScreen, &Hooks::DisableErrorScreen, &m_OriginalDisableErrorScreen);
		g_Logger->Info("Hooking Chat Censor Bypass...");
		MH_CreateHook(g_GameFunctions->m_ChatCensor, &Hooks::CensorChat, &m_OriginalCensorChat);
		g_Logger->Info("Hooking Script Event Handler...");
		MH_CreateHook(g_GameFunctions->m_ScriptEventHandler, &Hooks::ScriptEventHandler, &m_OriginalScriptEventHandler);
		MH_CreateHook(g_GameFunctions->m_WndProc, &Hooks::WndProc, &m_OriginalWndProc);
		//where we intiialize discord rpc
		DiscordMain();

		m_D3DHook.Hook(&Hooks::Present, Hooks::PresentIndex);
		m_D3DHook.Hook(&Hooks::ResizeBuffers, Hooks::ResizeBuffersIndex);
	}

	Hooking::~Hooking() noexcept
	{
		MH_RemoveHook(g_GameFunctions->m_WndProc);
		MH_RemoveHook(g_GameFunctions->m_GetEventData);
		MH_RemoveHook(g_GameFunctions->m_GetLabelText);
		//MH_RemoveHook(g_GameFunctions->m_JoinPattern);
		MH_RemoveHook(g_GameFunctions->m_TriggerScriptEvent);
		MH_RemoveHook(g_GameFunctions->m_ScriptedGameEvent);
		MH_RemoveHook(g_GameFunctions->m_SetWarningMessage);
		MH_RemoveHook(g_GameFunctions->m_GetNumOfEvents);
		MH_Uninitialize();
	}

	void Hooking::Hook()
	{
		m_D3DHook.Enable();
		MH_EnableHook(MH_ALL_HOOKS);
	}

	void Hooking::Unhook()
	{
		m_D3DHook.Disable();
		MH_DisableHook(MH_ALL_HOOKS);

	}
	/*scrThread* ActiveThread()
	{
		return *(scrThread**)(*(std::uintptr_t*)(__readgsqword(0x58)) + 0x830);
	}
	void set_active_thread(scrThread* thread)
	{
		*(scrThread**)(*(std::uintptr_t*)(__readgsqword(0x58)) + 0x830) = thread;
	}*/
	std::uint64_t** Hooking::getGlobalPtr()
	{
		return g_GameVariables->m_GlobalPtr;
	}

#pragma warning (push)
#pragma warning (disable: 4100)
	void __stdcall pTick(LPVOID lpParameter)
	{
		try {
			g_MenuScript->Initialize();
		}
		catch (...) {}
	}
#pragma warning (pop)
	struct objectHash
	{
		DWORD* ptr;
		DWORD	hash;
	};

#define get_array_size(array)	(sizeof(array) / sizeof(array[0]))
	static std::vector<objectHash>	g_antiCrash;
	DWORD __stdcall antiCrashSearch(LPVOID lpParam)
	{
		static void* objectHashTable;
		static void* objectHashTableSectionEnd;

		auto&& hashTableRegion = Signature("01 ? ? 08 43 7f 2e 27 ? ? FF 0F").VirtualScan();
		objectHashTable = hashTableRegion.Pointer();
		objectHashTableSectionEnd = hashTableRegion.End();
		Hash* hashptr = (Hash*)objectHashTable;
		char* end = (char*)objectHashTableSectionEnd - 0x20;

		for (; (char*)hashptr < end; ++hashptr)
		{
			if (*hashptr == 0)
				continue;
			for (int i = 0; i < get_array_size(Hashes::forbidden_object); ++i)
			{
				if (*hashptr != Hashes::forbidden_object[i])
					continue;
				//g_antiCrash.push_back({ *hashptr });
				g_Logger->Info("working");
			}
		}
		return S_OK;
	}
	bool Hooks::Swapping(bool toggle)
	{
		static HANDLE	hAntiCrash = 0;
		constexpr Hash	dildo = 0xafc55086;
		Hash* hashptr = nullptr;
		char* end = nullptr;
		
		if (!g_antiCrash.empty() || hAntiCrash)
			goto LABEL_SET_VALUE;
		
		if (!toggle)
			return true;
		
		hAntiCrash = CreateThread(NULL,
			0,
			antiCrashSearch,
			NULL,
			0,
			nullptr);
	
	LABEL_SET_VALUE:
		
		if (WaitForSingleObject(hAntiCrash, 0) != WAIT_OBJECT_0)	//if the thread is still alive, return; it's still searching
			return false;
		
		if (hAntiCrash)
			CloseHandle(hAntiCrash);
		
		if (!toggle)
		{
			
			for (std::vector<objectHash>::iterator it = g_antiCrash.begin(); it != g_antiCrash.end(); ++it)
			{
				if (*it->ptr == 0xafc55086)
				{
					
				}
				if (it->hash == 0xafc55086)
				{
					
				}
				if (*it->ptr == 0xdf9841d7)
				{
					
				}
				if (it->hash == 0xdf9841d7)
				{
				}

				if (*it->ptr != 0xafc55086 && *it->ptr != it->hash)
				{	
					continue;
				}
				*it->ptr = 0xafc55086;
				it->hash = 0xafc55086;
				*it->ptr = it->hash;
			}
			return true;
		}
		
		for (std::vector<objectHash>::iterator it = g_antiCrash.begin(); it != g_antiCrash.end(); ++it)
			*it->ptr = 0xafc55086;
		return true;
	}
	scrThread* find_networked(std::uint32_t hash)
	{
		for (auto thread : *g_GameVariables->m_ScriptThread)
		{
			if (!thread)
			{
				continue;
			}
			if (thread->m_context.m_state != eThreadState::running)
			{
				continue;
			}
			if (thread->m_context.m_script_hash != hash)
			{
				continue;
			}
			return thread;
		}
		return nullptr;
	}
#include "Util.hpp"
	void Hooks::tick()
	{
		scrThread* networked_thread{};
		if (*g_GameVariables->m_IsSessionStarted && (networked_thread = find_networked(RAGE_JOAAT("freemode"))) != nullptr)
		{
			if (g_GameVariables->m_IsSessionStarted)
			{
				auto og_thread = ActiveThread();
			}
		}
		else
		{
			g_ScriptManager->OnGameTick();
		}
	}
}
