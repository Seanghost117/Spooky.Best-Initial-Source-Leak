#include "imgui_render.h"
#include "imgui_internal.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "../common.hpp"
#define MAX_BUFFER_SIZE 100

ImFont* imgui_render::font(int index)
{
	return _font_default;
}

void imgui_render::add_line(const ImVec2& a, const ImVec2& b, ImU32 col, float thickness)
{
	_drawlist->AddLine(a, b, col, thickness);
}

void imgui_render::add_rect(const ImVec2& a, const ImVec2& b, ImU32 col, float thickness, float rounding, int rounding_corners_flags)
{
	_drawlist->AddRect(a, b, col, rounding, rounding_corners_flags, thickness);
}

void imgui_render::add_rect_filled(const ImVec2& a, const ImVec2& b, ImU32 col, float rounding, int rounding_corners_flags)
{
	_drawlist->AddRectFilled(a, b, col, rounding, rounding_corners_flags);
}

void imgui_render::add_circle(const ImVec2& center, float radius, ImU32 col, int segments)
{
	_drawlist->AddCircle(center, radius, col, segments);
}



void imgui_render::add_text(const ImFont* font, const ImU32& color, const ImU32& shadow_color, const ImVec2& pos, int flags, const char* format, ...)
{
	static char buffer[MAX_BUFFER_SIZE];

	va_list va;
	va_start(va, format);
	const int str_len = std::vsnprintf(buffer, MAX_BUFFER_SIZE, format, va);
	va_end(va);

	float x = pos.x;
	float y = pos.y;
	const auto size = font->CalcTextSizeA(font->FontSize, FLT_MAX, 0.0f, buffer);

	if (flags & CENTERED_X || flags & CENTERED_Y) {

		if (flags & CENTERED_X)
			x -= size.x / 2.f;

		if (flags & CENTERED_Y)
			y -= size.y / 2.f;
	}

	if (flags & DROPSHADOW)
		_drawlist->AddText(font, font->FontSize, ImVec2(x + 1, y + 1), shadow_color, buffer);

	if (flags & OUTLINED) {
		_drawlist->AddText(font, font->FontSize, ImVec2(x, y + 1), shadow_color, buffer);
		_drawlist->AddText(font, font->FontSize, ImVec2(x, y - 1), shadow_color, buffer);
		_drawlist->AddText(font, font->FontSize, ImVec2(x + 1, y), shadow_color, buffer);
		_drawlist->AddText(font, font->FontSize, ImVec2(x - 1, y), shadow_color, buffer);
	}
	_drawlist->AddText(font, font->FontSize, ImVec2(x, y), color, buffer);
}