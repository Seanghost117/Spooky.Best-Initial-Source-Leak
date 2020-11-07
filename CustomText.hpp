#pragma once
#include "Script.hpp"

namespace Mystic
{
	class CustomText
	{
	public:
		explicit CustomText() = default;
		~CustomText() noexcept = default;

		CustomText(CustomText const&) = delete;
		CustomText(CustomText&&) = delete;
		CustomText& operator=(CustomText const&) = delete;
		CustomText& operator=(CustomText&&) = delete;

		void AddText(std::uint32_t hash, const char* text);
		void RemoveText(std::uint32_t hash);
		const char* GetText(std::uint32_t hash);
	private:
		std::unordered_map<std::uint32_t, std::unique_ptr<char[]>> m_TextMap;
	};

	inline std::unique_ptr<CustomText> g_CustomText;
}
