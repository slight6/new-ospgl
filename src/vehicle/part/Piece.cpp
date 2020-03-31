#include "Piece.h"
#include "../Vehicle.h"
#include "../../util/Logger.h"

bool Piece::is_welded()
{
	return in_group != nullptr;
}

btTransform Piece::get_global_transform(bool use_mstate)
{
	if(in_vehicle->is_packed())
	{
		return in_vehicle->packed_veh.get_root_transform() * packed_tform;
	}
	else 
	{
		btTransform tform;

		if (use_mstate)
		{
			motion_state->getWorldTransform(tform);
		}
		else
		{
			tform = rigid_body->getWorldTransform();
		}

		if (is_welded())
		{
			return tform * welded_tform;
		}
		else
		{
			return tform;
		}
	}
}

btTransform Piece::get_local_transform()
{
	if (is_welded())
	{
		return welded_tform;
	}
	else
	{
		return btTransform::getIdentity();
	}
	
}

btVector3 Piece::get_linear_velocity(bool ignore_tangential)
{
	if(in_vehicle->is_packed())
	{
		btVector3 base = to_btVector3(in_vehicle->packed_veh.get_root_state().cartesian.vel);
	
		base += get_tangential_velocity();

		return base;
	}
	else
	{
		btVector3 base = rigid_body->getLinearVelocity();
		if (is_welded() && !ignore_tangential)
		{
			base += get_tangential_velocity();
		}

		return base;
	}
}

btVector3 Piece::get_angular_velocity()
{
	if(in_vehicle->is_packed())
	{
		return to_btVector3(in_vehicle->packed_veh.get_root_state().angular_velocity);
	}
	else 
	{
		return rigid_body->getAngularVelocity();
	}
}

btVector3 Piece::get_angular_momentum()
{
	double r2 = get_relative_position().length2();
	double m = mass;
	return get_angular_velocity() * r2 * m;
}

btVector3 Piece::get_tangential_velocity()
{

	if(in_vehicle->is_packed())
	{
		btVector3 r = packed_tform.getOrigin();
		// We need to correct r to the center of mass
		btVector3 com = in_vehicle->packed_veh.get_com_root_relative();
		r -= com;
		logger->info("COM: {} {} {}", com.x(), com.y(), com.z());
		btVector3 tangential = get_angular_velocity().cross(r);
		return tangential;
	}
	else 
	{

		if (!is_welded())
		{
			return btVector3(0.0, 0.0, 0.0);
		}

		btVector3 r = get_relative_position();
		btVector3 tangential = rigid_body->getAngularVelocity().cross(r);
	
		return tangential;
	}
}

btVector3 Piece::get_relative_position()
{
	// TODO: Check this stuff
	//return get_local_transform().getOrigin();
	if(is_welded())
	{
		// Not get_global_transform() as that already includes get_local_transform()
		btTransform global = rigid_body->getWorldTransform();
		btTransform local = get_local_transform();
		btTransform mul = global * local; //< This is final position

		btVector3 final = mul.getOrigin() - global.getOrigin();

		return final;
	}
	else
	{
		return btVector3(0, 0, 0);
	}
}	 

void Piece::set_dirty()
{
	if (in_group != nullptr)
	{
		in_group->dirty = true;
	}
	else
	{
		dirty = true;
	}

	in_vehicle->unpacked_veh.dirty = true;
}

glm::dvec3 Piece::transform_axis(glm::dvec3 axis)
{
	btQuaternion rot = get_global_transform().getRotation();
	glm::dvec3 result = glm::toMat4(to_dquat(rot)) * glm::dvec4(axis, 1.0);

	return result;	
}

glm::dvec3 Piece::get_forward()
{
	return transform_axis(glm::dvec3(0, 0, 1));	
}

glm::dvec3 Piece::get_up()
{
	return transform_axis(glm::dvec3(0, 1, 0));
}

glm::dvec3 Piece::get_right()
{
	return transform_axis(glm::dvec3(1, 0, 0));
}

Piece::Piece(Part* in_part, std::string piece_name) 
	: model_node(in_part->part_proto->pieces[piece_name].model_node.duplicate())
{
	attached_to = nullptr;
	part = nullptr;
	collider = nullptr;
	rigid_body = nullptr;
	motion_state = nullptr;
	in_group = nullptr;
	welded = false;

	part = in_part;

	mass = part->part_proto->pieces[piece_name].mass;
	friction = part->part_proto->pieces[piece_name].friction;
	restitution = part->part_proto->pieces[piece_name].restitution;

	collider = part->part_proto->pieces[piece_name].collider;
	collider_offset = part->part_proto->pieces[piece_name].render_offset;

}

Piece::~Piece()
{
}
