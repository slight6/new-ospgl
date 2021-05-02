local plumbing = {};

function plumbing.is_requester() return true end

function plumbing.fluid_update()

end

function plumbing.get_pressure(port)

end

function plumbing.out_flow(port, volume)

end

function plumbing.in_flow(port, fluids)

end

function plumbing.get_free_volume()
    print("Value of guamero: " .. guamero)

end

-- Transform, colors and width are already set, just draw the lines
-- Respect your bounding box
function plumbing.draw_diagram(vg, x, y)
    vg:moveTo(x, y)
    vg:lineTo(x+1.0, y)
    vg:lineTo(x+1.0, y+1.0)
    vg:fill()
    vg:stroke()
end

-- Return a pair of values, width and height of the box in units
-- It will be used for dragging and preventing overlaps
function plumbing.get_draw_bounds()

end

-- Return 4 values, first two represent position, next two direction
-- Ports will be drawn in an unified manner by the editor
function plumbing.get_port_draw_position(port)

end

return plumbing;