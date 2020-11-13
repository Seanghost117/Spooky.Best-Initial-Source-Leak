#pragma once
#include "Script.hpp"

namespace Mystic
{
	class LogScript : public Script
	{
	public:
		explicit LogScript() = default;
		~LogScript() noexcept = default;

		bool IsInitialized() override;
		ScriptType GetType() override;
		void Initialize() override;
		void Destroy() override;
		void Tick() override;
	private:
		bool m_Initialized{};
	};

	inline std::shared_ptr<LogScript> g_LogScript;
}
