#include "LuaVehicle.h"
#include "../../universe/vehicle/Vehicle.h"

void LuaVehicle::load_to(sol::table& table)
{
	table.new_usertype<Vehicle>("vehicle");	

	table.new_usertype<Piece>("piece",
		"rigid_body", &Piece::rigid_body,
		"welded", &Piece::welded,
		"get_global_transform", [](Piece& self)
		{
			return BulletTransform(self.get_global_transform());
		},
		"get_graphics_transform", [](Piece& self)
		{
			return BulletTransform(self.get_graphics_transform());
		},
		"get_local_transform", [](Piece& self)
		{
			return BulletTransform(self.get_local_transform());
		},
		"get_linear_velocity", [](Piece& self)
		{
			return to_dvec3(self.get_linear_velocity());
		},
		"get_angular_velocity", [](Piece& self)
		{
			return to_dvec3(self.get_angular_velocity());
		},
		"get_tangential_velocity", [](Piece& self)
		{
			return to_dvec3(self.get_tangential_velocity());
		},
		"get_relative_position", [](Piece& self)
		{
			return to_dvec3(self.get_relative_position());
		},
		"get_global_position", [](Piece& self)
		{
			return to_dvec3(self.get_global_transform().getOrigin());
		},
		"get_graphics_position", [](Piece& self)
		{
			return to_dvec3(self.get_graphics_transform().getOrigin());
		},
		"get_forward", &Piece::get_forward,
		"get_up", &Piece::get_up,
		"get_right", &Piece::get_right,
		"transform_axis", &Piece::transform_axis,
			
		"get_marker_position", &Piece::get_marker_position,
		"get_marker_rotation", &Piece::get_marker_rotation,
		"get_marker_transform", &Piece::get_marker_transform,
		"get_marker_forward", &Piece::get_marker_forward,
		"transform_point_to_rigidbody", &Piece::transform_point_to_rigidbody);

	table.new_usertype<Part>("part",
		"get_piece", &Part::get_piece,
		"get_machine", &Part::get_machine);

	table.new_usertype<Machine>("machine",
		"init_toml", &Machine::init_toml,
		"load_interface", [](Machine& self, const std::string& iname, sol::this_state st)
		{
			sol::state_view sv(st);
			auto[pkg, name] = assets->get_package_and_name(iname, sv["__pkg"]);
			std::string sane_name = pkg + ":" + name;
			auto result = sv.safe_script("return require(\"" + sane_name + "\")");
			sol::table n_table = result.get<sol::table>();
			// We give this one to the machine
			self.load_interface(sane_name, n_table);
			return n_table;
		},

		"get_all_wired_machines", sol::overload([](Machine& self)
		{
			return self.get_all_wired_machines();
		},
		[](Machine& self, bool include_this)
		{
			return self.get_all_wired_machines(include_this);
		}),
		"get_wired_machines_with", [](Machine& self, sol::variadic_args args)
		{
			// We build the vector, something which sol cannot do automatically
			// and also find any 'false' value which sets include_this to false
			std::vector<std::string> vec;
			bool include_this = true;
			for(auto arg : args)
			{
				if(arg.get_type() == sol::type::boolean)
				{
					if(arg.get<bool>() == false)
					{
						include_this = false;
					}
				}
				else if(arg.get_type() == sol::type::string)
				{
					vec.push_back(arg.get<std::string>());
				}
			}

			return self.get_wired_machines_with(vec, include_this);
		},
		"get_wired_interfaces", 
		sol::overload([](Machine& self, const std::string& int_type, bool include_this)
		{
			return self.get_wired_interfaces(int_type, include_this);
		},
		[](Machine& self, const std::string& int_type)
		{
			return self.get_wired_interfaces(int_type);
		}),
		"get_interface", &Machine::get_interface
		
	);

}
