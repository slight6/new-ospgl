---@module 'scene_script'
-- The map scene shows an overview of the solar system, including orbits and orbit predictions
-- Highly configurable so it can be used in many places

local rnd = require("renderer")
require("universe")
local glm = require("glm")
local gui = require("gui")
local assets = require("assets")
local logger = require("logger")
local orbit = require("orbit")
local veh_spawner = dofile("core:scenes/vehicle_spawner.lua")

require("toml")

local gui_screen = gui.screen.new(gui.skin.get_default_skin(), gui_input)
local universe = osp.universe
local renderer = osp.renderer
local anim = nil

local predictor = orbit.quick_predictor.new(universe.system)
predictor:launch()


local cubemap = assets.get_cubemap("debug_system:skybox.png")
local sunlight = rnd.sun_light.new(osp.renderer.quality.sun_terrain_shadow_size, osp.renderer.quality.sun_shadow_size)
local skybox = rnd.skybox.new(cubemap:move())
local camera = dofile("core:scenes/map/map_camera.lua"):init(universe, gui_input)

---@type universe.entity | nil
--- If non-nil, then we are controlling an entity and will pass inputs to it
--- and optionally draw navball, etc...
local controlled_ent = nil

local event_handlers = {}

---@param map_id string
---@param ncontrolled_ent universe.entity|nil
---@param nconfig table
function load(map_id, ncontrolled_ent, nconfig)
	if map_id == nil then map_id = "default" end
	config = nconfig
	controlled_ent = ncontrolled_ent
	-- Load map camera if saved for same map_id, or start at the sun
	local sets = universe.save_db:get_toml("map")
	if sets:contains("center_entity") then

	else

	end
	camera.sensitivity = sets:get_number_or("cam_sensitivity", 0.006)

	-- We only draw the universe and markers
	logger.info(universe.system:get_element_position("Earth"))
	renderer:add_drawable(universe.system)
	renderer:add_drawable(skybox)
	renderer:add_drawable(predictor)
	renderer:add_light(sunlight)
	
	table.insert(event_handlers, universe:sign_up_for_event("core:new_entity",
		function(id) logger.info(tostring(id)) renderer:add_drawable(universe.entities[id]) end))

	table.insert(event_handlers, universe:sign_up_for_event("core:remove_entity",
		function(id) renderer:remove_drawable(universe.entities[id]) end))

	-- Add already existing entities
	for _, entity in ipairs(universe.entities) do 
		renderer:add_drawable(entity) 
	end
	
	local veh = veh_spawner.spawn_vehicle(universe, assets.get_udata_vehicle("debug.toml"),
		glm.vec3.new(-2.720318042296709e10 + 9000e3, 1.329407956490104e10, 5.764165538717468e10),
		glm.vec3.new(-2.975363625e4, -5.189341029e3 + 9000.0, -2.251484e3), glm.quat.new(1, 0, 0, 0), glm.vec3.new(0, 0, 0), true)

	controlled_ent = veh

end

function update(dt)
	predictor:update(controlled_ent:get_visual_origin(), controlled_ent:get_velocity())

	gui_screen:new_frame()
	gui_screen:prepare_pass()

	camera:update(dt)
	local ent_blocked_kb = false
	if controlled_ent then
		local input_ctx = controlled_ent:get_input_ctx()
		if input_ctx then
			ent_blocked_kb = input_ctx:update(gui_input.keyboard_blocked, dt)
		end
	end
	gui_input.ext_keyboard_blocked = gui_input.ext_keyboard_blocked or ent_blocked_kb

	gui_screen:input_pass()
	gui_screen:draw()
end

function pre_update(dt)
	osp.universe:update(dt)
end

function render()
	renderer:render()
end

function unload()
	renderer:clear()
	-- Make sure all elements are render enabled 
	for _, elem in ipairs(osp.universe.system.elements) do 
		elem.render_enabled = true
	end
end

function close_map()

end

function get_camera_uniforms(width, height)
	return camera:get_camera_uniforms(width, height)
end
