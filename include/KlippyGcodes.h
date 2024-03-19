#ifndef KLIPPY_GCODES_H
#define KILPPY_GCODES_H

#include <iostream>

#define AXIS_X "X"
#define AXIS_Y "Y"
#define AXIS_Z "Z"

#define HOME "G28"
#define HOME_X "G28 X"
#define HOME_Y "G28 Y"
#define HOME_Z "G28 Z"
#define HOME_XY "G28 X Y"
#define Z_TILT "Z_TILT_ADJUST"
#define QUAD_GANTRY_LEVEL "QUAD_GANTRY_LEVEL"

#define MOVE "G1"
#define MOVE_ABSOLUTE "G90"
#define MOVE_RELATIVE "G91"

#define EXTRUDE_ABS "M82"
#define EXTRUDE_REL "M83"

#define SET_EXT_TEMP "M104"
#define SET_BED_TEMP "M140"

#define SET_EXT_FACTOR "M221"
#define SET_FAN_SPEED "M106"
#define SET_SPD_FACTOR "M220"

#define PROBE_CALIBRATE "PROBE_CALIBRATE"
#define Z_ENDSTOP_CALIBRATE "Z_ENDSTOP_CALIBRATE"
#define TESTZ "TESTZ Z="
#define ABORT "ABORT"
#define ACCEPT "ACCEPT"

#define SAVE_CONFIG "SAVE_CONFIG"
#define RESTART "RESTART"
#define FIRMWARE_RESTART = "FIRMWARE_RESTART"

std::string set_bed_temp(int temp);
std::string set_ext_temp(int temp, int tool);
std::string set_heater_temp(std::string heater, int temp);
std::string set_temp_fan_temp(std::string temp_fan, int temp);
std::string set_fan_speed(int speed);

std::string set_fan0_speed(int speed);
std::string set_fan2_speed(int speed);
std::string set_fan3_speed(int speed);

std::string set_extrusion_rate(std::string rate);
std::string set_speed_rate(std::string rate);
std::string testz_move(std::string dist);
std::string extrude(std::string dist, int speed);
std::string bed_mesh_load(std::string profile);
std::string bed_mesh_remove(std::string profile);
std::string bed_mesh_save(std::string profile);

#endif
