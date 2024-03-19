#include "../include/KlippyGcodes.h"

std::string set_bed_temp(int temp) {
    return SET_BED_TEMP + (std::string)" S" + std::to_string(temp);
}

std::string set_ext_temp(int temp, int tool) {
    return SET_EXT_TEMP + (std::string)" T" + std::to_string(tool) + (std::string)" S" + std::to_string(temp);
}

std::string set_heater_temp(std::string heater, int temp) {
    return "SET_HEATER_TEMPERATURE heater=" + heater + " target=" + std::to_string(temp);
}

std::string set_temp_fan_temp(std::string temp_fan, int temp) {
    return "SET_TEMPERATURE_FAN_TARGET temperature_fan=" + temp_fan + " target=" + std::to_string(temp);
}

std::string set_fan_speed(int speed) {
    // std::cout << "设置速率的计算结果为 = " << ((float)(int(speed) % 101) / 100 * 255) << std::endl;
    // std::cout << "设置速率的计算结果为 = " << (int)((float)(int(speed) % 101) / 100 * 255) << std::endl;
    std::string speed_temp = std::to_string(int(float(int(speed) % 101) / 100 * 255));
    return SET_FAN_SPEED + (std::string)" S" + speed_temp;
}

/* Xindi */
std::string set_fan0_speed(int speed) {
    // std::string speed_temp = std::to_string(int(float(int(speed)) / 100 * 255));
    std::string speed_temp = std::to_string(float((float)(speed * 255) / 100));
    std::cout << speed_temp << std::endl;
    return "M106 P0 S" + speed_temp;
}

std::string set_fan2_speed(int speed) {
    std::string speed_temp = std::to_string(float((float)(speed * 255) / 100));
    return "M106 P2 S" + speed_temp;
}

std::string set_fan3_speed(int speed) {
    std::string speed_temp = std::to_string(float((float)(speed * 255) / 100));
    return "M106 P3 S" + speed_temp;
}

/* Xindi */

std::string set_extrusion_rate(std::string rate) {
    return SET_EXT_FACTOR + (std::string)" S" + rate;
}

std::string set_speed_rate(std::string rate) {
    return SET_SPD_FACTOR + (std::string)" S" + rate;
}

std::string testz_move(std::string dist) {
    return TESTZ + dist;
}

std::string extrude(std::string dist, int speed) {
    return MOVE + (std::string)" E" + dist + (std::string)" F" + std::to_string(speed);
}

std::string bed_mesh_load(std::string profile) {
    return "BED_MESH_PROFILE LOAD=" + profile;
}

std::string bed_mesh_remove(std::string profile) {
    return "BED_MESH_PROFILE REMOVE=" + profile;
}

std::string bed_mesh_save(std::string profile) {
    return "BED_MESH_PROFILE SAVE=" + profile;
}

