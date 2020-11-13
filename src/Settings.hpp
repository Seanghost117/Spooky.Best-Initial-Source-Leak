#pragma once

class Settings
{
public:
	std::atomic_bool m_LogWindow = false;
	std::atomic_bool m_LockMouse = false;

	int m_SelectedCharacter = 0;
	const char* m_CharSlot = "MP0";
	const char* m_Header;
	int m_LevelSkip = 1;
	bool m_ExperimentalFeatures = false;
	bool TPWindowActivated = false;
	std::string initialusername;
	std::string initialuserpassword;
	bool proceed_auth = false;
	bool USerAuthType = false;
	int m_authLevel = 3;
	bool m_TargetFriendly = false;

	bool spawnIn = true;
	bool spawnMax = true;
	bool spawnGod = false;
	bool spawnEngineOn = true;
	bool spawnClearArea = false;
	inline const char* const BoolToString(bool b)
	{
		return b ? "True" : "False";
	}
};

inline Settings g_Settings;
