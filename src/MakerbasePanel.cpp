#include "../include/MakerbasePanel.h"
#include "../include/KlippyGcodes.h"
#include "../include/MoonrakerAPI.h"
#include "../include/MakerbasePanel.h"

std::string home() {
    return json_run_a_gcode(HOME);
}

std::string homexy() {
    return json_run_a_gcode(HOME_XY);
}

std::string z_tilt() {
    return json_run_a_gcode(Z_TILT);
}

std::string quad_gantry_level() {
    return json_run_a_gcode(QUAD_GANTRY_LEVEL);
}

// Move 函数
/**
 * @brief 
 * 
 * @param axis 可选参数：AXIS_X, AXIS_Y, AXIS_Z
 * @param dist 距离与方向：如 +10, -100
 * @param speed 速度
 * @return std::string 
 */
std::string move(std::string axis, std::string dist, int speed) {
    return json_run_a_gcode(MOVE_RELATIVE + (std::string)"\n" + MOVE + (std::string)" "
                            + axis + dist + (std::string)" F" + std::to_string(speed * 60) + (std::string)"\nG90");
}

/* 这个地方写在这里合不合适？ */
std::string get_printer_object_status() {
    nlohmann::json objects;
    objects["webhook"];
    objects["gcode_move"];
    objects["toolhead"];
    objects["configfile"];
    objects["extruder"];
    objects["heater_bed"];
    objects["fan"];
    objects["idle_timeout"];
    objects["virtual_sdcard"];
    objects["print_stats"];
    objects["display_status"];
    // objects["temperature_sensor sensor_name"];
    // objects["filament_switch_sensor sensor_name"];
    // objects["output_pin pin_name"];
    objects["bed_mesh"];
    // objects["gcode_macro macro_name"];
    return json_query_printer_object_status(objects);
}