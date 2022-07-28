#pragma once
#include "../GUIWidget.h"

// A simple one-line only text field
class GUITextField : public GUIWidget
{
private:
	bool focused;
	float last_offset;
public:

	std::string string;
	std::string default_string;
	float ft_size;
	std::string ft_font;
	bool uses_bitmap;

	int cursor_pos;
	std::vector<NVGglyphPosition> glyph_pos;

	virtual glm::ivec2 prepare(glm::ivec2 wpos, glm::ivec2 wsize, glm::ivec4 viewport, GUIInput* gui_input) override;
	virtual void draw(NVGcontext* ctx, GUISkin* skin) override;

	GUITextField(std::string font = "regular", float size = 16.0f);
};