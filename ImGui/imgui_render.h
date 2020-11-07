#pragma once
#include "imgui.h"
#include "../common.hpp"
#include "vector.h"

enum text_flags_t
{
	NO_FLAGS = 0,
	CENTERED_X = 1,
	CENTERED_Y = 2,
	OUTLINED = 4,
	DROPSHADOW = 8,
};

class imgui_render
{
public:
	static bool init();
	static void reset();
	static ImFont* font(int index);
	static void add_line(const ImVec2& a, const ImVec2& b, ImU32 col, float thickness = 1.f);
	static void add_rect(const ImVec2& a, const ImVec2& b, ImU32 col, float thickness = 1.f, float rounding = 0.f,
	                     int rounding_corners_flags = ImDrawCornerFlags_All);
	static void add_rect_filled(const ImVec2& a, const ImVec2& b, ImU32 col, float rounding = 0.f,
	                            int rounding_corners_flags = ImDrawCornerFlags_All);
	static void add_circle(const ImVec2& center, float radius, ImU32 col, int segments = 12);
	static void add_circle3d(const vector& centre, float points, float radius, float thickness, ImU32 color);
	static void add_text(const ImFont* font, const ImU32& color, const ImU32& shadow_color, const ImVec2& pos, int flags, const char* format, ...);
private:
	inline static ImDrawList* _drawlist;
	inline static ImFont* _font_default;
	//inline static IDirect3DTexture9* _background;
};
