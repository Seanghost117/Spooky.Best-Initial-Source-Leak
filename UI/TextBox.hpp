#pragma once
#include "../Common.hpp"

namespace Mystic
{
	class TextBox
	{
	public:

		const char* GetText()
		{
			return m_Buffer.c_str();
		}

		void Add(const char* text)
		{
			m_Buffer += text;
		}

		void pop_back_utf8(std::string& utf8)
		{
			if (utf8.empty())
				return;

			auto cp = utf8.data() + utf8.size();
			while (--cp >= utf8.data() && ((*cp & 0b10000000) && !(*cp & 0b01000000))) {}
			if (cp >= utf8.data())
				utf8.resize(cp - utf8.data());
		}

		void RemoveLastChar()
		{
			pop_back_utf8(m_Buffer);
		}
	private:
		std::string m_Buffer;
	};
}
