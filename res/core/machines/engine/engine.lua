-- Based on Design of Liquid Propellant Rocket Engines, Huzel and Huang.
-- for reasonable approximation of real-life values

-- This file glues the chamber and nozzle simulators and gives everything to the engine interface

local logger = require("logger")
local imgui = require("imgui")
local toml = require("toml")

local nozzle_piece_str = machine.init_toml:get_string_or("nozzle_piece", "p_root")
local nozzle_piece = part:get_piece(nozzle_piece_str)
local nozzle_marker = machine.init_toml:get_string("nozzle_marker")

local engine_interface = machine:load_interface("core:interfaces/engine.lua")

local last_thrust = 0.0

-- Include the simulators
dofile("machines/engine/chamber.lua")
dofile("machines/engine/nozzle.lua")

function physics_update(dt)
    engine_interface:physics_update(dt)
end


-- Implementation for the engine interface stuff

engine_interface.get_nozzle_position = function(self)
    local local_pos = nozzle_piece:get_marker_position(nozzle_marker)
    return nozzle_piece:transform_point_to_rigidbody(local_pos)
end

engine_interface.get_nozzle_forward = function(self)
    local local_fwd = -nozzle_piece:get_marker_forward(nozzle_marker)
    return nozzle_piece:transform_axis(local_fwd)
end

engine_interface.get_piece = function(self)
    return nozzle_piece
end

engine_interface.get_thrust = function(self, dt) 
    local to_nozzle, pressure, is_choked = simulate_chamber(dt)
    local v, T, P = get_exit_properties(to_nozzle.temperature, pressure, to_nozzle:get_specific_gas_constant(), is_choked) 
    -- Thrust equation for a rocket engine
    -- TODO: When atmospheres are simulated, we must include the ambient pressure!
    local ambient_P = 0.0
    local thrust = v * to_nozzle:get_total_gas_mass() / dt + (P - ambient_P) * exit_area 
    last_thrust = thrust
    return thrust
end 

function draw_imgui()
    draw_chamber_imgui()
    imgui.separator()
    draw_nozzle_imgui()
    imgui.separator()
    imgui.text("Thrust: " .. last_thrust .. "N")
end

