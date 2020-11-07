#pragma once

namespace Mystic {

	inline scrThread* ActiveThread()
	{
		//char* moduleTls = *(char**)__readgsqword(88);
		//return *reinterpret_cast<scrThread * *>(moduleTls + Mystic::g_GameVariables->activeThreadTlsOffset);
		return *(scrThread**)(*(std::uintptr_t*)(__readgsqword(0x58)) + 0x830);
	}

	inline void setActiveThread(scrThread* thread)
	{
		//char* moduleTls = *(char**)__readgsqword(88);
		//*reinterpret_cast<scrThread**>(moduleTls + Mystic::g_GameVariables->activeThreadTlsOffset) = thread;
		*(scrThread**)(*(std::uintptr_t*)(__readgsqword(0x58)) + 0x830) = thread;
	}

	inline scrThread* findNetworked(std::uint32_t hash)
	{
		for (auto thread : *Mystic::g_GameVariables->m_ScriptThread)
		{
			if (!thread)
			{
				continue;
			}
			if (thread->m_context.m_state != eThreadState::running)
			{
				continue;
			}
			if (thread->m_context.m_script_hash != hash)
			{
				continue;
			}
			return thread;
		}
		return nullptr;
	}

	inline bool is_script_thread_good(scrThread* thread)
	{
		if (!thread)
			return false;
		if (thread->m_context.m_state != eThreadState::running)
			return false;
		return true;
	}
}


