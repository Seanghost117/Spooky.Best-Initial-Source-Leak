#include "ScriptCallback.hpp"

namespace Mystic
{
	bool CallbackScript::IsInitialized()
	{
		return m_Initialized;
	}

	ScriptType CallbackScript::GetType()
	{
		return ScriptType::Game;
	}

	void CallbackScript::Initialize()
	{
		m_Initialized = true;
	}

	void CallbackScript::Destroy()
	{
	}

	void CallbackScript::Tick()
	{
		for (auto it = m_Callbacks.begin(); it != m_Callbacks.end();)
		{
			auto&& cb = *it;
			if (cb->IsDone())
			{
				cb->OnSuccess();
				it = m_Callbacks.erase(it);
				//return;
			}
			else
			{
				cb->OnFailure();
				++it;
			}
		}
	}
}
