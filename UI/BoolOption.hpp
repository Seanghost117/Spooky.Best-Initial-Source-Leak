#pragma once
#include "BaseOption.hpp"
#include "UIManager.hpp"

namespace Mystic::UserInterface
{
	enum class BoolDisplay
	{
		OnOff,
		YesNo
	};

	template <typename BoolType = bool>
	class BoolOption : public BaseOption<BoolOption<BoolType>>
	{
	public:
		explicit BoolOption(const char* text, const char* description, BoolType* b00l, BoolDisplay displayType, bool displayInverted = false, std::function<void()> action = [] {}) :
			m_Bool(b00l),
			m_DisplayInverted(displayInverted),
			m_DisplayType(displayType)
		{
			Base::SetLeftText(text);
			if (description)
				Base::SetDescription(description);
			Base::SetAction(std::move(action));
		}

		~BoolOption() noexcept = default;
		BoolOption(BoolOption const&) = default;
		BoolOption& operator=(BoolOption const&) = default;
		BoolOption(BoolOption&&) = default;
		BoolOption& operator=(BoolOption&&) = default;	

		const char* GetRightText() override
		{
			switch (m_DisplayType)
			{
			case BoolDisplay::OnOff:
				Base::SetRightText((*m_Bool ^ m_DisplayInverted) ? "~g~ON" : "~r~OFF");
				break;
			case BoolDisplay::YesNo:
				Base::SetRightText((*m_Bool ^ m_DisplayInverted) ? "~g~YES" : "~r~NO");
				break;
			}
			
			return Base::GetRightText();
		}

		void HandleAction(OptionAction action) override
		{
			if (action == OptionAction::EnterPress)
			{
				*m_Bool = !*m_Bool;
			}

			Base::HandleAction(action);
		}
	private:
		BoolType* m_Bool;
		BoolDisplay m_DisplayType;
		BoolType m_DisplayInverted = false;

		using Base = BaseOption<BoolOption<BoolType>>;
	};
}
