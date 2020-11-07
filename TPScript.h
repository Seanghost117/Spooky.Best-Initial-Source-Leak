#pragma once
#include "Script.hpp"

namespace Mystic
{
	class TPScript : public Script
	{
	public:
		explicit TPScript() = default;
		~TPScript() noexcept = default;

		bool IsInitialized() override;
		ScriptType GetType() override;
		void Initialize() override;
		void Destroy() override;
		void Tick() override;
	private:
		bool m_Initialized{};
	};

	inline std::shared_ptr<TPScript> g_TPScript;
}
