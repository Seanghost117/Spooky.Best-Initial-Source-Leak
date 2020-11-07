#include "UIManager.hpp"
#include "../Timer.hpp"
#include "../Natives.hpp"

namespace Mystic::UserInterface
{
	void UIManager::OnTick()
	{
		std::lock_guard lock(m_Mutex);
		Ped ped = PLAYER::PLAYER_PED_ID();
		Player player = PLAYER::PLAYER_ID();
		Vehicle vehicle = PED::GET_VEHICLE_PED_IS_IN(ped, false);
		NativeVector3 coords = ENTITY::GET_ENTITY_COORDS(ped, true);

		if (IsMouseLocked())
		{
			PAD::DISABLE_ALL_CONTROL_ACTIONS(0);
			if(!g_Settings.m_LockMouse)
			ShowCursor(true);
			SetCursor(LoadCursorA(NULL, (LPCSTR)IDC_ARROW));
		
			if (GetMenuRect().IsInBounds(GetMousePos()))
			{
				g_Logger->Info("Mystic Check...");
			}
		}

		/*if (g_UiManager->m_Opened && g_Settings.m_LockMouse)
		{
			PAD::DISABLE_ALL_CONTROL_ACTIONS(0);
		}*/

		CheckForInput();
		HandleInput();

		if (!Utils::isPlayerDead() && *g_GameVariables->m_GameState != GameStateLoading) {
			Functions::setFunctionLoop(ped, player, vehicle, coords);
		}

		if (m_Opened && g_Settings.USerAuthType)
		{
			globalHandle(19486).At(1).As<int>() = 1; // Disable phone
			globalHandle(14536).As<bool>() = true; // Disable phone
			m_DrawBaseY = m_PosY;
			DrawHeader();
			if (!m_SubmenuStack.empty())
			{
				auto sub = m_SubmenuStack.top();
				sub->Reset();
				sub->Execute();

				DrawSubmenuBar(sub);
				if (sub->GetNumOptions() != 0)
				{
					std::size_t startPoint = 0;
					std::size_t endPoint = sub->GetNumOptions() > m_OptionsPerPage ? m_OptionsPerPage : sub->GetNumOptions();
					if (sub->GetNumOptions() > m_OptionsPerPage && sub->GetSelectedOption() >= m_OptionsPerPage)
					{
						startPoint = sub->GetSelectedOption() - m_OptionsPerPage + 1;
						endPoint = sub->GetSelectedOption() + 1;
					}

					for (std::size_t i = startPoint, j = 0; i < endPoint; ++i, ++j)
					{
						DrawOption(sub->GetOption(i), i == sub->GetSelectedOption());
					}
				}
			}

			DrawFooter();
			DrawDescription();
			DrawInstructions();
		}
	}
	std::pair<UINT16, UINT16> menubindsGamepad = { INPUT_FRONTEND_RB, INPUT_FRONTEND_RIGHT };

	void UIManager::CheckForInput()
	{
		ResetInput();
		UINT8 index1 = menubindsGamepad.first < 50 ? 0 : 2;
		UINT8 index2 = menubindsGamepad.second < 50 ? 0 : 2;
		m_OpenKeyPressed = IsKeyPressed(std::stoi(m_OpenKey)) || PAD::IS_DISABLED_CONTROL_PRESSED(index1, menubindsGamepad.first) && PAD::IS_DISABLED_CONTROL_JUST_PRESSED(index2, menubindsGamepad.second);
		m_BackKeyPressed = IsKeyPressed(std::stoi(m_BackKey)) || PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_FRONTEND_RRIGHT);
		m_EnterKeyPressed = IsKeyPressed(std::stoi(m_EnterKey)) || PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_FRONTEND_ACCEPT);
		m_UpKeyPressed = IsKeyPressed(std::stoi(m_UpKey)) || PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_FRONTEND_UP);
		m_DownKeyPressed = IsKeyPressed(std::stoi(m_DownKey)) || PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_FRONTEND_DOWN);
		m_LeftKeyPressed = IsKeyPressed(std::stoi(m_LeftKey)) || PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_FRONTEND_LEFT);
		m_RightKeyPressed = IsKeyPressed(std::stoi(m_RightKey)) || PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_FRONTEND_RIGHT);
	}

	void UIManager::HandleInput()
	{
		static auto openTicker = GetTickCount();
		if (m_OpenKeyPressed && GetTickCount() - openTicker >= static_cast<std::uint32_t>(m_OpenDelay))
		{
			openTicker = GetTickCount();
			m_Opened ^= true;

			if (m_Sounds)
				AUDIO::PLAY_SOUND_FRONTEND(-1, m_Opened ? "SELECT" : "BACK", "HUD_FRONTEND_DEFAULT_SOUNDSET", false);
		}

		static Timer backTimer(0ms);
		backTimer.SetDelay(std::chrono::milliseconds(m_BackDelay));
		if (m_Opened && m_BackKeyPressed && backTimer.Update())
		{
			if (m_Sounds)
				AUDIO::PLAY_SOUND_FRONTEND(-1, "BACK", "HUD_FRONTEND_DEFAULT_SOUNDSET", false);

			if (m_SubmenuStack.size() <= 1)
			{
				m_Opened = false;
			}
			else
			{
				m_SubmenuStack.pop();
			}
		}
		
		if (m_Opened && !m_SubmenuStack.empty())
		{
			auto sub = m_SubmenuStack.top();

			static Timer enterTimer(0ms);
			enterTimer.SetDelay(std::chrono::milliseconds(m_EnterDelay));
			if (m_EnterKeyPressed && sub->GetNumOptions() != 0 && enterTimer.Update())
			{
				if (m_Sounds)
					AUDIO::PLAY_SOUND_FRONTEND(-1, "SELECT", "HUD_FRONTEND_DEFAULT_SOUNDSET", false);

				if (auto opt = sub->GetOption(sub->GetSelectedOption()))
				{
					opt->HandleAction(OptionAction::EnterPress);
				}
			}

			static Timer upTimer(0ms);
			upTimer.SetDelay(std::chrono::milliseconds(m_VerticalDelay));
			if (m_UpKeyPressed && sub->GetNumOptions() != 0 && upTimer.Update())
			{
				if (m_Sounds)
					AUDIO::PLAY_SOUND_FRONTEND(-1, "NAV_UP_DOWN", "HUD_FRONTEND_DEFAULT_SOUNDSET", false);

				sub->ScrollBackward();
			}

			static Timer downTimer(0ms);
			downTimer.SetDelay(std::chrono::milliseconds(m_VerticalDelay));
			if (m_DownKeyPressed && sub->GetNumOptions() != 0 && downTimer.Update())
			{
				if (m_Sounds)
					AUDIO::PLAY_SOUND_FRONTEND(-1, "NAV_UP_DOWN", "HUD_FRONTEND_DEFAULT_SOUNDSET", false);

				sub->ScrollForward();
			}

			static Timer leftTimer(0ms);
			leftTimer.SetDelay(std::chrono::milliseconds(m_HorizontalDelay));
			if (m_LeftKeyPressed && sub->GetNumOptions() != 0 && leftTimer.Update())
			{
				if (m_Sounds)
					AUDIO::PLAY_SOUND_FRONTEND(-1, "NAV_LEFT_RIGHT", "HUD_FRONTEND_DEFAULT_SOUNDSET", false);

				if (auto opt = sub->GetOption(sub->GetSelectedOption()))
				{
					opt->HandleAction(OptionAction::LeftPress);
				}
			}

			static Timer rightTimer(0ms);
			rightTimer.SetDelay(std::chrono::milliseconds(m_HorizontalDelay));
			if (m_RightKeyPressed && sub->GetNumOptions() != 0 && rightTimer.Update())
			{
				if (m_Sounds)
					AUDIO::PLAY_SOUND_FRONTEND(-1, "NAV_LEFT_RIGHT", "HUD_FRONTEND_DEFAULT_SOUNDSET", false);

				if (auto opt = sub->GetOption(sub->GetSelectedOption()))
				{
					opt->HandleAction(OptionAction::RightPress);
				}
			}
		}
	}

	void UIManager::ResetInput()
	{
		m_OpenKeyPressed = false;
		m_BackKeyPressed = false;
		m_EnterKeyPressed = false;
		m_UpKeyPressed = false;
		m_DownKeyPressed = false;
		m_LeftKeyPressed = false;
		m_RightKeyPressed = false;
	}

	void UIManager::DrawHeader()
	{
		switch (m_HeaderType)
		{
		case HeaderType::Static:
			DrawRect(
				m_PosX,
				m_DrawBaseY + (m_HeaderHeight / 2.f), m_Width,
				m_HeaderHeight,
				m_HeaderBackgroundColor);
			break;
		case HeaderType::Gradient:
			for (std::size_t i = 0; i < (m_HeaderGradientTransparent ? 1 : 20); ++i)
			{
				DrawSprite(
					"aircraft_dials",
					"aircraft_dials_g0",
					m_PosX - (m_HeaderGradientStretch / 2.f),
					m_DrawBaseY + (m_HeaderHeight / 2.f),
					m_Width + m_HeaderGradientStretch + m_HeaderGradientFiller,
					m_HeaderHeight,
					m_HeaderGradientColorRight,
					m_HeaderGradientFlip ? 0.f : 180.f);
				DrawSprite(
					"aircraft_dials",
					"aircraft_dials_g0",
					m_PosX + (m_HeaderGradientStretch / 2.f),
					m_DrawBaseY + (m_HeaderHeight / 2.f),
					m_Width + m_HeaderGradientStretch + m_HeaderGradientFiller,
					m_HeaderHeight,
					m_HeaderGradientColorLeft,
					m_HeaderGradientFlip ? 180.f : 0.f);
			}
			break;
		case HeaderType::YTD:
			DrawSprite(		
				(const char*)g_Settings.m_Header ,
				(const char*)g_Settings.m_Header,
				m_PosX,
				m_DrawBaseY + (m_HeaderHeight / 2.f), 
				m_Width,
				m_HeaderHeight,
				m_YTDHeaderBackgroundColor,
				0.f);
			break;
			break;
		}


		if (m_HeaderText)
		{
			DrawCenteredText(
				Mystic_NAME,
				m_PosX,
				m_DrawBaseY + (m_HeaderHeight / 2.f) - (GetTextHeight(m_HeaderFont, m_HeaderTextSize) / 2.f),
				m_HeaderTextSize,
				m_HeaderFont,
				m_HeaderTextColor,
				false, true);
		}

		m_DrawBaseY += m_HeaderHeight;
	}

	void UIManager::DrawSubmenuBar(AbstractSubmenu* sub)
	{
		char leftText[64] = {};
		std::strncpy(&leftText[0], sub->GetName(), sizeof(leftText) - 1);
		std::transform(std::begin(leftText), std::end(leftText), std::begin(leftText), [](char c) { return static_cast<char>(toupper(c)); });

		char rightText[32] = {};
		std::snprintf(rightText, sizeof(rightText) - 1, "%zu / %zu", sub->GetSelectedOption() + 1, sub->GetNumOptions());

		DrawRect(
			m_PosX,
			m_DrawBaseY + (m_SubmenuBarHeight / 2.f),
			m_Width, m_SubmenuBarHeight,
			m_SubmenuBarBackgroundColor);
		DrawLeftText(
			&leftText[0],
			m_PosX - (m_Width / m_SubmenuBarPadding),
			m_DrawBaseY + (m_SubmenuBarHeight / 2.f) - (GetTextHeight(m_SubmenuBarFont, m_SubmenuBarTextSize) / 1.5f),
			m_SubmenuBarTextSize, m_SubmenuBarFont,
			m_SubmenuBarTextColor,
			false, true);
		DrawRightText(
			&rightText[0],
			m_PosX + (m_Width / m_SubmenuBarPadding),
			m_DrawBaseY + (m_SubmenuBarHeight / 2.f) - (GetTextHeight(m_SubmenuBarFont, m_SubmenuBarTextSize) / 1.5f),
			m_SubmenuBarTextSize, m_SubmenuBarFont,
			m_SubmenuBarTextColor,
			false, true);

		m_DrawBaseY += m_SubmenuBarHeight;
	}

	void UIManager::DrawOption(AbstractOption* opt, bool selected)
	{
		DrawRect(
			m_PosX,
			m_DrawBaseY + (m_OptionHeight / 2.f),
			m_Width,
			m_OptionHeight,
			selected ? m_OptionSelectedBackgroundColor : m_OptionUnselectedBackgroundColor);
		DrawLeftText(
			opt->GetLeftText(),
			m_PosX - (m_Width / m_OptionPadding),
			m_DrawBaseY + (m_OptionHeight / 2.f) - (GetTextHeight(m_OptionFont, m_OptionTextSize) / 1.5f),
			m_OptionTextSize,
			m_OptionFont,
			selected ? m_OptionSelectedTextColor : m_OptionUnselectedTextColor,
			false, false);
		DrawRightText(
			opt->GetRightText(),
			m_PosX + (m_Width / m_OptionPadding),
			m_DrawBaseY + (m_OptionHeight / 2.f) - (GetTextHeight(m_OptionFont, m_OptionTextSize) / 1.5f),
			m_OptionTextSize,
			m_OptionFont,
			selected ? m_OptionSelectedTextColor : m_OptionUnselectedTextColor,
			false, false);

		if (opt->GetFlag(OptionFlag::Enterable))
		{
			DrawRightText(
				">",
				m_PosX + (m_Width / m_OptionPadding - 0.006f),
				m_DrawBaseY + (m_OptionHeight / 2.f) - (GetTextHeight(Font::Monospace, m_OptionTextSize) / 1.725f),
				m_OptionTextSize,
				Font::Monospace,
				selected ? m_OptionSelectedTextColor : m_OptionUnselectedTextColor,
				false, false);
		}

		if (opt->GetFlag(OptionFlag::BoolOption))
		{
			if (!g_GameVariables->getBool("isGUIToggledOn"))
			{
				GRAPHICS::DRAW_SPRITE("CommonMenu", "common_medal", m_PosX + (m_Width / m_OptionPadding - 0.01f), 
					m_DrawBaseY + (m_OptionHeight / 2.f) - (GetTextHeight(m_OptionFont, m_OptionTextSize) / 1.5f) + 0.015f, 
					0.020f, 0.030f, 0.0, 169, 169, 169, 255, 0);
			}
			else {
				selected 
					? GRAPHICS::DRAW_SPRITE("CommonMenu", "common_medal", g_UiManager->m_PosX + (g_UiManager->m_Width / g_UiManager->m_OptionPadding - 0.01f), 
						g_UiManager->m_DrawBaseY + (g_UiManager->m_OptionHeight / 2.f) - (g_UiManager->GetTextHeight(g_UiManager->m_OptionFont, g_UiManager->m_OptionTextSize) / 1.5f) + 0.015f,
						0.020f, 0.030f, 0.0, 60, 60, 60, 255, 0)
					: GRAPHICS::DRAW_SPRITE("CommonMenu", "common_medal", g_UiManager->m_PosX + (g_UiManager->m_Width / g_UiManager->m_OptionPadding - 0.01f), 
						g_UiManager->m_DrawBaseY + (g_UiManager->m_OptionHeight / 2.f) - (g_UiManager->GetTextHeight(g_UiManager->m_OptionFont, g_UiManager->m_OptionTextSize) / 1.5f) + 0.015f, 
						0.020f, 0.030f, 0.0, 255, 255, 255, 255, 0);
			}
		}

		m_DrawBaseY += m_OptionHeight;
	}

	void UIManager::DrawFooter()
	{
		DrawRect(
			m_PosX,
			m_DrawBaseY + (m_FooterHeight / 2.f),
			m_Width,
			m_FooterHeight,
			m_FooterBackgroundColor);
		
		float size = m_FooterSpriteSize;
		float rotation = 0.f;
		const char* texture = "shop_arrows_upanddown";

		if (!m_SubmenuStack.empty())
		{
			auto sub = m_SubmenuStack.top();
			if (sub->GetSelectedOption() == 0)
			{
				rotation = 90.f;
				texture = "arrowright";
				size *= 0.8f;
			}
			else if (sub->GetSelectedOption() + 1 == sub->GetNumOptions())
			{
				rotation = 270.f;
				texture = "arrowright";
				size *= 0.8f;
			}
		}

		auto sizee = GetSpriteScale(size);

		DrawSprite(
			"commonmenu",
			texture,
			m_PosX,
			m_DrawBaseY + (m_FooterHeight / 2.f),
			sizee.x,
			sizee.y,
			m_FooterSpriteColor,
			rotation);

		m_DrawBaseY += m_FooterHeight;
	}

	void UIManager::DrawDescription()
	{
		const char* description{};

		if (!m_SubmenuStack.empty())
		{
			auto sub = m_SubmenuStack.top();
			if (sub->GetNumOptions())
			{
				if (auto opt = sub->GetOption(sub->GetSelectedOption()))
				{
					description = opt->GetDescription();
				}
			}
		}

		if (!description || !*description)
			return;

		m_DrawBaseY += m_DescriptionHeightPadding;

		DrawRect(
			m_PosX,
			m_DrawBaseY + (m_DescriptionHeight / 2.f),
			m_Width,
			m_DescriptionHeight,
			m_DescriptionBackgroundColor);

		auto spriteSize = GetSpriteScale(m_DescriptionSpriteSize);
		DrawSprite(
			"shared", "info_icon_32",
			m_PosX - (m_Width / m_DescriptionPadding) + (spriteSize.x / 2.f),
			m_DrawBaseY + (m_DescriptionHeight / 2.f),
			spriteSize.x,
			spriteSize.y,
			m_DescriptionSpriteColor,
			0.f
		);

		DrawLeftText(
			description,
			m_PosX - (m_Width / m_DescriptionPadding) + (spriteSize.x * 1.15f),
			m_DrawBaseY + (m_DescriptionHeight / 2.f) - (GetTextHeight(m_DescriptionFont, m_DescriptionTextSize) / 1.5f),
			m_DescriptionTextSize,
			m_DescriptionFont,
			m_DescriptionTextColor,
			false, false
		);

		m_DrawBaseY += m_DescriptionHeight;
	}

	int m_InstructionsContainer{};
	int m_InstructionsCount{};

	void UIManager::InitInstructions()
	{
		m_InstructionsCount = 0;
		m_InstructionsContainer = GRAPHICS::REQUEST_SCALEFORM_MOVIE("instructional_buttons");
		GRAPHICS::DRAW_SCALEFORM_MOVIE_FULLSCREEN(m_InstructionsContainer, 255, 255, 255, 0, 0);
		GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(m_InstructionsContainer, "CLEAR_ALL");
		GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
		GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(m_InstructionsContainer, "SET_CLEAR_SPACE");
		GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_INT(200);
		GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
	}

	void UIManager::AddInstructions(const char* instruction, int button)
	{
		GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(m_InstructionsContainer, "SET_DATA_SLOT");
		GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_INT(m_InstructionsCount);
		GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_INT(button);
		GRAPHICS::BEGIN_TEXT_COMMAND_SCALEFORM_STRING("STRING");
		HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(instruction);
		GRAPHICS::END_TEXT_COMMAND_SCALEFORM_STRING();
		GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
		m_InstructionsCount++;
	}

	void UIManager::EndInstructions()
	{
		GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(m_InstructionsContainer, "DRAW_INSTRUCTIONAL_BUTTONS");
		GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
		GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(m_InstructionsContainer, "SET_BACKGROUND_COLOUR");
		GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_INT(0);
		GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_INT(0);
		GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_INT(0);
		GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_INT(80);
		GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
	}

	void UIManager::DrawInstructions() {
		InitInstructions();
		AddInstructions("Back", 136);
		AddInstructions("Select", 141);
		AddInstructions("Left/Right", 46);
		AddInstructions("Up/Down", 10);
		EndInstructions();
	}

	void UIManager::DrawRect(float x, float y, float width, float height, Color color)
	{
		GRAPHICS::DRAW_RECT(x, y, width, height, color.r, color.g, color.b, color.a, 0);
	}

	void UIManager::DrawSprite(const char* dict, const char* texture, float x, float y, float width, float height, Color color, float rotation)
	{
		if (GRAPHICS::HAS_STREAMED_TEXTURE_DICT_LOADED(dict))
		{
			GRAPHICS::DRAW_SPRITE(dict, texture, x, y, width, height, rotation, color.r, color.g, color.b, color.a, 0);
		}
		else
		{
			GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT(dict, false);
		}
	}

	void UIManager::DrawTextInfo(std::string text, RGBAF rgbaf, Vector2 position, Vector2 size, bool center, bool right)
	{
		HUD::SET_TEXT_RIGHT_JUSTIFY(right);
		HUD::SET_TEXT_CENTRE(center);
		HUD::SET_TEXT_COLOUR(rgbaf.r, rgbaf.g, rgbaf.b, rgbaf.a);
		HUD::SET_TEXT_FONT(rgbaf.f);
		HUD::SET_TEXT_SCALE(size.x, size.y);
		HUD::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
		HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text.data());
		HUD::END_TEXT_COMMAND_DISPLAY_TEXT(position.x, position.y, 0);
	}

	void UIManager::DrawLeftText(const char* text, float x, float y, float size, Font font, Color color, bool outline, bool shadow)
	{
		HUD::SET_TEXT_SCALE(size, size);
		HUD::SET_TEXT_FONT(static_cast<int>(font));
		HUD::SET_TEXT_COLOUR(color.r, color.g, color.b, color.a);
		if (outline)
			HUD::SET_TEXT_OUTLINE();
		if (shadow)
			HUD::SET_TEXT_DROP_SHADOW();

		HUD::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
		HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
		HUD::END_TEXT_COMMAND_DISPLAY_TEXT(x, y, 0);
	}

	void UIManager::DrawCenteredText(const char* text, float x, float y, float size, Font font, Color color, bool outline, bool shadow)
	{
		HUD::SET_TEXT_CENTRE(true);
		DrawLeftText(text, x, y, size, font, color, outline, shadow);
	}

	void UIManager::DrawRightText(const char* text, float x, float y, float size, Font font, Color color, bool outline, bool shadow)
	{
		HUD::SET_TEXT_WRAP(0.f, x);
		HUD::SET_TEXT_RIGHT_JUSTIFY(true);
		DrawLeftText(text, x, y, size, font, color, outline, shadow);
	}

	float x_pos{};
	float current_y_pos{};

	void UIManager::DrawInfoTitle(std::string title)
	{
		if (m_PosX > 0.482f)
			x_pos = m_PosX - 0.2f;
		else
			x_pos = m_PosX + 0.21f;

		DrawSprite("commonmenu", "gradient_nav", x_pos, m_PosY + 0.05f, 0.17f, 0.1f, { 33, 29, 33, 255 }, 0.f);
		DrawTextInfo(title, { 255, 255, 255, 255, 1 }, { x_pos, m_PosY + 0.025f }, Utils::getSpriteScale(0.85f), true);

		current_y_pos = m_PosY + 0.0492f;
	}

	void UIManager::AddInfo(std::string left_text, std::string right_text)
	{
		DrawRect(x_pos, current_y_pos + 0.072f, 0.17f, 0.030f, { 0, 0, 0, 100 });

		DrawTextInfo(left_text + ":", Colors::White, { x_pos - 0.08f, current_y_pos + 0.06f }, Utils::getSpriteScale(0.25f));

		HUD::SET_TEXT_WRAP(0.f, x_pos + 0.08f);

		DrawTextInfo(right_text, Colors::White, { x_pos + 0.08f, current_y_pos + 0.06f }, Utils::getSpriteScale(0.25f), false, true);

		current_y_pos += 0.030f;
	}

	void UIManager::AddInfoSprite(std::string left_text, const char* sprite)
	{
		DrawRect(x_pos, current_y_pos + 0.068f, 0.17f, 0.035f, { 0, 0, 0, 100 });

		DrawTextInfo(left_text + ":", Colors::White, { x_pos - 0.08f, current_y_pos + 0.0605f }, Utils::getSpriteScale(0.25f));

		HUD::SET_TEXT_WRAP(0.f, x_pos + 0.08f);

		DrawSprite(sprite, sprite, x_pos + 0.07f, current_y_pos + 0.070f, 0.018f, 0.025f, { 255, 255, 255, 255 }, 0.f);

		current_y_pos += 0.028f;
	}

	void UIManager::AddSingleInfo(std::string left_text)
	{
		DrawRect(x_pos, current_y_pos + 0.018f, 0.17f, 0.029f, { 0, 0, 0, 100 });

		DrawTextInfo(left_text, Colors::White, { x_pos - 0.08f, current_y_pos }, Utils::getSpriteScale(0.25f));

		current_y_pos += 0.035f;
	}

	void UIManager::AddEmptyInfo()
	{
		DrawRect(x_pos, current_y_pos + 0.018f, 0.17f, 0.035f, { 0, 0, 0, 100 });
		current_y_pos += 0.035f;
	}

	void UIManager::DrawPlayerInfo(Player p)
	{
		Ped ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(p);

		//  169, 169, 169, 255, 0 
		bool isActiveSession = NETWORK::NETWORK_IS_SESSION_ACTIVE();

		// Display Semi-Transparent Box
		// Postive Value = Down | Negative Value = Up
		float PositionX = m_PosX + 0.252f;
		float PositionY = m_PosY + 0.213f;

		if (m_PosX >= 0.65f)
		{
			PositionX = m_PosX - 0.252f;
		}


		float TextPosY = m_PosY - 0.05;

		float LinePosY = m_PosY - 0.05;

		float leftTextX = PositionX - 0.101f;
		float rightTextX = PositionX + 0.007f;
		float lineSeperatorX = PositionX + 0.003f;

		RGBA Background = { 0, 0, 0, 190 };
		RGBA White = { 255, 255, 255, 255 };

		RGBAF White1 = { 255, 255, 255, 255, 1 };
		RGBAF White2 = { 255, 255, 255, 255, 0 };

		// {RGBA}, {pos x, pos y}, {width, height}
		// Position X ( Postive (+) = Right | Negative (-) = Left)
		// Position Y ( Negative (-) = UP | Positive (+) = Down )


		// Background (Black with 190% Alpha Opacity)
		Rect({ Background }, { PositionX, PositionY }, { 0.215f, 0.425f });

		// From Role - Rank
		Rect({ White }, { lineSeperatorX, LinePosY + 0.138f }, { 0.001f, 0.066f });

		// Ammo Seperator
		Rect({ White }, { lineSeperatorX, LinePosY + 0.212f }, { 0.001f, 0.020f });

		// Distance - Moddel Model
		Rect({ White }, { lineSeperatorX, LinePosY + 0.338f }, { 0.001f, 0.120f });

		NativeVector3 playerCoords = ENTITY::GET_ENTITY_COORDS(ped, false);
		NativeVector3 selfCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), false);

		//char KDRatio[20], playerVehicle[100], Zone[100], Distance[100], CurrentWeapon[100], Username[50], Role[50], Wallet[50], Level[3], CoordX[100], CoordZ[100], CoordY[100], Street[100], Speed[20], WantedLevel[10], Health[10], Armor[10];
		auto playerAddress = g_GameFunctions->m_GetPlayerAddress(p);
		auto playerInfo = *reinterpret_cast<CPlayerInfo**>(playerAddress + 0x10B8);

		int64_t money = globalHandle(1590446).At(p, 871).At(211).At(3).As<int>();
		int64_t earnings = globalHandle(1590446).At(p, 871).At(211).At(56).As<int>();
		int64_t bank = earnings - money;


		Hash street[2]{};
		PATHFIND::GET_STREET_NAME_AT_COORD(playerCoords.x, playerCoords.y, playerCoords.z, &street[0], &street[1]);

		int netHandle[13];
		NETWORK::NETWORK_HANDLE_FROM_PLAYER(p, netHandle, 13);
		const char* playerSCID = NETWORK::NETWORK_MEMBER_ID_FROM_GAMER_HANDLE(&netHandle[0]);

		float distance = Utils::get_distance(&selfCoords, &playerCoords);

		float newDistance{};

		distance > 1000 ? newDistance = round((distance / 1000) * 100) / 100 : newDistance = round(distance * 1000) / 100;

#pragma region clientData

		std::string KD = isActiveSession ? Functions::getPlayerKDRatio(p) : "N/A";
		std::string playerVehicle = Functions::getPlayerVehicle(ped);
		std::string CurrentWeapon = Functions::getPlayerWeapon(ped);
		std::string Username = PLAYER::GET_PLAYER_NAME(p);
		std::string Role = Functions::getRole(p);
		std::string Wallet = isActiveSession ? fmt::format("${0}", money) : "N/A";
		std::string Level = isActiveSession ? std::to_string(Functions::getPlayerLevel(p)) : "N/A";
		std::string CoordX = std::to_string(round((double)playerCoords.x));
		std::string CoordY = std::to_string(round((double)playerCoords.y));
		std::string CoordZ = std::to_string(round((double)playerCoords.z));
		std::string Health = fmt::format("{0}%", std::to_string(Functions::getPlayerHealth(ped)));
		std::string Armor = fmt::format("{0}%", std::to_string(Functions::getPlayerArmor(ped, p)));
		std::string playerIP = playerInfo ? fmt::format("{0}.{1}.{2}.{3}", playerInfo->ExternalIPNibble1, playerInfo->ExternalIPNibble2, playerInfo->ExternalIPNibble3, playerInfo->ExternalIPNibble4) : "N/A";
		std::string Speed = std::to_string(Functions::getPlayerSpeed(ped));
		std::string WantedLevel = Functions::getPlayerWantedLevel(p);
		std::string Street = HUD::GET_STREET_NAME_FROM_HASH_KEY(street[0]);
		std::string Zone = HUD::_GET_LABEL_TEXT(ZONE::GET_NAME_OF_ZONE(playerCoords.x, playerCoords.y, playerCoords.z));
		std::string Distance = fmt::format("{0} {1}", roundf(newDistance), roundf(distance) > 1000 ? "Kilometers" : "Meters");
		std::string RID = isActiveSession ? fmt::format("{0}", playerSCID) : "N/A";
		std::string Bank = isActiveSession ? fmt::format("${0}", bank) : "N/A";
#pragma endregion

		// {RGBA}, {pos x, pos y}, {width, height}
		// Position X ( Postive (+) = Right | Negative (-) = Left)
		// Position Y ( Negative (-) = UP | Positive (+) = Down )

		Text("Client -", { White1 }, { leftTextX, TextPosY + 0.06f }, { 0.55f, 0.55f }, false);
		Text(Username.c_str(), { White1 }, { leftTextX + 0.045f, TextPosY + 0.06f }, { 0.55f, 0.55f }, false);

		// Role
		Text("Role", { White2 }, { leftTextX, TextPosY + 0.10f }, { 0.30f, 0.30f }, false);
		TextRight(Role.c_str(), { White2 }, { leftTextX, TextPosY + 0.10f }, { 0.30f, 0.30f }, 1);

		// Wallet Cash Value
		Text("Wallet", { White2 }, { leftTextX, TextPosY + 0.125f }, { 0.30f, 0.30f }, false);
		TextRight(Wallet.c_str(), { White2 }, { leftTextX, TextPosY + 0.125f }, { 0.30f, 0.30f }, 1);

		// Rank / Level
		Text("Bank", { White2 }, { leftTextX, TextPosY + 0.15f }, { 0.30f, 0.30f }, false);
		TextRight(Bank.c_str(), { White2 }, { leftTextX, TextPosY + 0.15f }, { 0.30f, 0.30f }, 1);

		// Weapon
		Text("Weapon", { White2 }, { leftTextX, TextPosY + 0.175f }, { 0.30f, 0.30f }, false);
		TextRight(CurrentWeapon.c_str(), { White2 }, { leftTextX, TextPosY + 0.175f }, { 0.30f, 0.30f }, 2);

		// Ammo
		Text("Rank", { White2 }, { leftTextX, TextPosY + 0.200f }, { 0.30f, 0.30f }, false);
		TextRight(Level.c_str(), { White2 }, { leftTextX, TextPosY + 0.200f }, { 0.30f, 0.30f }, 1);

		// Zone
		Text("Zone", { White2 }, { leftTextX, TextPosY + 0.225f }, { 0.30f, 0.30f }, false);
		TextRight(Zone.c_str(), { White2 }, { leftTextX, TextPosY + 0.225f }, { 0.30f, 0.30f }, 2);

		// Street
		Text("Street", { White2 }, { leftTextX, TextPosY + 0.25f }, { 0.30f, 0.30f }, false);
		TextRight(Street.c_str(), { White2 }, { leftTextX, TextPosY + 0.25f }, { 0.30f, 0.30f }, 2);


		// Distance
		Text("Distance", { White2 }, { leftTextX, TextPosY + 0.275f }, { 0.30f, 0.30f }, false);
		TextRight(Distance.c_str(), { White2 }, { leftTextX, TextPosY + 0.275f }, { 0.30f, 0.30f }, 1);

		// X
		Text("X", { White2 }, { leftTextX, TextPosY + 0.3f }, { 0.30f, 0.30f }, false);
		TextRight(CoordX.c_str(), { White2 }, { leftTextX, TextPosY + 0.3f }, { 0.30f, 0.30f }, 1);

		// Z
		Text("Z", { White2 }, { leftTextX, TextPosY + 0.325f }, { 0.30f, 0.30f }, false);
		TextRight(CoordZ.c_str(), { White2 }, { leftTextX, TextPosY + 0.325f }, { 0.30f, 0.30f }, 1);

		// Office Garages
		Text("IP", { White2 }, { leftTextX, TextPosY + 0.350f }, { 0.30f, 0.30f }, false);
		TextRight(playerIP.c_str(), { White2 }, { leftTextX, TextPosY + 0.350f }, { 0.30f, 0.30f }, 1);

		// Modded Model
		Text("Modded Model", { White2 }, { leftTextX, TextPosY + 0.375f }, { 0.30f, 0.30f }, false);
		TextRight("No", { White2 }, { leftTextX, TextPosY + 0.375f }, { 0.30f, 0.30f }, 1);

		// Vehicle
		Text("Vehicle", { White2 }, { leftTextX, TextPosY + 0.4f }, { 0.30f, 0.30f }, false);
		TextRight(playerVehicle.c_str(), { White2 }, { leftTextX, TextPosY + 0.4f }, { 0.30f, 0.30f }, 2);

		// R* ID
		Text("Rockstar ID", { White2 }, { leftTextX, TextPosY + 0.425f }, { 0.30f, 0.30f }, false);
		TextRight(RID.c_str(), { White2 }, { leftTextX, TextPosY + 0.425f }, { 0.30f, 0.30f }, 2);
#pragma endregion

#pragma region RightSide

		// Health
		Text("Health", { White2 }, { rightTextX, TextPosY + 0.10f }, { 0.30f, 0.30f }, false);
		TextRight(Health.c_str(), { White2 }, { rightTextX, TextPosY + 0.10f }, { 0.30f, 0.30f }, 2);

		// Armor
		Text("Armor", { White2 }, { rightTextX, TextPosY + 0.125f }, { 0.30f, 0.30f }, false);
		TextRight(Armor.c_str(), { White2 }, { rightTextX, TextPosY + 0.125f }, { 0.30f, 0.30f }, 2);

		// Wanted Level
		Text("Wanted Level", { White2 }, { rightTextX, TextPosY + 0.15f }, { 0.30f, 0.30f }, false);
		TextRight(WantedLevel.c_str(), { White2 }, { rightTextX, TextPosY + 0.15f }, { 0.30f, 0.30f }, 2);

		// KD Ratio
		Text("K/D Ratio", { White2 }, { rightTextX, TextPosY + 0.200f }, { 0.30f, 0.30f }, false);
		TextRight(KD.c_str(), { White2 }, { rightTextX, TextPosY + 0.200f }, { 0.30f, 0.30f }, 2);

		// Speed (MPH)
		Text("Speed (MPH)", { White2 }, { rightTextX, TextPosY + 0.275f }, { 0.30f, 0.30f }, false);
		TextRight(Speed.c_str(), { White2 }, { rightTextX, TextPosY + 0.275f }, { 0.30f, 0.30f }, 2);

		// Y
		Text("Y", { White2 }, { rightTextX, TextPosY + 0.30f }, { 0.30f, 0.30f }, false);
		TextRight(CoordY.c_str(), { White2 }, { rightTextX, TextPosY + 0.300f }, { 0.30f, 0.30f }, 2);

		// Heading
		Text("Heading", { White2 }, { rightTextX, TextPosY + 0.325f }, { 0.30f, 0.30f }, false);
		TextRight("E (179)", { White2 }, { rightTextX, TextPosY + 0.325f }, { 0.30f, 0.30f }, 2);

		// IP
		Text("IP", { White2 }, { rightTextX, TextPosY + 0.350f }, { 0.30f, 0.30f }, false);
		TextRight(playerIP.c_str(), { White2 }, { rightTextX, TextPosY + 0.350f }, { 0.30f, 0.30f }, 2);

		// Off The Radar
		Text("Off The Radar", { White2 }, { rightTextX, TextPosY + 0.375f }, { 0.30f, 0.30f }, false);
		TextRight("No", { White2 }, { rightTextX, TextPosY + 0.375f }, { 0.30f, 0.30f }, 2);
#pragma endregion
	}

	float UIManager::GetTextHeight(Font font, float size)
	{
		return HUD::_GET_TEXT_SCALE_HEIGHT(size, static_cast<int>(font));
	}

	void UIManager::Rect(RGBA rgba, Vector2 position, Vector2 size)
	{
		GRAPHICS::DRAW_RECT(position.x, position.y, size.x, size.y, rgba.r, rgba.g, rgba.b, rgba.a, 0);
	}
	void UIManager::Text(const char* text, RGBAF rgbaf, Vector2 position, Vector2 size, bool center)
	{
		HUD::SET_TEXT_CENTRE(center);
		HUD::SET_TEXT_COLOUR(rgbaf.r, rgbaf.g, rgbaf.b, rgbaf.a);
		HUD::SET_TEXT_FONT(rgbaf.f);
		HUD::SET_TEXT_SCALE(size.x, size.y);
		HUD::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
		HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME((char*)text);
		HUD::END_TEXT_COMMAND_DISPLAY_TEXT(position.x, position.y, 0);
	}
	void UIManager::TextRight(const char* text, RGBAF rgbaf, Vector2 position, Vector2 size, int type)
	{
		float PositionX = m_PosX + 0.242f;

		if (m_PosX >= 0.65f)
		{
			PositionX = m_PosX - 0.242f;
		}

		switch (type)
		{
		case 1:
			// Left Hand Side Values

			HUD::SET_TEXT_WRAP(0.0f, PositionX - 0.01f);
			break;

		case 2:
			// Full Width Values
			HUD::SET_TEXT_WRAP(0.0f, PositionX + 0.092f);
			break;
		}


		HUD::SET_TEXT_RIGHT_JUSTIFY(true);
		HUD::SET_TEXT_COLOUR(rgbaf.r, rgbaf.g, rgbaf.b, rgbaf.a);
		HUD::SET_TEXT_FONT(rgbaf.f);
		HUD::SET_TEXT_SCALE(size.x, size.y);
		HUD::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
		HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME((char*)text);
		HUD::END_TEXT_COMMAND_DISPLAY_TEXT(position.x, position.y, 0);
	}


	Vector2 UIManager::GetSpriteScale(float size)
	{
		int x;
		int y;
		GRAPHICS::_GET_ACTIVE_SCREEN_RESOLUTION(&x, &y);

		return { (static_cast<float>(y) / static_cast<float>(x)) * size, size };
	}

	bool UIManager::IsMouseLocked()
	{
		return m_Opened && m_MouseLocked;
	}

	Rectangle UIManager::GetMenuRect()
	{
		float height = m_HeaderHeight;
		height += m_SubmenuBarHeight;

		if (!m_SubmenuStack.empty())
		{
			height += m_OptionHeight * std::min(m_SubmenuStack.top()->GetNumOptions(), m_OptionsPerPage);
		}

		height += m_FooterHeight;

		return
		{
			{ m_PosX + (m_Width / 2.f), m_PosY + (height / 2.f) },
			{ m_Width, height }
		};
	}

	Vector2 UIManager::GetMousePos()
	{
		POINT point;
		GetCursorPos(&point);

		return Vector2{ static_cast<float>(point.x), static_cast<float>(point.y) };
	}
	void UIManager::Speedometer(const char* text) {
		RGBAF toDraw = { 255, 255, 255, 255, 7 };
		Text(text, toDraw, { 0.84f, 0.8800f }, { 0.70f, 0.70f }, false);
	}
}
