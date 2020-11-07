#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT _WIN32_WINNT_WIN7
#define _HAS_STD_BYTE 0

#pragma comment(lib, "Winmm.lib")

#include <SDKDDKVer.h>

#include <winsdkver.h>
#include <sdkddkver.h>
#include <windows.h>
#include <winternl.h>
#include <intrin.h>
#include <emmintrin.h>
#include <nmmintrin.h>
#include <timeapi.h>
#include <d3d11.h>
#include <shlobj.h>

#include <cassert>
#include <cinttypes>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <deque>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <mutex>
#include <optional>
#include <sstream>
#include <stack>
#include <string>
#include <thread>
#include <type_traits>
#include <unordered_map>
#include <xstring>
#include <vector>
#include <set>
#include <queue>
#include <wrl/client.h>

#define Mystic_WIDE_IMPL(str) L ## str
#define Mystic_WIDE(str) Mystic_WIDE_IMPL(str)

#define Mystic_STRINGIZE_IMPL(x) #x
#define Mystic_STRINGIZE(x) Mystic_STRINGIZE_IMPL(x)

#define Mystic_NAME "Mystic"	


class sysMemAllocator;

template <typename T>
using comptr = Microsoft::WRL::ComPtr<T>;
using namespace std::chrono_literals;
namespace fs = std::filesystem;

inline HMODULE g_Module{};
inline std::atomic_bool g_Running = true;

inline std::uint32_t g_SelectedPlayer{};

inline bool g_LogScriptEvents = false;
using present_callback_t = void(*)(void* callback);

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <JSON/json.hpp>

#include <cmath>

#include "Keyboard.hpp"
#include "enums.hpp"
#include "Levels.hpp"
#include "Types.hpp"
#include "MemoryEdit.hpp"
#include "Settings.hpp"
#include "Functions.hpp"
#include "Utils.hpp"
#include "Script.hpp"
#include "Blackholes.hpp"
#include "Logger.hpp"
#include "List.hpp"
#include "Util.hpp"
#include "Memory.hpp"
#include "Game.hpp"
#include "ScriptGlobal.hpp"
#include "SubMenus.hpp"
#include "Lists.hpp"
#include "Natives.hpp"
#include "spotifyhelper.hpp"
#include "Timer.hpp"
#include "Translation.hpp"
#include "TeleportManager.h"
#include "CustomText.hpp"
#include "Colors.hpp"
#include "HeaderManager.hpp"
#include "UI/UIManager.hpp"
#include "UI/BoolOption.hpp"
#include "UI/ChooseOption.hpp"
#include "UI/NumberOption.hpp"
#include "UI/RegularOption.hpp"
#include "UI/SubOption.hpp"
#include "UI/RegularSubmenu.hpp"
#include "UI/PlayerSubmenu.hpp"

// Dll entry point
#include "Reclass.hpp"
#include "NativeSpoofer.hpp"
#include "Game.hpp"
#include "Hooking.hpp"
#include "ScriptManager.hpp"
#include "LogScript.hpp"
#include "MenuScript.hpp"
#include "ScriptCallback.hpp"
#include "Fiberscript.hpp"
#include "Invoker.hpp"
#include "CustomText.hpp"
#include "D3DRenderer.hpp"
#include "UI/UIManager.hpp"
#include "Translation.hpp"
#include "fonts.hpp"
//#include "spotifyaction.hpp"

#include "Hooking.hpp"
#include "Game.hpp"
#include "Hashes.hpp"
#include "Structs.hpp"
#include "NetworkThreadHandoff.hpp"
#include "CustomText.hpp"
#include "Util.hpp"
#include "D3DRenderer.hpp"
#include "UI/UIManager.hpp"
#include <MinHook/MinHook.h>

class Ini
{
private:
	std::string inifile;
public:
	Ini(std::string file)
	{
		this->inifile = file;
	}

	void WriteString(std::string string, std::string app, std::string key)
	{
		WritePrivateProfileStringA(app.c_str(), key.c_str(), string.c_str(), this->inifile.c_str());
	}
	std::string GetString(std::string app, std::string key)
	{
		char buf[100];
		GetPrivateProfileStringA(app.c_str(), key.c_str(), "NULL", buf, 100, this->inifile.c_str());
		return (std::string)buf;
	}

	void WriteInt(int value, std::string app, std::string key)
	{
		WriteString(std::to_string(value), app, key);
	}
	int GetInt(std::string app, std::string key)
	{
		int iResult = GetPrivateProfileInt((LPCWSTR)app.c_str(), (LPCWSTR)key.c_str(), 90210, (LPCWSTR)this->inifile.c_str());
		return iResult;
	}

	void WriteFloat(float value, std::string app, std::string key)
	{
		WriteString(std::to_string(value), app, key);
	}
	float GetFloat(std::string app, std::string key)
	{
		return std::stof(GetString(app, key));
	}
};
//auth 
