#ifndef MAKERBASE_PANEL_H
#define MAKERBASE_PANEL_H

#include <iostream>
#include "nlohmann/json.hpp"

#define AXIS_X "X"
#define AXIS_Y "Y"
#define AXIS_Z "Z"

std::string home();
std::string homexy();
std::string z_tilt();
std::string quad_gantry_level();

std::string move(std::string axis, std::string dist, int speed);

std::string get_printer_object_status();

#endif
