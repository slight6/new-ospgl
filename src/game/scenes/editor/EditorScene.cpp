#include "EditorScene.h"
#include <OSP.h>

void EditorScene::load()
{
	gui.vg = get_osp()->renderer->vg;
	gui.init(this);

	get_osp()->game_state.universe.paused = true;
	
	// The GUI takes a big portion of the screen and it's opaque
	get_osp()->renderer->override_viewport = glm::dvec4(0.25, 0.0, 1.0, 1.0);
	get_osp()->renderer->cam = &cam;	


	get_osp()->renderer->add_drawable(&vehicle);
	
}

void EditorScene::update()
{
	gui_input.update();
	do_gui();
}

void EditorScene::render()
{
	get_osp()->renderer->render(nullptr);
}

void EditorScene::do_gui()
{
	float width = get_osp()->renderer->get_width(true);
	float height = get_osp()->renderer->get_height(true);

	gui.do_gui(width, height, &gui_input);	
}

void EditorScene::unload()
{
	get_osp()->renderer->remove_drawable(&vehicle);
	get_osp()->renderer->override_viewport = glm::dvec4(0.0, 0.0, 1.0, 1.0);
	get_osp()->game_state.universe.paused = false;	
}


