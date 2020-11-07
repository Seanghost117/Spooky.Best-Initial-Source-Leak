#pragma once
#include "../Common.hpp"
#include "../Types.hpp"
#include "TextBox.hpp"
#include "AbstractSubmenu.hpp"

namespace Mystic::UserInterface
{
	enum class Font : std::int32_t
	{
		ChaletLondon = 0,
		HouseScript = 1,
		Monospace = 2,
		Wingdings = 3,
		ChaletComprimeCologne = 4,
		Pricedown = 7
	};

	enum class HeaderType : std::int32_t
	{
		Static = 0,
		Gradient = 1,
		YTD = 2
	};

	struct Rectangle
	{
		Vector2 m_Center;
		Vector2 m_Size;

		bool IsInBounds(Vector2 point)
		{
			auto left = m_Center.x - (m_Size.x / 2.f);
			auto right = m_Center.x + (m_Size.x / 2.f);
			auto top = m_Center.y + (m_Size.y / 2.f);
			auto bottom = m_Center.y - (m_Size.y / 2.f);
			
			if (point.x > right || point.x < left)
				return false;
			if (point.y > top || point.y < bottom)
				return false;

			return true;
		}
	};

	class UIManager
	{
	public:
		explicit UIManager() = default;
		~UIManager() noexcept = default;
		UIManager(UIManager const&) = delete;
		UIManager(UIManager&&) = delete;
		UIManager& operator=(UIManager const&) = delete;
		UIManager& operator=(UIManager&&) = delete;
	
		template <typename SubmenuType, typename ...TArgs>
		void AddSubmenu(TArgs&&... args)
		{
			auto sub = std::make_unique<SubmenuType>(std::forward<TArgs>(args)...);
			if (m_SubmenuStack.empty())
			{
				m_SubmenuStack.push(sub.get());
			}

			m_AllSubmenus.push_back(std::move(sub));
		}

		void SwitchToSubmenu(std::uint32_t id)
		{
			for (auto&& sub : m_AllSubmenus)
			{
				if (sub->GetId() == id)
				{
					m_SubmenuStack.push(sub.get());
					return;
				}
			}
		}

		void OnTick();
	public:
		std::mutex m_Mutex;

		bool m_Opened = false;
		bool m_MouseLocked = false;
		float m_PosX = 0.87f;
		float m_PosY = 0.10f;
		float m_Width = 0.22f;
		std::size_t m_OptionsPerPage = 11;
		bool m_Sounds = true;
		
		// Input
		std::int32_t m_OpenDelay = 200;
		std::int32_t m_BackDelay = 300;
		std::int32_t m_EnterDelay = 300;
		std::int32_t m_VerticalDelay = 120;
		std::int32_t m_HorizontalDelay = 120;

		//keys
		std::string m_OpenKey;
		std::string m_BackKey;
		std::string m_EnterKey;
		std::string m_UpKey;
		std::string m_DownKey;
		std::string m_LeftKey;
		std::string m_RightKey;
		std::string m_Controller;


		// Header
		float m_HeaderHeight = 0.1f;
		bool m_HeaderText = true;
		float m_HeaderTextSize = 1.f;
		Font m_HeaderFont = Font::HouseScript;
		Color m_HeaderBackgroundColor{ 2, 130, 53, 255 };
		Color m_HeaderTextColor{ 255, 255, 255, 255 };
		HeaderType m_HeaderType = HeaderType::YTD;
		Color m_YTDHeaderBackgroundColor{ 10, 15, 220, 255 };

		bool m_HeaderGradientTransparent = false;
		bool m_HeaderGradientFlip = false;
		float m_HeaderGradientStretch = 0.f;
		float m_HeaderGradientFiller = 0.f;
		Color m_HeaderGradientColorLeft{ 182, 17, 247, 255 };
		Color m_HeaderGradientColorRight{ 92, 38, 252, 255 };
		//Color m_HeaderGradientColorLeft{ 66, 244, 179, 255 };
		//Color m_HeaderGradientColorRight{ 65, 244, 244, 255 };

		// Submenu bar
		float m_SubmenuBarHeight = 0.04f;
		float m_SubmenuBarTextSize = 0.34f;
		Font m_SubmenuBarFont = Font::ChaletLondon;
		float m_SubmenuBarPadding = 2.1f;
		Color m_SubmenuBarBackgroundColor{ 0, 0, 0, 220 };
		Color m_SubmenuBarTextColor{ 255, 255, 255, 255 };

		// Options
		float m_OptionHeight = 0.04f;
		float m_OptionTextSize = 0.30f;
		Font m_OptionFont = Font::ChaletLondon;
		float m_OptionPadding = 2.1f;
		Color m_OptionSelectedTextColor{ 10, 10, 10, 255 };
		Color m_OptionUnselectedTextColor{ 255, 255, 255, 255 };
		Color m_OptionSelectedBackgroundColor{ 255, 255, 255, 255 };
		Color m_OptionUnselectedBackgroundColor{ 0, 0, 0, 160 };

		// Footer
		float m_FooterHeight = 0.04f;
		float m_FooterSpriteSize = 0.025f;
		Color m_FooterBackgroundColor{ 10, 10, 10, 200 };
		Color m_FooterSpriteColor{ 255, 255, 255, 255 };

		// Description
		float m_DescriptionHeightPadding = 0.01f;
		float m_DescriptionHeight = 0.033f;
		float m_DescriptionTextSize = 0.28f;
		Font m_DescriptionFont = Font::ChaletLondon;
		Color m_DescriptionBackgroundColor{ 0, 0, 0, 120 };
		Color m_DescriptionTextColor{ 255, 255, 255, 255 };
		float m_DescriptionSpriteSize = 0.025f;
		float m_DescriptionPadding = 2.1f;
		Color m_DescriptionSpriteColor{ 255, 255, 255, 255 };

		void DrawSprite(const char* dict, const char* texture, float x, float y, float width, float height, Color color, float rotation);
		void DrawPlayerInfo(Player player);
		void DrawInstructions();
		void DrawTextInfo(std::string text, RGBAF rgbaf, Vector2 position, Vector2 size, bool center = false, bool right = false);
		void ResetInput();
		void Speedometer(const char* text);
	private:
		bool m_OpenKeyPressed = false;
		bool m_BackKeyPressed = false;
		bool m_EnterKeyPressed = false;
		bool m_UpKeyPressed = false;
		bool m_DownKeyPressed = false;
		bool m_LeftKeyPressed = false;
		bool m_RightKeyPressed = false;
		void CheckForInput();
		void HandleInput();
		

		float m_DrawBaseY{};
		void DrawHeader();
		void DrawSubmenuBar(AbstractSubmenu* sub);
		void DrawOption(AbstractOption* opt, bool selected);
		void DrawFooter();
		void DrawInfoTitle(std::string title);
		void AddInfoSprite(std::string left_text, const char* sprite);
		void AddInfo(std::string left_text, std::string right_text);
		void AddSingleInfo(std::string left_text);
		void AddEmptyInfo();
		void DrawDescription();
		void InitInstructions();
		void AddInstructions(const char* instruction, int button);
		void EndInstructions();

		void DrawRect(float x, float y, float width, float height, Color color);
		void DrawLeftText(const char* text, float x, float y, float size, Font font, Color color, bool outline, bool shadow);
		void DrawCenteredText(const char* text, float x, float y, float size, Font font, Color color, bool outline, bool shadow);
		void DrawRightText(const char* text, float x, float y, float size, Font font, Color color, bool outline, bool shadow);
		float GetTextHeight(Font font, float size);
		Vector2 GetSpriteScale(float size);

		void Rect(RGBA rgba, Vector2 position, Vector2 size);
		void Text(const char* text, RGBAF rgbaf, Vector2 position, Vector2 size, bool center);
		void TextRight(const char* text, RGBAF rgbaf, Vector2 position, Vector2 size, int type);

		bool IsMouseLocked();
		Rectangle GetMenuRect();
		Vector2 GetMousePos();
		std::vector<std::unique_ptr<AbstractSubmenu>> m_AllSubmenus;
		std::stack<AbstractSubmenu*, std::vector<AbstractSubmenu*>> m_SubmenuStack;
	};
}

namespace Mystic
{
	inline std::unique_ptr<UserInterface::UIManager> g_UiManager;
}
