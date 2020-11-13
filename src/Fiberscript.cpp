#include "Common.hpp"

#pragma warning (push)
#pragma warning (disable: 4100)

namespace Mystic
{
	bool FiberScript::IsInitialized()
	{
		return m_Initialized;
	}

	ScriptType FiberScript::GetType()
	{
		return ScriptType::Game;
	}

	void FiberScript::Initialize()
	{
		m_Initialized = true;
	}

	void FiberScript::Destroy()
	{
	}

	void FiberScript::Tick()
	{
		if (!m_main_fiber)
		{
			m_main_fiber = IsThreadAFiber() ? GetCurrentFiber() : ConvertThreadToFiber(nullptr);
		}

		if (GetTickCount() < m_wake_at)
			return;

		if (!m_script_fiber)
		{
			m_script_fiber = CreateFiber(0, [](PVOID param)
			{
				try
				{
					while (1)
					{
						if (!g_FiberScript->getTasks().empty())
						{
							std::lock_guard lock(g_FiberScript->get_mutex());

							std::invoke(g_FiberScript->getTasks().front());
							g_FiberScript->getTasks().pop();
						}

						g_FiberScript->Wait(0);
					}
				}
				catch (...)
				{
					g_Logger->Info("Unable to start Fiber script.");
				}

			}, nullptr);
		}
		SwitchToFiber(m_script_fiber);
	}

	void FiberScript::Wait(DWORD ms)
	{
		m_wake_at = timeGetTime() + ms;
		SwitchToFiber(m_main_fiber);
	}
}

#pragma warning (pop)
