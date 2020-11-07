#include "Common.hpp"
#include "TPScript.h"
#include "TeleportManager.h"
#include "Executables.hpp"

void ErasePE()
{
	char* pBaseAddr = (char*)GetModuleHandle(nullptr);
	DWORD dwOldProtect = 0;
	VirtualProtect(pBaseAddr, 4096, PAGE_READWRITE, &dwOldProtect);
	ZeroMemory(pBaseAddr, 4096);
	VirtualProtect(pBaseAddr, 4096, dwOldProtect, &dwOldProtect);
}
BOOL DllMain(HINSTANCE hInstance, DWORD reason, LPVOID)
{
	using namespace Mystic;
	using namespace Mystic::UserInterface;

	if (reason == DLL_PROCESS_ATTACH)
	{
		g_Module = hInstance;
		CreateThread(nullptr, 0, [](LPVOID) -> DWORD
		{
			
			g_Logger = std::make_unique<Logger>();
			g_Logger->Info("Mystic Injected.");

			wchar_t Folder[1024];
			HRESULT hr = SHGetFolderPathW(nullptr, CSIDL_MYDOCUMENTS, nullptr, 0, Folder);
			if (SUCCEEDED(hr))
			{
				char str[1024]; wcstombs(str, Folder, 1023); char temp[1024];
				std::snprintf(&temp[0], sizeof(temp) - 1, "%s//Mystic//", &str);
				try{if (!std::filesystem::exists(&temp[0])) {std::filesystem::create_directory(&temp[0]);}}
				catch (fs::filesystem_error const&){}
			}
			g_Logger->Info("Loading Translation Manager.");
			g_TranslationManager = std::make_unique<TranslationManager>();
			g_TranslationManager->LoadTranslations("English");

			g_TeleportManager = std::make_unique<TeleportManager>();
			g_ExeManager = std::make_unique<ExeManager>();
			g_ExeManager->Init();

			g_Logger->Info("Hooking Game Functions.");
			g_GameFunctions = std::make_unique<GameFunctions>();
			g_Logger->Info("Hooking Game Variables.");
			g_GameVariables = std::make_unique<GameVariables>();

			// Wait for the game to load
			while (*g_GameVariables->m_GameState != GameStatePlaying)
			{
				std::this_thread::sleep_for(3s);
				std::this_thread::yield();
			}

			g_GameVariables->PostInit();

			g_Logger->Info("Initializing Invoker.");
			g_Invoker = std::make_unique<Invoker>();
			g_CustomText = std::make_unique<CustomText>();
			g_Logger->Info("Initializing Renderer.");
			g_D3DRenderer = std::make_unique<D3DRenderer>();
			g_Logger->Info("Initializing UI.");
			g_UiManager = std::make_unique<UserInterface::UIManager>();
			g_Spoofer = std::make_unique<Spoofer>(g_GameVariables->m_JmpRbx);

			g_Logger->Info("Loading Scripts.");
			g_ScriptManager = std::make_unique<ScriptManager>();
			g_MenuScript = std::make_shared<MenuScript>();
			g_AuthScript = std::make_shared<AuthScript>();
			g_LogScript = std::make_shared<LogScript>();
			g_TPScript = std::make_shared<TPScript>();
			g_CallbackScript = std::make_shared<CallbackScript>();
			g_FiberScript = std::make_shared<FiberScript>();
			g_ScriptManager->AddScript(g_MenuScript);
			g_ScriptManager->AddScript(g_LogScript);
			g_ScriptManager->AddScript(g_TPScript);
			g_ScriptManager->AddScript(g_CallbackScript);
			g_ScriptManager->AddScript(g_FiberScript);
			g_ScriptManager->AddScript(g_AuthScript);


			g_Logger->Info("Mystic Hooking.");
			g_MenuScript->InitializeVariables();
			g_Hooking = std::make_unique<Hooking>();
			g_Hooking->Hook();

			system("cls");
			g_Logger->Info("Mystic Hooked");

			globalHandle(4267883).As<bool>() = true; //SP DLC vehicle bypass

			while (g_Running)
			{
				if (IsKeyPressed(VK_DELETE))
					g_Running = false;

				std::this_thread::sleep_for(3ms);
				std::this_thread::yield();
			}

			std::this_thread::sleep_for(500ms);
			g_Hooking->Unhook();

			g_ScriptManager.reset();
			g_LogScript.reset();
			g_TPScript.reset();
			g_MenuScript.reset();
			g_CallbackScript.reset();
			g_FiberScript.reset();
			g_AuthScript.reset();
		

			g_UiManager.reset();
			g_D3DRenderer.reset();
			g_CustomText.reset();
			g_Invoker.reset();

			std::this_thread::sleep_for(500ms);

			g_Hooking.reset();

			g_TranslationManager.reset();
			g_TeleportManager.reset();
			g_ExeManager.reset();

			g_GameVariables.reset();
			g_GameFunctions.reset();
			
			g_Logger->Info("Mystic unhooked.");
			g_Logger.reset();

			FreeLibraryAndExitThread(g_Module, 0);
		}, nullptr, 0, nullptr);
	}

	return true;
}
