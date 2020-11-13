#include "Game.hpp"
#include "Invoker.hpp"
#include "Common.hpp"
#include "Functions.hpp"

namespace Mystic
{
	GameVariables::GameVariables() :
		m_GameState(Signature(("48 85 C9 74 4B 83 3D")).Scan().Add(7).Rip().As<decltype(m_GameState)>()),
		m_FrameCount(Signature(("F3 0F 10 0D ? ? ? ? 44 89 6B 08")).Scan().Add(4).Rip().Sub(8).As<decltype(m_FrameCount)>()),
		m_GameWindow(FindWindowA("grcWindow", "Grand Theft Auto V")),
		m_Swapchain(Signature(("48 8B 0D ? ? ? ? 48 8D 55 A0 48 8B 01")).Scan().Add(3).Rip().As<decltype(m_Swapchain)&>()),
		m_NativeRegistrations(Signature(("48 83 EC 20 48 8D 0D ? ? ? ? E8 ? ? ? ? 0F B7 15 ? ? ? ? 33 FF")).Scan().Add(7).Rip().As<decltype(m_NativeRegistrations)>()),
		m_GlobalBase(Signature(("4C 8D 4D 08 48 8D 15 ? ? ? ? 4C 8B C0")).Scan().Add(7).Rip().As<decltype(m_GlobalBase)>()),
		m_WorldPtr(Signature(("48 8B C3 48 83 C4 20 5B C3 0F B7 05 ? ? ? ?")).Scan().Sub(11).Add(3).Rip().As<decltype(m_WorldPtr)>()),
		m_PlayerPtr(Signature(("48 8B 0D ? ? ? ? E8 ? ? ? ? 48 8B C8 E8 ? ? ? ? 48 8B CF")).Scan().Add(3).Rip().As<decltype(m_PlayerPtr)>()),
		m_GlobalPtr(Signature(("4C 8D 4D 08 48 8D 15 ? ? ? ? 4C 8B C0")).Scan().Add(7).Rip().As<decltype(m_GlobalPtr)>()),
		m_JmpRbx(Signature(("FF 23")).Scan().As<decltype(m_JmpRbx)>()),
		m_SetVectorResult(Signature(("83 79 18 00 48 8B D1 74 4A FF 4A 18")).Scan().As<decltype(m_SetVectorResult)>()),
		m_ScriptThread(Signature(("48 8B C8 EB 03 48 8B CB 48 8B 05")).Scan().Add(11).Rip().As<decltype(m_ScriptThread)>()),
		m_OwnedExplosionBypass(Signature(("3B F8 40 0F 94 C7")).Scan().As<char*>()),
		m_IsSessionStarted(Signature("44 38 1D ? ? ? ? 0F 84 ? ? ? ? 48 8B 01 FF 90 ? ? ? ? 48 8B 85").Scan().Add(3).Rip().As<bool*>()),
		m_EventHook(Signature(("48 83 EC 28 E8 ? ? ? ? 48 8B 0D ? ? ? ? 4C 8D 0D ? ? ? ? 4C 8D 05 ? ? ? ? BA 03")).Scan().As<decltype(m_EventHook)>()),
		m_ClockTime(Signature(("48 8D 0D ? ? ? ? 8B FA E8 ? ? ? ? 44 8D 0C 5B")).Scan().Add(3).Rip().As<decltype(m_ClockTime)>()),
		m_TransactionProcessor(Signature(("48 8B 0D ? ? ? ? E8 ? ? ? ? 84 C0 75 1F 40 84 FF")).Scan().Add(3).Rip().As<decltype(m_TransactionProcessor)>())
	{
		auto sig = Signature("48 83 EC 60 48 8D 0D ? ? ? ? E8").Scan().Sub(17);
		m_GameBuild = sig.Add(265 + 3).Rip().As<decltype(m_GameBuild)>();

		if (m_EventHook)
		{
			int i = 0, j = 0, matches = 0, found = 0;
			const char* pattern = "\x4C\x8D\x05";
			while (found != m_EventCount)
			{
				if (m_EventHook[i] == pattern[j])
				{
					if (++matches == 3)
					{
						m_EventPtr.push_back((void*)(reinterpret_cast<uint64_t>(m_EventHook + i - j) + *reinterpret_cast<int*>(m_EventHook + i + 1) + 7));
						found++;
						j = matches = 0;
					}
					j++;
				}
				else
				{
					matches = j = 0;
				}
				i++;
			}
		}
	}
	void GameVariables::OwnedExplosionBypass(bool toggle)
	{
		static constexpr bytes<2> patched = { 0x39, 0xDB };
		static constexpr bytes<2> restore = { 0x3B, 0xF8 };
		if (g_GameVariables->m_OwnedExplosionBypass == nullptr) return;
		*reinterpret_cast<bytes<2>*>(g_GameVariables->m_OwnedExplosionBypass) = toggle ? patched : restore;
	}

	void GameVariables::PostInit()
	{
	}
	 
	GameFunctions::GameFunctions() :
		m_GetNumOfEvents(Signature("48 83 EC 28 33 D2 85 C9").Scan().As<decltype(m_GetNumOfEvents)>()),
		m_WndProc(Signature("48 8B 0D ? ? ? ? 48 8D 55 EF FF 15").Scan().Sub(853).As<decltype(m_WndProc)>()),
		m_GetEventData(Signature("48 85 C0 74 14 4C 8B 10").Scan().Sub(28).As<decltype(m_GetEventData)>()),
		m_SessionTime(Signature("48 89 5C 24 08 57 48 83 EC 20 8B F9 48 8B 0D ? ? ? ? 48 8B DA 33 D2 E9 ? ? ? ?").Scan().As<decltype(m_SessionTime)>()),
		m_GetLabelText(Signature("75 ? E8 ? ? ? ? 8B 0D ? ? ? ? 65 48 8B 04 25 ? ? ? ? BA ? ? ? ? 48 8B 04 C8 8B 0C 02 D1 E9").Scan().Sub(0).Sub(19).As<decltype(m_GetLabelText)>()),
		m_TriggerScriptEvent(Signature("48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 48 89 78 20 41 56 48 81 EC ? ? ? ? 45 8B F0 41 8B F9").Scan().As<decltype(m_TriggerScriptEvent)>()),
		m_SpectatePlayer(Signature("48 89 5C 24 ? 57 48 83 EC 20 41 8A F8 84 C9").Scan().As<decltype(m_SpectatePlayer)>()),
		m_ChatCensor(Signature("E8 ? ? ? ? 83 F8 FF 75 B9").Scan().Add(1).Rip().As<decltype(m_ChatCensor)>()),
		m_DenyDrops(Signature("49 8B 80 ? ? ? ? 48 85 C0 74 0C F6 80 ? ? ? ? ? 75 03 32 C0 C3").Scan().As<decltype(m_DenyDrops)>()),
		m_DisableErrorScreen(Signature("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC 60 4C 8B F2 48 8B 94 24 ? ? ? ? 33 DB").Scan().As<decltype(m_DisableErrorScreen)>()),
		m_JoinPattern(Signature("48 8B C4 48 89 58 10 48 89 70 18 48 89 78 20 48 89 48 08 55 41 54 41 55 41 56 41 57 48 8D 68 A8").Scan().As<decltype(m_JoinPattern)>()),
		m_GetWaypointPos(Signature("48 89 5C 24 08 57 48 83 EC 20 8B 15 ? ? ? ? 65 48 8B 04 25 ? ? ? ? 48 8B F9 48 8B 04 D0 B9 B4").Scan().As<decltype(m_GetWaypointPos)>()),
		m_SetWarningMessage(Signature("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC 60 4C 8B F2 48 8B 94 24 ? ? ? ? 33 DB").Scan().As<decltype(m_SetWarningMessage)>()),
		m_FileRegister(Signature("C3 48 89 5C 24 ? 48 89 6C 24 ? 48 89 7C 24 ? 41 54 41 56 41 57 48 83 EC 50").Scan().Add(1).As<decltype(m_FileRegister)>()),
		m_SessionWeather(Signature("E8 ? ? ? ? 48 85 C0 74 12 4C 8B C3").Scan().Add(116).As<decltype(m_SessionWeather)>()),
		m_NetworkEventHandler(Signature("66 41 83 F9 ? 0F 83 ? ? ? ?").Scan().As<decltype(m_NetworkEventHandler)>()),
		m_ScriptEventHandler(Signature("40 53 48 81 EC ? ? ? ? 44 8B 81 ? ? ? ? 4C 8B CA 41 8D 40 FF 3D ? ? ? ? 77 42").Scan().As<decltype(m_ScriptEventHandler)>()),
		m_AddOwnedExplosion(Signature("55 48 8D 68 B9 48 81 EC ? ? ? ? 0F 29 70 E8 41 8B D8").Scan().Sub(19).As<decltype(m_AddOwnedExplosion)>()), //48 8B C4 48 89 58 08 48 89 70 10 48 89 78 18 4C 89 70 20 55 48 8D 68 B9 48 81 EC ? ? ? ? 0F 29 70 E8 41 8B D8
		m_GetPlayerName(Signature("40 53 48 83 EC 20 80 3D ? ? ? ? ? 8B D9 74 22").Scan().As<decltype(m_GetPlayerName)>()),
		m_GetPlayerAddress(Signature("77 26 E8 ? ? ? ? 48 85 C0 74 1C 48 8B C8").Scan().Sub(19).As<decltype(m_GetPlayerAddress)>()),
		m_HandleToPointer(Signature("83 F9 FF 74 31 4C 8B 0D ? ? ? ?").Scan().As<decltype(m_HandleToPointer)>()),
		m_NetworkBlock(Signature("66 41 83 F9 ? 0F 83 ? ? ? ?").Scan().As<decltype(m_NetworkBlock)>()),
		m_GetPlayerNetworkId(Signature("80 F9 20 73 13 48 8B").Scan().As<decltype(m_GetPlayerNetworkId)>()),
		m_GiveWantedLevel(Signature("48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 44 88 48 20 57 41 54 41 55 41 56 41 57 48 83 EC 30 4C 8B F1 48 8B 0D ? ? ? ? 44 8B E2 B2 01 41 8A D9 45 8B E8 E8 ? ? ? ? 48 8B 0D ? ? ? ?").Scan().As<decltype(m_GiveWantedLevel)>()),
		m_NetShopBeginService(Signature("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC 30 83 0A FF").Scan().As<decltype(m_NetShopBeginService)>()),
		m_NetShopCheckoutStart(Signature("48 89 5C 24 ? 57 48 83 EC 20 48 8B 59 20 45 32 C0").Scan().As<decltype(m_NetShopCheckoutStart)>()),
		m_SetEntityMaxHealth(Signature("40 53 48 83 EC 20 E8 ? ? ? ? 48 8B D8 48 85 C0 74 19 4C 8B 00 8B 15 ? ? ? ? 48 8B C8 41 FF 50 08 84 C0 74 05 48 8B C3 EB 02 33 C0 48 83 C4 20 5B C3 48 89 5C 24 ? 48 89 6C 24 ? 56 57").Scan().As<decltype(m_SetEntityMaxHealth)>()),
		m_ScriptedGameEvent(Signature("40 53 48 81 EC ? ? ? ? 44 8B 81 ? ? ? ?").Scan().As<decltype(m_ScriptedGameEvent)>()),
		m_GetScriptHandlerIfNetworked(Signature("40 53 48 83 EC 20 E8 ? ? ? ? 48 8B D8 48 85 C0 74 12 48 8B 10 48 8B C8").Scan().Add(1).As<decltype(m_GetScriptHandlerIfNetworked)>()),
		m_GetScriptHandler(Signature("40 53 48 83 EC 20 E8 ? ? ? ? 48 8B D8 48 85 C0 74 12 48 8B 10 48 8B C8").Scan().Add(1).As<decltype(m_GetScriptHandler)>()),
		m_MobileRadio(Signature("40 53 48 83 EC 20 8A D9 48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8D 0D ? ? ? ? 8B D0 E8 ? ? ? ? 48 85 C0 74 10").Scan().Add(1).As<decltype(m_MobileRadio)>()),
		m_W2S(Signature("48 89 5C 24 ?? 55 56 57 48 83 EC 70 65 4C 8B 0C 25").Scan().As<decltype(m_W2S)>()),
		m_GetGameplayCamCoord(Signature(("40 53 48 83 EC 20 48 8B D9 E8 ? ? ? ? 8B 90 ? ? ? ?")).Scan().As<decltype(m_GetGameplayCamCoord)>()),
		m_BonePositionAddress(Signature("48 89 5C 24 ?? 48 89 6C 24 ?? 48 89 74 24 ?? 57 48 83 EC 60 48 8B 01 41 8B E8 48 8B F2 48 8B F9 33 DB").Scan().As<decltype(m_BonePositionAddress)>())

	{}

	void GameFunctions::DefuseEvent(eRockstarEvent event, bool toggle)
	{
		static const unsigned char retn = 0xC3;
		char* p = (char*)g_GameVariables->m_EventPtr[event];
		if (toggle)
		{
			if (g_GameVariables->m_EventRestore[event] == 0)
				g_GameVariables->m_EventRestore[event] = p[0];
			*p = retn;
		}
		else
		{
			if (g_GameVariables->m_EventRestore[event] != 0)
				*p = g_GameVariables->m_EventRestore[event];
		}
	}
	

}
