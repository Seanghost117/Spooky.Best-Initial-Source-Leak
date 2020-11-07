#pragma once

namespace Mystic
{
	class VMTHook
	{
	public:
		explicit VMTHook(void* object, std::size_t numFuncs) noexcept:
			m_Object(reinterpret_cast<std::uintptr_t**>(object)),
			m_NumFuncs(numFuncs),
			m_Original(*m_Object),
			m_New(std::make_unique<std::uintptr_t[]>(m_NumFuncs + 1))
		{
			std::copy_n(m_Original - 1, m_NumFuncs + 1, m_New.get());
		}

		~VMTHook() noexcept = default;

		void Enable() noexcept
		{
			*m_Object = &m_New[1];
		}

		void Disable() noexcept
		{
			*m_Object = m_Original;
		}

		void Hook(void* replacement, std::size_t index) noexcept
		{
			m_New[index + 1] = reinterpret_cast<std::uintptr_t>(replacement);
		}

		void Unhook(std::size_t index) noexcept
		{
			m_New[index + 1] = m_Original[index];
		}

		template <typename T>
		T GetOriginal(std::size_t index) noexcept
		{
			return reinterpret_cast<T>(m_Original[index]);
		}

		VMTHook(VMTHook const&) noexcept = delete;
		VMTHook(VMTHook&&) noexcept = delete;
		VMTHook& operator=(VMTHook const&) noexcept = delete;
		VMTHook& operator=(VMTHook&&) noexcept = delete;
	private:
		std::uintptr_t** m_Object;
		std::size_t m_NumFuncs;

		std::uintptr_t* m_Original;
		std::unique_ptr<std::uintptr_t[]> m_New;
	};
}
