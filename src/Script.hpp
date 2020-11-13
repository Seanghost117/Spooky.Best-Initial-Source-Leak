#pragma once

namespace Mystic
{
	enum class ScriptType
	{
		Game,
		D3D
	};

	enum class ScriptAction
	{
		D3DResizeBuffersPre,
		D3DResizeBuffersPost,
		GetLabelText
	};

	class Script
	{
	public:
		virtual ~Script() noexcept = default;
		Script(Script const&) = delete;
		Script(Script&&) = delete;
		Script& operator=(Script const&) = delete;
		Script& operator=(Script&&) = delete;

		virtual bool IsInitialized() = 0;
		virtual ScriptType GetType() = 0;
		virtual void Initialize() = 0;
		virtual void Destroy() = 0;
		virtual void Tick() = 0;
	protected:
		Script() = default;
	};
}
