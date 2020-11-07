#pragma once
#include <JSON/json.hpp>

namespace Mystic
{
	class TeleportManager
	{
	public:
		explicit TeleportManager()
		{
			std::snprintf(&m_TeleportDir[0], sizeof(m_TeleportDir) - 1, "%s\\" Mystic_NAME "\\GTA\\Teleports\\", std::getenv("appdata"));
			try
			{
				if (!std::filesystem::exists(&m_TeleportDir[0])) {
					std::filesystem::create_directory(&m_TeleportDir[0]);
				}
			}
			catch (fs::filesystem_error const&)
			{
			}
		}

		const char* GetTeleportDirectory()
		{
			return &m_TeleportDir[0];
		}

		void AddTeleport(std::string name, NativeVector3 position)
		{
			char prefix[64] = {};
			std::snprintf(prefix, sizeof(prefix) - 1, "%s%s", name, ".mys");

			m_FilePath.append(std::getenv("appdata"));
			m_FilePath.append(Mystic_NAME);
			m_FilePath.append("GTA\\");
			m_FilePath.append("Teleports\\");
			if (!fs::exists(m_FilePath))
				fs::create_directory(m_FilePath);

			std::ofstream ofs(m_FilePath /= prefix);
			ofs << position.x << "," << position.y << "," << position.z;
			ofs.close();
		}

		~TeleportManager() noexcept = default;
		TeleportManager(TeleportManager const&) = delete;
		TeleportManager(TeleportManager&&) = delete;
		TeleportManager& operator=(TeleportManager const&) = delete;
		TeleportManager& operator=(TeleportManager&&) = delete;
	private:
		std::unordered_map<std::uint32_t, std::string> m_Teleports;
		char m_TeleportDir[1024] = {};
		fs::path m_FilePath;
	};

	inline std::unique_ptr<TeleportManager> g_TeleportManager;
}