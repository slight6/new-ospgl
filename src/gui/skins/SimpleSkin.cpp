#include "SimpleSkin.h"
#include "../GUIWindow.h"

void SimpleSkin::draw_button(NVGcontext* v, glm::ivec2 pos, glm::ivec2 size, const std::string& text,
	ButtonState state, ButtonStyle style)
{
	
	nvgBeginPath(v);	

	int bevel_pixels = 10;
	
	if(size.x <= 32 || size.y <= 32)
	{
		// Fallback to a rectangle, bevel looks wrong
		nvgRect(v, pos.x - 0.5f, pos.y - 0.5f, size.x + 1.0f, size.y + 1.0f);	
	}
	else
	{
	
		if(style == ButtonStyle::NORMAL)
		{
			nvgMoveTo(v, pos.x + 0.5f, pos.y + size.y + 0.5f);
			nvgLineTo(v, pos.x + size.x - bevel_pixels + 0.5f, pos.y + size.y + 0.5f);
			nvgLineTo(v, pos.x + size.x + 0.5f, pos.y + size.y - bevel_pixels + 0.5f);
			nvgLineTo(v, pos.x + size.x + 0.5f, pos.y + 0.5f);
			nvgLineTo(v, pos.x + bevel_pixels + 0.5f, pos.y + 0.5f);
			nvgLineTo(v, pos.x + 0.5f, pos.y + bevel_pixels + 0.5f);
			nvgLineTo(v, pos.x + 0.5f, pos.y + size.y + 0.5f);	
		}
		else if(style == ButtonStyle::SYMMETRIC)
		{
			// Similar to before but with "bevel" on all borders
			nvgMoveTo(v, pos.x + bevel_pixels + 0.5f, pos.y + 0.5f);
			nvgLineTo(v, pos.x + 0.5f, pos.y + bevel_pixels + 0.5f);
			nvgLineTo(v, pos.x + 0.5f, pos.y + size.y - bevel_pixels + 0.5f);
			nvgLineTo(v, pos.x + bevel_pixels + 0.5f, pos.y + size.y + 0.5f);
			nvgLineTo(v, pos.x + size.x - bevel_pixels + 0.5f, pos.y + size.y + 0.5f);
			nvgLineTo(v, pos.x + size.x + 0.5f, pos.y + size.y - bevel_pixels + 0.5f);
			nvgLineTo(v, pos.x + size.x + 0.5f, pos.y + bevel_pixels + 0.5f);
			nvgLineTo(v, pos.x + size.x - bevel_pixels + 0.5f, pos.y + 0.5f);
			nvgLineTo(v, pos.x + bevel_pixels + 0.5f, pos.y + 0.5f);	
		}
	}

	nvgStrokeWidth(v, 1.0f);

	if(style == ButtonStyle::GOOD)
	{
	}
	else if(style == ButtonStyle::BAD)
	{
		
	}
	else
	{
		if(state == ButtonState::CLICKED)
		{
			nvgFillColor(v, nvgRGB(73, 73, 73));
			nvgStrokeColor(v, nvgRGB(142, 142, 142));
		}
		else if(state == ButtonState::DISABLED)
		{
			nvgFillColor(v, nvgRGB(21, 21, 21));
			nvgStrokeColor(v, nvgRGB(41, 41, 41));
		}
		else if(state == ButtonState::HOVERED)
		{
			nvgFillColor(v, nvgRGB(41, 41, 41));
			nvgStrokeColor(v, nvgRGB(73, 73, 73));
		}
		else
		{
			nvgFillColor(v, nvgRGB(21, 21, 21));
			nvgStrokeColor(v, nvgRGB(73, 73, 73));
		}
	}

	nvgFill(v);
	nvgStroke(v);

}

bool SimpleSkin::can_drag_window(GUIWindow* window, glm::ivec2 mpos) 
{
	
	return true;
}

bool SimpleSkin::can_close_window(GUIWindow* window, glm::ivec2 mpos) 
{
	float side_size = window_margins + window_edge_size;
	glm::vec2 button_pos = 
		glm::vec2(window->pos.x + window->size.x - window_icon_size, 
		window->pos.y - titlebar_height - side_size + window_icon_size * 0.5f) + 0.5f;

	logger->info("{},{} / {},{}", mpos.x, mpos.y, button_pos.x, button_pos.y);

	return mpos.x > button_pos.x - 2.0f && mpos.y > button_pos.y - 2.0f && 
		mpos.x < button_pos.x + window_icon_size + 2.0f && mpos.y < button_pos.y + window_icon_size + 2.0f;
}

bool SimpleSkin::can_minimize_window(GUIWindow* window, glm::ivec2 mpos) 
{
	float side_size = window_margins + window_edge_size;
	glm::vec2 button_pos = 
		glm::vec2(window->pos.x + window->size.x - window_icon_size, 
		window->pos.y - titlebar_height - side_size + window_icon_size * 0.5f) + 0.5f;
	if(window->closeable)
	{
		button_pos.x -= (window_icon_size + 2.0f);
	}

	return mpos.x > button_pos.x - 2.0f && mpos.y > button_pos.y - 2.0f && 
		mpos.x < button_pos.x + window_icon_size + 2.0f && mpos.y < button_pos.y + window_icon_size + 2.0f;
}

bool SimpleSkin::can_pin_window(GUIWindow* window, glm::ivec2 mpos) 
{
	float side_size = window_margins + window_edge_size;
	glm::vec2 button_pos = 
		glm::vec2(window->pos.x + window->size.x - window_icon_size, 
		window->pos.y - titlebar_height - side_size + window_icon_size * 0.5f) + 0.5f;
	if(window->closeable)
	{
		button_pos.x -= (window_icon_size + 2.0f);
	}
	if(window->minimizable)
	{
		button_pos.x -= (window_icon_size + 2.0f);
	}

	return mpos.x > button_pos.x - 2.0f && mpos.y > button_pos.y - 2.0f && 
		mpos.x < button_pos.x + window_icon_size + 2.0f && mpos.y < button_pos.y + window_icon_size + 2.0f;
}

glm::ivec4 SimpleSkin::get_window_aabb(GUIWindow* window) 
{
	glm::ivec4 aabb;

	aabb.x = window->pos.x - window_margins - window_edge_size;
	aabb.y = window->pos.y - window_margins - window_edge_size - titlebar_height;
	aabb.z = window->size.x + (window_margins + window_edge_size) * 2;
	aabb.w = window->size.y + window_margins * 2 + window_edge_size + titlebar_height;

	return aabb;
}

void SimpleSkin::draw_window(NVGcontext* vg, GUIWindow* window) 
{
	glm::vec2 pos = window->pos;
	glm::vec2 size = window->size;

	float side_size = window_margins + window_edge_size;

	glm::vec2 title_tl = glm::vec2(pos.x - side_size, pos.y - titlebar_height - side_size) + 0.5f;
	glm::vec2 title_tr = glm::vec2(pos.x + size.x, pos.y - titlebar_height - side_size) + 0.5f;
	glm::vec2 title_trb = glm::vec2(pos.x + size.x + side_size, pos.y - titlebar_height) + 0.5f;
	glm::vec2 title_br = glm::vec2(pos.x + size.x + side_size, pos.y - window_margins) + 0.5f;
	glm::vec2 title_bl = glm::vec2(pos.x - side_size, pos.y - window_margins) + 0.5f;
	glm::vec2 win_br = title_br + glm::vec2(0.0f, size.y);
	glm::vec2 win_bbr = win_br + glm::vec2(-4.0f, 4.0f);
	glm::vec2 win_bbl = win_bbr + glm::vec2(-size.x, 0.0f);
	glm::vec2 win_bl = win_bbl + glm::vec2(-4.0f, -4.0f);

	// Titlebar background is brighter
	nvgBeginPath(vg);
	nvgMoveTo(vg, title_tl.x, title_tl.y);
	nvgLineTo(vg, title_tr.x, title_tr.y);
	nvgLineTo(vg, title_trb.x, title_trb.y);
	nvgLineTo(vg, title_br.x, title_br.y);
	nvgLineTo(vg, title_bl.x, title_bl.y);
	nvgLineTo(vg, title_tl.x, title_tl.y);
	nvgFillColor(vg, nvgRGB(61, 70, 79));
	nvgFill(vg);
	// Window background is darker
	nvgBeginPath(vg);
	nvgMoveTo(vg, title_bl.x, title_bl.y - 0.5f);
	nvgLineTo(vg, title_br.x, title_br.y - 0.5f);
	nvgLineTo(vg, win_br.x, win_br.y);
	nvgLineTo(vg, win_bbr.x, win_bbr.y);
	nvgLineTo(vg, win_bbl.x, win_bbl.y);
	nvgLineTo(vg, win_bl.x, win_bl.y);
	nvgLineTo(vg, title_bl.x, title_bl.y - 0.5f);
	nvgFillColor(vg, nvgRGB(40, 45, 50));
	nvgFill(vg);
	// Window edge is colored separately
	if(window->is_focused())
	{
		nvgStrokeColor(vg, nvgRGB(170, 170, 170));
	}
	else
	{
		nvgStrokeColor(vg, nvgRGB(61, 70, 79));
	}
	nvgBeginPath(vg);
	nvgMoveTo(vg, title_tl.x, title_tl.y);
	nvgLineTo(vg, title_tr.x, title_tr.y);
	nvgLineTo(vg, title_trb.x, title_trb.y);
	nvgLineTo(vg, title_br.x, title_br.y);
	nvgLineTo(vg, win_br.x, win_br.y);
	nvgLineTo(vg, win_bbr.x, win_bbr.y);
	nvgLineTo(vg, win_bbl.x, win_bbl.y);
	nvgLineTo(vg, win_bl.x, win_bl.y);
	nvgLineTo(vg, title_tl.x, title_tl.y);
	nvgStroke(vg);

	nvgFillColor(vg, nvgRGB(255, 255, 255));
	nvgBitmapText(vg, window_title_font.duplicate(), TextDrawer::LEFT, 
		title_tl.x + titlebar_margin + 0.5f, title_tl.y + titlebar_margin + 0.5f, window->title.c_str());


	bool bools[3] = {window->closeable, window->minimizable, window->pinable};
	bool hovered[3] = {window->close_hovered, window->minimize_hovered, window->pin_hovered};
	AssetHandle<Image>* imgs[3] = {&window_close, &window_min, &window_pin};
	float x_offset = 0.0f;
	for(int i = 0; i < 3; i++)
	{
		if(bools[i])
		{
			float alpha = hovered[i] ? 1.0f : 0.5f;
			glm::vec2 pos = title_tr + glm::vec2(-window_icon_size - x_offset, window_icon_size * 0.5f);
			x_offset += window_icon_size + 2.0f;
			NVGpaint pnt = nvgImagePattern(vg, pos.x, pos.y, window_icon_size, window_icon_size, 
				0.0f, (*imgs[i])->get_nvg_image(vg), alpha);
			nvgBeginPath(vg);
			nvgRect(vg, pos.x, pos.y, window_icon_size, window_icon_size);
			nvgFillPaint(vg, pnt);
			nvgFill(vg);
		}

	}
}	

SimpleSkin::SimpleSkin()
{
	window_title_font = AssetHandle<BitmapFont>("core:fonts/ProggySquare.fnt");
	window_close = AssetHandle<Image>("core:gui/close.png");
	window_min = AssetHandle<Image>("core:gui/minimize.png");
	window_pin = AssetHandle<Image>("core:gui/pin.png");
}