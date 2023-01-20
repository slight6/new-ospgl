---@module 'scene_script'
-- The map scene shows an overview of the solar system, including orbits and orbit predictions
-- Highly configurable so it can be used in many places

require("renderer")
require("universe")
local glm = require("glm")
local gui = require("gui")
require("toml")

local gui_screen = gui.screen.new(gui.skin.get_default_skin(), gui_input)
local universe = osp.universe
local renderer = osp.renderer
local anim = nil
local config = nil


---@param animation core.map.anim
---@param map_id string
---@param config table
function load(animation, map_id, nconfig)
  if map_id == nil then map_id = "default" end
  -- Save the anim so it can be used on close too
  anim = animation
  config = nconfig
  -- Load map position if saved for same map_id, or start at the sun
  local sun_pos = glm.vec3.new(0.0, 0.0, 0.0)
  -- We only draw the universe and markers
  renderer:add_drawable(universe.system)

end

function update(dt)

end

function pre_update(dt)
  osp.universe:update(dt)
end


function close_map() 

end
