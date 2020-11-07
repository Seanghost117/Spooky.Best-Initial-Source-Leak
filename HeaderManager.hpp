#pragma once
#include <JSON/json.hpp>

namespace Mystic
{
	class HeaderManager
	{
	public:
		explicit HeaderManager()
		{
			std::snprintf(&m_HeaderDir[0], sizeof(m_HeaderDir) - 1, "%s\\" Mystic_NAME "\\GTA\\Headers\\", std::getenv("appdata"));
			try
			{
				if (!std::filesystem::exists(&m_HeaderDir[0])) {
					std::filesystem::create_directory(&m_HeaderDir[0]);
				}
			}
			catch (fs::filesystem_error const&)
			{
			}
		}

		const char* GetHeaderDirectory()
		{
			return &m_HeaderDir[0];
		}

		~HeaderManager() noexcept = default;
		HeaderManager(HeaderManager const&) = delete;
		HeaderManager(HeaderManager&&) = delete;
		HeaderManager& operator=(HeaderManager const&) = delete;
		HeaderManager& operator=(HeaderManager&&) = delete;
	private:
		std::unordered_map<std::uint32_t, std::string> m_Headers;
		char m_HeaderDir[1024] = {};
		fs::path m_FilePath;
	};

	inline std::unique_ptr<HeaderManager> g_HeaderManager;
}