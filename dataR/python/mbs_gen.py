# Manipulate .mbs files to combine them and generate the resulting XML files
# for both Robotran and the visualization.
#
# author: Nicolas Van der Noot

import os
import xml.etree.ElementTree as ET

# set an element
def set_text(elem, value):
	elem.text = str(value)

# create the child elem
def create(parent_elem, child_name):

	ET.SubElement(parent_elem, child_name)
	child_elem = parent_elem.find(child_name)

	return child_elem

# get the child elem (create it if non-existent)
def get_create(parent_elem, child_name):

	child_elem = parent_elem.find(child_name)

	if child_elem == None:
		return create(parent_elem, child_name)
	else:
		return child_elem

# create a field and add a text
def create_text(parent_elem, child_name, value):

	elem = create(parent_elem, child_name)
	elem.text = str(value)

	return elem

# change the name of the mbs
def change_mbsname(tree, new_name):

	mbsname = tree.getroot().find('mbsname')
	mbsname.text = str(new_name)

# remove a body from a tree
def remove_body(tree, remove_name):

	bodytree = tree.getroot().find('bodytree')

	for body in bodytree.findall('body'):
		if str(body.find('bodyname').text) == remove_name:
			bodytree.remove(body)

# add bodies from a second tree to a main tree.
# bodies are added after a first body was discovered
# to be similar between two bodies
def add_body(main_tree, second_tree):

	main_bodytree   = main_tree.getroot().find('bodytree')
	second_bodytree = second_tree.getroot().find('bodytree')

	common_found = 0

	for body_1 in main_bodytree.findall('body'):

		if common_found == 1:
			break

		for body_2 in second_bodytree.findall('body'):

			if str(body_1.find('bodyname').text) == str(body_2.find('bodyname').text):
				common_found = 1
			elif common_found == 1:
				main_bodytree.append(body_2)

# add all the bodies of the second tree to the main one
def add_all_bodies(main_tree, second_tree):

	main_bodytree   = main_tree.getroot().find('bodytree')
	second_bodytree = second_tree.getroot().find('bodytree')

	for cur_body in second_bodytree.findall('body'):
		main_bodytree.append(cur_body)

# add all the links of the second tree to the main one
def add_all_links(main_tree, second_tree):

	main_links   = get_create(main_tree.getroot(), 'links')
	second_links = second_tree.getroot().find('links')

	for cur_body in second_links.findall('link'):
		main_links.append(cur_body)

# add all user models of the second tree to the main one
def add_all_user_models(main_tree, second_tree):

	main_users   = main_tree.getroot().find('user_models')
	second_users = second_tree.getroot().find('user_models')

	if main_users == None:

		ET.SubElement(main_tree.getroot(), 'user_models')
		main_users = main_tree.getroot().find('user_models')

	for cur_user in second_users.findall('user_model'):

		new_field = 1
		cur_name  = cur_user.find('name').text
		cur_param = cur_user.find('parameter')
		cur_type  = cur_param.find('type').text

		for cur_main in main_users.findall('user_model'):

			if cur_main.find('name').text == cur_name:
				new_field = 0

				cur_main_param = cur_main.find('parameter')
				cur_main_type  = cur_main_param.find('type').text

				if (cur_type == 'state') and (cur_main_type == 'state'):
					
					for elem in cur_param.findall('value'):
						cur_main_param.append(elem)

		if new_field:
			main_users.append(cur_user)	

# append a defined name to the end of each bodyname
def append_body_names(tree, str_end):

	root = tree.getroot()

	for name in root.iter('bodyname'):
		cur_name = str(name.text)

		if cur_name != 'base':
			name.text = '{}{}'.format(cur_name, str_end)

# append a defined name to the end of each jointname
def append_joint_names(tree, str_end):

	root = tree.getroot()

	for name in root.iter('jointname'):
		cur_name = str(name.text)
		name.text = '{}{}'.format(cur_name, str_end)

# append a defined name to the end of each pointname
def append_point_names(tree, str_end):

	root = tree.getroot()

	for name in root.iter('pointname'):
		cur_name = str(name.text)

		if cur_name != 'Ground_level_anchor':
			name.text = '{}{}'.format(cur_name, str_end)

# append a defined name to the end of each extforce name
def append_force_names(tree, str_end):

	root = tree.getroot()

	for ext in root.iter('extforce'):

		cur_name = ext.get('name')
		ext.set('name', '{}{}'.format(cur_name, str_end))

# change the robot position
def change_position(tree, end_name, new_T1, new_T2, new_T3, new_R1, new_R2, new_R3):

	root = tree.getroot()

	for body in root.iter('body'):
		for joint in body.findall('joint'):

			cur_name = joint.find('jointname').text

			if (cur_name == 'FJ_T1{}'.format(end_name)) or (cur_name == 'FJ_T2{}'.format(end_name)) or (cur_name == 'FJ_T3{}'.format(end_name)) or \
			   (cur_name == 'FJ_R1{}'.format(end_name)) or (cur_name == 'FJ_R2{}'.format(end_name)) or (cur_name == 'FJ_R3{}'.format(end_name)):

				cur_init = get_create(joint, 'initialvalue')
				cur_q = get_create(cur_init,'q')

				if cur_name == 'FJ_T1{}'.format(end_name):
					cur_q.text = str(new_T1)
				elif cur_name == 'FJ_T2{}'.format(end_name):
					cur_q.text = str(new_T2)
				elif cur_name == 'FJ_R3{}'.format(end_name):
					cur_q.text = str(new_R3)

# add color to a robot
def add_robot_color(tree, color_R, color_G, color_B):

	root = tree.getroot()

	for body in root.iter('body'):
		if body.find('bodyname').text == 'Main_frame':

			x3D_shapes = body.find('graphics').find('x3D').find('x3D_shapes')

			box = create(x3D_shapes, 'box')

			create_text(box, 'name', 'Robot_color_visu')

			position = create(box, 'position')
			create_text(position, 'x', 0.083)
			create_text(position, 'y', 0.0)
			create_text(position, 'z', 0.1565)

			scale = create(box, 'scale')
			create_text(scale, 'x', 1.0)
			create_text(scale, 'y', 1.0)
			create_text(scale, 'z', 1.0)

			rotation = create(box, 'rotation')
			create_text(rotation, 'x', 0.0)
			create_text(rotation, 'y', 0.0)
			create_text(rotation, 'z', 0.0)

			color = create(box, 'color')
			color.attrib['R'] = str(color_R)
			color.attrib['G'] = str(color_G)
			color.attrib['B'] = str(color_B)

			create_text(box, 'transparency', 0.0)

			size = create(box, 'size')
			create_text(size, 'x', 0.083)
			create_text(size, 'y', 0.083)
			create_text(size, 'z', 0.009)

# move a joint or a point (elem) by dx, dy in the Pad
def pad_move_2D_elem(elem, dx, dy):

	if elem.find('graphics') != None:

		x2D_pos = elem.find('graphics').find('x2D').find('position')

		x2D_pos.find('x').text = str(float(x2D_pos.find('x').text) + dx)
		x2D_pos.find('y').text = str(float(x2D_pos.find('y').text) + dy)

# mirror a joint or a point (elem) in the Pad
def pad_mirror_2D_elem(elem, x_flag):

	if elem.find('graphics') != None:

		x2D_pos = elem.find('graphics').find('x2D').find('position')

		if x_flag:
			x2D_pos.find('x').text = str(-float(x2D_pos.find('x').text))
		else:
			x2D_pos.find('y').text = str(-float(x2D_pos.find('y').text))

# move a body by dx, dy in the Pad
def pad_move_2D_body(body, dx, dy):

	for cur_joint in body.findall('joint'):
		pad_move_2D_elem(cur_joint, dx, dy)

	for cur_point in body.findall('point'):
		pad_move_2D_elem(cur_point, dx, dy)

# mirror a body in the Pad
def pad_mirror_2D_body(body, x_flag):

	for cur_joint in body.findall('joint'):
		pad_mirror_2D_elem(cur_joint, x_flag)

	for cur_point in body.findall('point'):
		pad_mirror_2D_elem(cur_point, x_flag)

# move a whole tree by dx, dy in the Pad
def pad_move_2D_tree(tree, dx, dy):

	bodytree = tree.getroot().find('bodytree')

	for cur_body in bodytree.findall('body'):
		pad_move_2D_body(cur_body, dx, dy)

# mirror a whole tree in the Pad
def pad_mirror_2D_tree(tree, x_flag):

	bodytree = tree.getroot().find('bodytree')

	for cur_body in bodytree.findall('body'):
		pad_mirror_2D_body(cur_body, x_flag)

# adapt the vrml anim path
def adapt_anim_path(tree):

	root = tree.getroot()

	for url in root.iter('url'):
		cur_line = url.text
		cur_line = cur_line.replace('PRJPATH/../animationR','PRJPATH/animationR')
		cur_line = cur_line.replace('PRJPATH/../../animationR','PRJPATH/animationR')
		cur_line = cur_line.replace('PRJPATH/../../../animationR','PRJPATH/animationR')
		url.text = cur_line

# print the mbs file
def print_mbs(tree, filename):

	out_temp = 'output/temp.xml'

	tree.write(out_temp)

	with open(filename, 'w') as mbs_file:
		with open(out_temp, 'r') as input_temp:	
			content = input_temp.read()
			mbs_file.seek(0,0)
			mbs_file.write('<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n')
			mbs_file.write(content)
			mbs_file.write('\r\n\r\n')

	os.remove(out_temp)


########################
##   MAIN MBS FILE   ###
########################

# project name
mbs_name = 'm454_project'

# mbs files to load
map_tree = ET.parse('../elem/map.mbs')
robot_tree_B = ET.parse('../elem/robot.mbs')
robot_tree_R = ET.parse('../elem/robot.mbs')
robot_tree_Y = ET.parse('../elem/robot.mbs')
robot_tree_W = ET.parse('../elem/robot.mbs')

# .mbs name
change_mbsname(map_tree, mbs_name)

# add color
add_robot_color(robot_tree_B,   0,  51, 204) # blue
add_robot_color(robot_tree_R, 204,   0,   0) # red
add_robot_color(robot_tree_Y, 255, 204,   0) # yellow
add_robot_color(robot_tree_W, 255, 255, 255) # white

# robot initial positions
pi = 3.14159265359
change_position(robot_tree_B, '_robot', 0.67,   1.15, 0.0, 0.0, 0.0, -pi/2)
change_position(robot_tree_R, '_robot', 0.82,    1.4, 0.0, 0.0, 0.0, -pi/2)
change_position(robot_tree_Y, '_robot', 0.67,  -1.15, 0.0, 0.0, 0.0,  pi/2)
change_position(robot_tree_W, '_robot', 0.82,   -1.4, 0.0, 0.0, 0.0,  pi/2)

# mirror robots (on the Pad)
pad_mirror_2D_tree(robot_tree_Y, 0)
pad_mirror_2D_tree(robot_tree_W, 0)

# move robots (on the Pad)
pad_move_2D_tree(robot_tree_B,   0,   0)
pad_move_2D_tree(robot_tree_R, 500,   0)
pad_move_2D_tree(robot_tree_Y,   0, 900)
pad_move_2D_tree(robot_tree_W, 500, 900)

# correct robot body names
append_body_names(robot_tree_B, '_B')
append_body_names(robot_tree_R, '_R')
append_body_names(robot_tree_Y, '_Y')
append_body_names(robot_tree_W, '_W')

# correct robot joint names
append_joint_names(robot_tree_B, '_B')
append_joint_names(robot_tree_R, '_R')
append_joint_names(robot_tree_Y, '_Y')
append_joint_names(robot_tree_W, '_W')

# correct robot point names
append_point_names(robot_tree_B, '_B')
append_point_names(robot_tree_R, '_R')
append_point_names(robot_tree_Y, '_Y')
append_point_names(robot_tree_W, '_W')

# correct robot force names
append_force_names(robot_tree_B, '_B')
append_force_names(robot_tree_R, '_R')
append_force_names(robot_tree_Y, '_Y')
append_force_names(robot_tree_W, '_W')

# add robots
add_all_bodies(map_tree, robot_tree_B)
add_all_bodies(map_tree, robot_tree_R)
add_all_bodies(map_tree, robot_tree_Y)
add_all_bodies(map_tree, robot_tree_W)

# add robots user models
add_all_user_models(map_tree, robot_tree_B)
add_all_user_models(map_tree, robot_tree_R)
add_all_user_models(map_tree, robot_tree_Y)
add_all_user_models(map_tree, robot_tree_W)

# adapt anim vrml files path
adapt_anim_path(map_tree)

# generate result
print_mbs(map_tree, 'output/{}.mbs'.format(mbs_name))



###########################
##  VISUALIZATION FILE  ###
###########################

# project name
anim_name = 'm454_project_anim'

# mbs files to load
target_1_a_tree = ET.parse('../elem/targets/target_1.mbs')
target_1_b_tree = ET.parse('../elem/targets/target_1.mbs')
target_1_c_tree = ET.parse('../elem/targets/target_1.mbs')
target_1_d_tree = ET.parse('../elem/targets/target_1.mbs')
target_2_a_tree = ET.parse('../elem/targets/target_2.mbs')
target_2_b_tree = ET.parse('../elem/targets/target_2.mbs')
target_2_c_tree = ET.parse('../elem/targets/target_2.mbs')
target_3_tree   = ET.parse('../elem/targets/target_3.mbs')

# .mbs name
change_mbsname(map_tree, anim_name)

# change position
change_position(target_1_a_tree, '_target_1',  0.7 ,  -0.6, 0.0, 0.0, 0.0, 0.0)
change_position(target_1_b_tree, '_target_1',  0.7 ,   0.6, 0.0, 0.0, 0.0, 0.0)
change_position(target_1_c_tree, '_target_1', -0.4 ,  -0.6, 0.0, 0.0, 0.0, 0.0)
change_position(target_1_d_tree, '_target_1', -0.4 ,   0.6, 0.0, 0.0, 0.0, 0.0)
change_position(target_2_a_tree, '_target_2',  0.25, -1.25, 0.0, 0.0, 0.0, 0.0)
change_position(target_2_b_tree, '_target_2',  0.25,  1.25, 0.0, 0.0, 0.0, 0.0)
change_position(target_2_c_tree, '_target_2',  0.1 ,   0.0, 0.0, 0.0, 0.0, 0.0)
change_position(target_3_tree  , '_target_3', -0.8 ,   0.0, 0.0, 0.0, 0.0, 0.0)

# move pad elements
pad_move_2D_tree(target_1_a_tree,  600, 250)
pad_move_2D_tree(target_1_b_tree,  700, 250)
pad_move_2D_tree(target_1_c_tree,  800, 250)
pad_move_2D_tree(target_1_d_tree,  900, 250)
pad_move_2D_tree(target_2_a_tree, 1000, 250)
pad_move_2D_tree(target_2_b_tree, 1100, 250)
pad_move_2D_tree(target_2_c_tree, 1200, 250)
pad_move_2D_tree(target_3_tree  , 1300, 250)


# change body names
append_body_names(target_1_a_tree, '_a')
append_body_names(target_1_b_tree, '_b')
append_body_names(target_1_c_tree, '_c')
append_body_names(target_1_d_tree, '_d')
append_body_names(target_2_a_tree, '_a')
append_body_names(target_2_b_tree, '_b')
append_body_names(target_2_c_tree, '_c')

# change joint names
append_joint_names(target_1_a_tree, '_a')
append_joint_names(target_1_b_tree, '_b')
append_joint_names(target_1_c_tree, '_c')
append_joint_names(target_1_d_tree, '_d')
append_joint_names(target_2_a_tree, '_a')
append_joint_names(target_2_b_tree, '_b')
append_joint_names(target_2_c_tree, '_c')

# add targets
add_all_bodies(map_tree, target_1_a_tree)
add_all_bodies(map_tree, target_1_b_tree)
add_all_bodies(map_tree, target_1_c_tree)
add_all_bodies(map_tree, target_1_d_tree)
add_all_bodies(map_tree, target_2_a_tree)
add_all_bodies(map_tree, target_2_b_tree)
add_all_bodies(map_tree, target_2_c_tree)
add_all_bodies(map_tree, target_3_tree)

# adapt anim vrml files path
adapt_anim_path(map_tree)

# generate result
print_mbs(map_tree, 'output/{}.mbs'.format(anim_name))
