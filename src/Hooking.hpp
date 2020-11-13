#pragma once
#include "VMTHook.hpp"

namespace Mystic
{
	struct Hooks
	{
		static int GetNumOfEvents(int eventgroup);
		static const char* GetLabelText(void* unk, const char* label);
		static void NetworkEventHandler(__int64 a1, CNetGamePlayer* a2, __int64 a3, unsigned __int16 a4, int a5, int a6, __int64 a7, __int64 a8);
		static bool ScriptEventHandler(std::int64_t NetEventStruct, std::int64_t CNetGamePlayer);
		static int TriggerScriptEvent(bool unk0, uint64_t* args, int argCount, int bitFlags);
		static int JoinPlayer(__int64 a1, int a2, __int64* a3, int a4);
		static int Pattern(int a1, __int64 a2, unsigned int a3, __int64 a4, __int64 a5, __int64 a6, __int64 a7);
		static void SetWarningMessage(char* entryHeader, char* entryLine1, int instructionalKey, char* entryLine2, BOOL p4, Any p5, Any* p6, Any* p7, BOOL background);
		static void DisableErrorScreen(char* entryHeader, char* entryLine1, int instructionalKey, char* entryLine2, BOOL p4, Any p5, Any* p6, Any* p7, BOOL background);
		static int CensorChat(__int64 chat_menu, const char* user_text, const char** output_text);
		static bool BlockDrops(void* drop, void* unk, CPed* ped);
		static LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static constexpr auto PresentIndex = 8;
		static void tick();
		bool Swapping(bool toggle);
		static HRESULT Present(IDXGISwapChain* dis, UINT syncInterval, UINT flags);

		static constexpr auto ResizeBuffersIndex = 13;
		static HRESULT ResizeBuffers(IDXGISwapChain* dis, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags);

	};

	class Hooking
	{
	public:
		explicit Hooking();
		~Hooking() noexcept;
		Hooking(Hooking const&) = delete;
		Hooking(Hooking&&) = delete;
		Hooking& operator=(Hooking const&) = delete;
		Hooking& operator=(Hooking&&) = delete;

		void Hook();
		void Unhook();
		static std::uint64_t** getGlobalPtr();

		std::set<present_callback_t>& GetPresentCallbacks()
		{
			return m_PresentCallbacks;
		}

		friend struct Hooks;
	private:
		void* m_NtQueryVirtualMemory{};

		void* m_OriginalGetNumOfEvents{};
		void* m_OriginalGetLabelText{};
		void* m_OriginalScanEventQueue{};
		void* m_OriginalScanChatMessage{};
		void* m_OriginalGetEventData{};
		void* m_OriginalNetworkEventHandler{};
		void* m_OriginalScriptEventHandler{};
		void* m_OriginalScriptedGameEvent{};
		void* m_OriginalTriggerScriptEvent{};
		void* m_OriginalCensorChatText{};
		void* m_OriginalDisableErrorScreen{};
		void* m_OriginalCensorChat{};
		void* m_OriginalDenyDrops{};
		void* m_OriginalSetWarningMessage{};
		void* m_OriginalJoinPlayer{};
		void* m_OriginalPattern{};
		void* m_OriginalWndProc{};
		VMTHook m_D3DHook;

		std::set<present_callback_t> m_PresentCallbacks{};
	};

	inline std::unique_ptr<Hooking> g_Hooking;
}
