#pragma once
#include "BaseInterface.h"
#include <game/plumbing/PlumbingEditor.h>

class EditorVehicleInterface;

class PlumbingInterface : public BaseInterface
{
private:

	float view_size;
	PlumbingEditor pb_editor;

public:

	bool do_interface(const CameraUniforms& cu, glm::dvec3 ray_start, glm::dvec3 ray_end,
					  glm::dvec4 viewport, NVGcontext* vg, GUIInput* gui_input, GUISkin* gui_skin) override;
	void update(double dt) override;
	void leave() override;
	bool can_leave() override;
	glm::dvec4 get_vehicle_viewport() override;

	PlumbingInterface(EditorVehicleInterface* edveh_int);
};