#pragma once
#include "Script.hpp"

namespace Mystic
{
	class ScriptManager
	{
	public:
		explicit ScriptManager();
		~ScriptManager() noexcept;
		ScriptManager(Logger const&) = delete;
		ScriptManager(Logger&&) = delete;
		ScriptManager& operator=(Logger const&) = delete;
		ScriptManager& operator=(Logger&&) = delete;

		void AddScript(std::shared_ptr<Script> script);
		void RemoveScript(Script* script);

		void OnGameTick();
		void OnD3DTick();

	private:
		std::mutex m_Mutex;
		std::vector<std::shared_ptr<Script>> m_Scripts;
	};

	inline std::unique_ptr<ScriptManager> g_ScriptManager;
}
