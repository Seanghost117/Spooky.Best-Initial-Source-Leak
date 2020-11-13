#include "Structs.hpp"
#include "Reclass.hpp"

#pragma once

namespace rage
{
	struct scrNativeRegistration;
	struct scrNativeCallContext;
}

namespace Mystic
{
	using tSetVectorResult = void(*)(rage::scrNativeCallContext* context);
	using tAddOwnedExplosion = bool(*)(Ped ped, NativeVector3 coords, int explosionType, float damageScale, bool isAudible, bool isInvisible, float cameraShake);

	class GameVariables
	{
	public:
		explicit GameVariables();
		~GameVariables() noexcept = default;
		GameVariables(GameVariables const&) = delete;
		GameVariables(GameVariables&&) = delete;
		GameVariables& operator=(GameVariables const&) = delete;
		GameVariables& operator=(GameVariables&&) = delete;

		void PostInit();

		void OwnedExplosionBypass(bool toggle);
		char* m_OwnedExplosionBypass;

		clockTime* m_ClockTime;

		const int m_EventCount = 84;
		std::vector<void*> m_EventPtr;
		char m_EventRestore[84] = {};
		bool* m_IsSessionStarted{};
		std::uint32_t* m_GameState;
		std::uint32_t* m_FrameCount;
		HWND m_GameWindow;
		IDXGISwapChain* m_Swapchain;
		rage::scrNativeRegistration** m_NativeRegistrations;
		std::uint64_t** m_GlobalBase;
		char* m_GameBuild;
		char* m_EventHook;
		std::uint64_t m_WorldPtr{};
		std::uint64_t m_PlayerPtr{};
		std::uint64_t** m_GlobalPtr{};
		std::uint64_t m_TransactionProcessor{};
		void* m_JmpRbx{};
		tSetVectorResult m_SetVectorResult{};

		pgCollection<scrThread*>* m_ScriptThread;
		uint32_t activeThreadTlsOffset = 0x830;

		HMODULE m_HModule = GetModuleHandleA(nullptr);

		template <typename T> T& get(std::string variableName, int at_index = -1)
		{
			static std::vector<Pointer<T>> pointers;

			for (int i = 0; i < pointers.size(); i++)
			{
				if (pointers[i].str == variableName && pointers[i].at_index == at_index)
					return pointers[i].value;
			}

			T temp{};

			pointers.push_back({ variableName, temp, at_index });

			return get<T>(variableName, at_index);
		}

		bool& getBool(std::string name, int index = -1)
		{
			for (int i = 0; i < boolVector.size(); i++)
			{
				if (boolVector[i].name == name && boolVector[i].index == index)
					return boolVector[i].value;
			}

			boolVector.push_back({ name, false, index });

			return getBool(name, index);
		}

	private:
		std::vector<Bool> boolVector;
	};

	class GameFunctions
	{
	public:
		explicit GameFunctions();
		~GameFunctions() noexcept = default;
		GameFunctions(GameFunctions const&) = delete;
		GameFunctions(GameFunctions&&) = delete;
		GameFunctions& operator=(GameFunctions const&) = delete;
		GameFunctions& operator=(GameFunctions&&) = delete;

		void DefuseEvent(eRockstarEvent e, bool toggle);
		bool ogGetScriptHandlerIfNetworked();
		bool ogScriptedGameEvent(int64_t a1, int64_t a2);

		using GetNumOfEvents = int(bool p0);
		GetNumOfEvents* m_GetNumOfEvents;

		using WndProc = LRESULT(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		WndProc* m_WndProc;

		using GetEventData = bool(std::int32_t eventGroup, std::int32_t eventIndex, std::int64_t* args, std::uint32_t argCount);
		GetEventData* m_GetEventData;

		using NetworkEventHandler = void(__int64 a1, CNetGamePlayer* a2, __int64 a3, unsigned __int16 a4, int a5, int a6, __int64 a7, __int64 a8);
		NetworkEventHandler* m_NetworkEventHandler;

		using ScriptEventHandler = bool(void* events, CNetGamePlayer *sourcePlayer, CNetGamePlayer *targetPlayer);
		ScriptEventHandler* m_ScriptEventHandler;

		using GetLabelText = const char*(void* unk, const char* label);
		GetLabelText* m_GetLabelText;

		using TriggerScriptEvent = int(bool unk0, uint64_t* args, int argCount, int bitFlags);
		TriggerScriptEvent* m_TriggerScriptEvent;

		using SpectatePlayer = bool(bool toggle, Ped playerPed);
		SpectatePlayer* m_SpectatePlayer;

		using GetWaypointPos = bool(Vector2 *pos);
		GetWaypointPos* m_GetWaypointPos;

		using SetWarningMessage = void(char* entryHeader, char* entryLine1, int instructionalKey, char* entryLine2, BOOL p4, Any p5, Any* p6, Any* p7, BOOL background);
		SetWarningMessage* m_SetWarningMessage;

		using FileRegister = std::uint32_t(std::uint32_t* texture_id, const char* path, bool p1, const char* name, bool p2);
		FileRegister* m_FileRegister;

		using SessionWeather = void(char a1, int a2, int a3, __int64 a4);
		SessionWeather* m_SessionWeather;

		using OwnedExplosion = void(Ped ped, NativeVector3 coords, int explosionType, float damageScale, bool isAudible, bool isInvisible, float cameraShake);
		OwnedExplosion* m_AddOwnedExplosion;

		using GetPlayerName = char*(Player player);
		GetPlayerName* m_GetPlayerName;

		using GetPlayerAddress = __int64(Player player);
		GetPlayerAddress* m_GetPlayerAddress;

		using HandleToPointer = std::uint64_t(Entity entity);
		HandleToPointer* m_HandleToPointer;

		using NetworkBlock = int(__int64 a1, CNetGamePlayer *source, CNetGamePlayer *target, std::uint16_t eventId, std::int32_t unk1, std::int32_t unk2, std::int64_t unk3, datBitBuffer* buffer);
		NetworkBlock* m_NetworkBlock;

		using GetGameplayCamCoord = void(NativeVector3 * out);
		GetGameplayCamCoord* m_GetGameplayCamCoord;

		using GetPlayerNetworkId = CNetGamePlayer * (int player);
		GetPlayerNetworkId* m_GetPlayerNetworkId;

		using GiveWantedLevel = void(CNetGamePlayer* a1, int64_t wantedLevel, int a3, char a4);
		GiveWantedLevel* m_GiveWantedLevel;

		using SessionTime = void(int hour, int min);
		SessionTime* m_SessionTime;

		using NetWorkShopBeginService = bool(std::uint64_t a1, Any* a2, int a3, Any a4, int transactionHash, Any a6, int a7, int mode);
		NetWorkShopBeginService* m_NetShopBeginService;

		using NetworkShopCheckoutStart = bool(std::uint64_t a1, Any transactionId, int unk);
		NetworkShopCheckoutStart* m_NetShopCheckoutStart;

		using SetEntityMaxHealth = void(Entity entity, int value);
		SetEntityMaxHealth* m_SetEntityMaxHealth;

		using ScriptedGameEvent = bool(__int64 a1, __int64 a2);
		ScriptedGameEvent* m_ScriptedGameEvent;
		int fpGetEventData();

		using GetScriptHandlerIfNetworked = void* (*) ();
		GetScriptHandlerIfNetworked* m_GetScriptHandlerIfNetworked;

		using GetScriptHandler = void* (*) ();
		GetScriptHandler* m_GetScriptHandler;

		using MobileRadio = bool(bool toggle);
		MobileRadio* m_MobileRadio;

		using DisableErrorScreen = void(char* entryHeader, char* entryLine1, int instructionalKey, char* entryLine2, BOOL p4, Any p5, Any* p6, Any* p7, BOOL background);
		DisableErrorScreen* m_DisableErrorScreen;

		using ChatCensor = int(__int64 chat_menu, const char* user_text, const char** output_text);
		ChatCensor* m_ChatCensor;

		using DenyDrops = bool(void* drop, void* unk, CPed* ped);
		DenyDrops* m_DenyDrops;

		using StatSetInt = bool(Hash statname, int value, bool save);
		StatSetInt* m_StatSetInt;

		using StatSetBool = bool(Hash statname, bool value, bool save);
		StatSetBool* m_StatSetBool;

		using StatSetFloat = bool(Hash statname, float value, bool save);
		StatSetFloat* m_StatSetFloat;

		using WorldToScreen = void(NativeVector3*, float*, float*);
		WorldToScreen* m_W2S;

		using BonePosAd = void(NativeVector3 pos);
		BonePosAd* m_BonePositionAddress;

		using JoinPattern = int(__int64 a1, int a2, __int64* a3, int a4);
		JoinPattern* m_JoinPattern;
	};

	inline std::unique_ptr<GameVariables> g_GameVariables;
	inline std::unique_ptr<GameFunctions> g_GameFunctions;
}
