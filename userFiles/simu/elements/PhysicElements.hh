/*! 
 * \author Nicolas Van der Noot
 * \file PhysicElements.hh
 * \brief PhysicElements class
 */

#ifndef __PHYSIC_ELEMENTS__HH__
#define __PHYSIC_ELEMENTS__HH__

#include "Elements.hh"
#include "GeometryShape.hh"

/*! \brief Physic element object
 * 
 * physical object which can produce external forces on other physical objects
 */
class PhysicElements: public Elements
{
	public:
		PhysicElements(GeometryShape *geometry);

		virtual ~PhysicElements();

		/// get the mass of the object
		double get_mass()    const { return mass;    }

		/// get the inertia of the object
		double get_inertia() const { return inertia; }

		/// get the kinetif friction coefficicent
		double get_kinetic_friction() { return kinetic_friction; }

		/// set a new mass for the object
		void set_mass(double value)
		{
			mass = value;
			set_kinetic_friction();    
		}

		/// set a new inertia for the object
		void set_inertia(double value) { inertia = value; }

		/// set the kinetif friction coefficicent
		void set_kinetic_friction();

		/// set the index of the element
		void set_index_elem(int index_value) { geometry->set_index_elem(index_value); }

		/// get the force applied on this object
		Vector2D get_force() const { return geometry->get_force();  }

		/// get the torque applied on this object
		double get_torque()  const { return geometry->get_torque(); }

		/// get the pose of the object geometry
		Pose get_pose() const { return geometry->get_pose(); }

		/// get the position of the object geometry
		Vector2D get_pos()   const { return geometry->get_pose().get_pos();   }

		/// get the velocity of the object geometry
		Vector2D get_pos_p() const { return geometry->get_pose().get_pos_p(); }

		/// get the ansolute speed
		double get_speed() const { return get_pos_p().get_dist(); }

		/// get the orientation angle of the object geometry
		double get_alpha()   const { return geometry->get_pose().get_alpha(); }

		/// get the orientation velocity of the object geometry
		double get_alpha_p() const { return geometry->get_pose().get_alpha_p(); }

		/// get the geometry descibing this object
		GeometryShape* get_geometry() const { return geometry; }

		/// get the x coordinate of this object
		double get_pose_x() const { return geometry->get_pose().get_x(); }

		/// get the y coordinate of this object
		double get_pose_y() const { return geometry->get_pose().get_y(); }

		/// update the pose from a given position
		void update_from_pos(Vector2D const& vec, double angle) { geometry->update_from_pos(vec, angle); }
		
		/// update the pose from a a given velocity
		void update_from_vel(Vector2D const& vec_p, double angle_p, double delta_t) { geometry->update_from_vel(vec_p , angle_p , delta_t); }
		
		/// update the pose from a given acceleration
		void update_from_acc(Vector2D const& vec_pp, double angle_pp, double delta_t) { geometry->update_from_acc(vec_pp, angle_pp, delta_t); }

		/// set a new pose
		void update_new_pose(double x, double y, double x_p, double y_p, double orient, double orient_p) 
		{
			geometry->update_new_pose(x, y, x_p, y_p, orient, orient_p); 
		}

		/// modify the flag 'flag_contact'
		void set_contact(int flag_value) { get_geometry()->set_contact(flag_value); }

		/// modify the flag 'flag_uSwitch'
		void set_uSwitch(int flag_value) { get_geometry()->set_uSwitch(flag_value); }

		/// set the forces and torques to 0
		void reset_force_torque() { geometry->reset_force_torque(); }

		/// update recursively the forces and torques
		void update_force_torque() { geometry->update_force_torque(); }

		/// get the force applied on this object
		Vector2D get_force() { return geometry->get_force();  }

		/// get the torque applied on this body
		double get_torque()  { return geometry->get_torque(); }

		/// get the absolute speed [rad/s]
		double get_abs_speed() { return geometry->get_abs_speed(); }

		/// update the segments in case of rectangle geometries
		void update_segments() { geometry->update_segments(); }

		virtual int is_robot(int &robot_index) = 0;

	protected:
		GeometryShape *geometry; ///< description of the shapes describing the object

		double mass;    ///< mass [kg]
		double inertia; ///< inertia [kg*m^2]

		double kinetic_friction; ///< kinetic friction basic force [N]
};


#endif
