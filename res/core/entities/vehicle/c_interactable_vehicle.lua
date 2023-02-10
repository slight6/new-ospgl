-- Allows highlighting pieces using raycast of linked vehicle
-- Highlighted pieces can spawn dialog menus on mouse to show info, and can be interacted
-- with via all button clicks.
-- Highly customizable, created around a vehicle entity
-- Add as a drawable to the renderer
local glm = require("glm")
local input = require("input")
local bullet = require("bullet")
local model = require("model")
local assets = require("assets")
local raycast = require("core:util/g_raycast.lua")
local debug_drawer = require("debug_drawer")
local logger = require("logger")

local hover_material = assets.get_material("core:mat_hover.toml")
local mat_override = model.mat_override.from_table({color = glm.vec3.new(1, 1, 1)})

---@class core.interactable_vehicle
local interactable_vehicle = {}

---@type universe.entity
interactable_vehicle.veh_ent = nil
---@type vehicle
interactable_vehicle.veh = nil

---@type integer Points to a piece, if negative means not hovering anything
interactable_vehicle.hovered = -1

--- Stores all active contextual menus
interactable_vehicle.context_menus = {}

---@param veh_ent universe.entity
function interactable_vehicle:init(veh_ent) 
	self.veh_ent = veh_ent
	self.veh = veh_ent.lua.vehicle
	assert(self.veh, "Linked entity was not a vehicle, this is not allowed")
	--self.veh.on_change:add_handler(self.on_dirty, self)
	self:on_veh_dirty()

	return self
end

--- Call when the vehicle entity is lost (also called on gc)
function interactable_vehicle:finish()
	self.veh = nil
	self.veh_ent = nil
end

function interactable_vehicle:on_veh_dirty()
	
end

function interactable_vehicle:update_hover(cu)
	local rstart, rend = raycast.get_mouse_ray(osp.renderer, cu, 100.0) 
	local result = osp.universe.bt_world:raycast(rstart, rend)

	local closest_hit = nil
	local closest_type = nil
	local closest_dist = math.huge
	for id, hit in pairs(result) do
		local dist = glm.length(hit.pos - rstart)
		local hit_udata = hit.rg:get_udata_type()
		debug_drawer.add_point(hit.pos, glm.vec3.new(1.0, 1.0, 0.0))
		if dist < closest_dist and (hit_udata == "piece" or hit_udata == "welded_group") then
			closest_dist = dist
			closest_type = hit_udata
			closest_hit = id
		end
	end


	if not closest_hit then
		self.hovered = -1
		return
	end

	local hit = result[closest_hit]
	local piece = nil

	-- Resolve it, pretty easy thanks to compound_id
	if closest_type == "welded_group" then
		local as_welded_group = hit.rg:get_udata_wgroup()
		assert(as_welded_group)
		piece = as_welded_group:get_piece(hit.compound_id)	
	else 
		piece = hit.rg:get_udata_piece()
	end
	assert(piece, "Could not resolve piece, something's broken in vehicle!")

	self.hovered = piece.id

end

function interactable_vehicle:new_context_menus() 
	if self.hovered < 0 then return end
	if not input.mouse_down(input.btn.left) then return end
	
	if self.context_menus[self.hovered] then
		-- Context menu is already present, highlight it
		self.context_menus[self.hovered].highlight_timer = 1.0
		return
	end

	self.context_menus[self.hovered] = {highlight_timer = 1.0}

	-- Remove all others if CTRL is not held
	if not input.key_down(input.key.left_control) then
		for k, _ in pairs(self.context_menus) do 
			if k ~= self.hovered then 
				self.context_menus[k] = nil
			end
		end
	end

end

---@param gui gui.screen
function interactable_vehicle:update_context_menus(dt, gui)
	for p_id, menu in pairs(self.context_menus) do 
		if menu.highlight_timer > 0.0 then
			menu.highlight_timer = menu.highlight_timer - dt
		end

	end
end

---@param cu renderer.camera_uniforms Camera uniforms for the raycast (generate on physics update)
function interactable_vehicle:physics_update(cu)
	self:update_hover(cu)

end

---@param gui gui.screen
function interactable_vehicle:update(dt, gui)
	-- Prevent hovering if mouse is blocked, this effectively blocks everything else
	if gui.input.mouse_blocked or gui_input.ext_mouse_blocked then
		self.hovered = -1
	end

	-- Bring up context menus
	self:new_context_menus()
	self:update_context_menus(dt, gui)

end

function interactable_vehicle:forward_pass(cu, _)
	if self.hovered < 0 then return end
	local piece = self.veh:get_piece_by_id(self.hovered)
	assert(piece)
	local i = glm.inverse(piece.collider_offset) ---@cast i glm.mat4
	local tform = piece:get_graphics_transform():to_mat4() * i
	piece:get_model_node():draw_override(cu, hover_material:get(), tform, 0, mat_override, true, true)

end


return interactable_vehicle
