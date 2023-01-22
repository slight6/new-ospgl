---@meta 


---@class universe.lua_event_handler
local lua_event_handler = {}
function lua_event_handler:sign_out() end

---@class universe
---@field bt_world bullet.world
---@field system universe.planetary_system
---@field entities table Read only
---@field save_db universe.save_database
local universe = {}

---@param event_id string
---@param fun function
---@return universe.lua_event_handler
function universe:sign_up_for_event(event_id, fun) end

---@param event_id string
---@param ... any further arguments
function universe:emit_event(event_id, ...) end

---@param script_path string path to the script
---@return universe.entity
--- Passes extra arguments directly to the new entity
function universe:create_entity(script_path, ...) end

---@param id integer
---@return universe.entity
--- Do not hold to the returned value for long, as it may go invalid pretty quickly!
function universe:get_entity(id) end

---@param dt number
function universe:update(dt) end

---@class universe.planetary_system
local planetary_system = {}

---@param name string Name of the element, or __default 
---@return glm.vec3
function planetary_system:get_element_position(name) end

---@param name string Name of the element, or __default 
---@return glm.vec3
function planetary_system:get_element_velocity(name) end

---@class universe.entity
--- Entities are implemented in lua and work as tables!
--- (ie, you can access all public stuff in their environment)
--- but some functions and variables are also exposed, written in C++
---@field uid integer
---@field drawable_uid integer
---@field init_toml toml.table
local entity = {}
-- TODO: Do what's said in the comment

---@return glm.vec3
function entity:get_visual_origin() end


---@class universe.save_database
local save_db = {}

---@param arg1 string Path to load (or package if 2 arguments are passed)
---@param arg2? Path to load if 2 arguments are used
---@return toml.table
--- Obtains a persistent toml file. You can modify the contents and they will
--- automatically be serialized on next save-game
function save_db:get_toml(arg1, arg2) end
