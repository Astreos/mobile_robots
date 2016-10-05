
#ifndef __LIST_PHYSIC_ELEMENTS__HH__
#define __LIST_PHYSIC_ELEMENTS__HH__

#include <vector>
#include "Vector2D.hh"
#include "UnionShape.hh"
#include "mbs_data.h"
#include "Controller.hh"

class MovingObject;
class FixedObject;
class Robot;
class PhysicElements;

enum{ROBOT_1};

/*! \brief List of physical elements
 * 
 * These elements can generate contact forces
 */
class ListPhysicElements
{
	public:
		ListPhysicElements(MbsData *mbs_data);
		virtual ~ListPhysicElements();

		MovingObject*   get_moving(int p) { return list_moving[p]; }
		FixedObject*    get_fixed(int p)  { return list_fixed[p];  }
		Robot*          get_robot(int p)  { return list_robot[p];  }
		PhysicElements* get_full(int p)   { return list_full[p];   }

		/// get last element added to the list
		PhysicElements* get_last()
		{
			if (list_full.size())
			{
				return list_full[list_full.size()-1];   
			}
			else
			{
				return NULL;
			}
		}

		int get_nb_moving() { return list_moving.size(); }
		int get_nb_fixed()  { return list_fixed.size();  }
		int get_nb_robot()  { return list_robot.size();  }
		int get_nb_full()   { return list_full.size();   }

		void fill_T1_T2_R3(int moving_id, int &T1, int &T2, int &R3);

		void add_wall(Vector2D const& init_center, double angle, double wrapping_radius);
		void add_fixed_circle(Vector2D const& init_center, double radius);
		void add_moving_circle(Vector2D const& init_center, double radius, int visu_T1, int visu_T2, int visu_R3);
		void add_fixed_rect(Vector2D const& init_center, double orientation, double semi_width, double semi_height);
		void add_moving_rect(Vector2D const& init_center, double orientation, double semi_width, double semi_height, int visu_T1, int visu_T2, int visu_R3);
		void add_fixed_union(UnionShape *cur_union);
		void add_moving_union(UnionShape *cur_union, int visu_T1, int visu_T2, int visu_R3);
		void add_robot(int robot_id, Controller *ctrl);

	protected:
		MbsData *mbs_data;

		std::vector<MovingObject*> list_moving; ///< list of all moving objects (with contact)
		std::vector<FixedObject*> list_fixed;   ///< list of all fixed objects (with contact)
		std::vector<Robot*> list_robot;         ///< list of all robots
		std::vector<PhysicElements*> list_full; ///< list of all objects with contact (physical elements)

		std::vector<int> moving_T1_id; ///< list of IDs for T1
		std::vector<int> moving_T2_id; ///< list of IDs for T2
		std::vector<int> moving_R3_id; ///< list of IDs for R3

		// function prototypes
		void add_circle(Vector2D const& init_center, double radius, int visu_T1, int visu_T2, int visu_R3, int flag_moving);
		void add_rect(Vector2D const& init_center, double orientation, double semi_width, double semi_height,
			int visu_T1, int visu_T2, int visu_R3, int flag_moving);
		Controller* get_ctrl_from_name(char *ctrl_name, int robot_id, int nb_opp);
		void robot_ctrl();
		int is_opp_team_A(char *ctrl_name, int robot_id);
		int is_opp_team_B(char *ctrl_name, int robot_id);
};


#endif
