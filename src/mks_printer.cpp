#include <iostream>

#include "../include/mks_printer.h"

// mks ini data
bool mks_led_status;
bool mks_beep_status;
bool mks_fila_status;
int mks_language_status;
int mks_extruder_target;
int mks_heater_bed_target;
int mks_hot_target;
std::string mks_babystep_value;
std::string mks_adxl_offset;
std::string mks_version_soc;
std::string mks_version_mcu;
std::string mks_version_ui;

// webhooks
std::string printer_webhooks_state;             
std::string printer_webhooks_state_message;
std::string current_webhooks_state_message;

// gcode_move
float printer_gcode_move_speed_factor;
float printer_gcode_move_speed;
float printer_gcode_move_extrude_factor;
float printer_gcode_move_homing_origin[4];                  // [X, Y, Z, E] - 返回应用于每个轴的“gcode 偏移”。例如，可以检查“Z”轴以确定通过“babystepping”应用了多少偏移量。
float printer_gcode_move_position[4];
float printer_gcode_move_gcode_position[4];

// toolhead
std::string printer_toolhead_homed_axes;
float printer_toolhead_print_time;
float printer_toolhead_extimated_print_time;
double printer_toolhead_position[4];
double printer_toolhead_axis_minimum[4];
double printer_toolhead_axis_maximum[4];

// x, y, z坐标
double x_position;
double y_position;
double z_position;
// gcode, z坐标
double gcode_z_position;

float e_position;

// extruder
int printer_extruder_temperature = 0;
int printer_extruder_target = 0;

// heater bed
int printer_heater_bed_temperature = 0;
int printer_heater_bed_target = 0;

// hot
int printer_hot_temperature = 0;
int printer_hot_target = 0;

// fan
float printer_fan_speed;

// heater fan
float printer_heater_fan_speed;

// heater_fan my_nozzle_fan1
float printer_heater_fan_my_nozzle_fan1_speed;

// output_pin fan0
float printer_out_pin_fan0_value;

// output_pin fan2
float printer_out_pin_fan2_value;

float printer_out_pin_fan3_value;

float printer_out_pin_beep_value;

// idle_timeout
std::string printer_idle_timeout_state;
float printer_printing_time;

// virtual sdcard

// print stats
std::string printer_print_stats_filename;
float printer_print_stats_total_duration;
float printer_print_stats_print_duration;
float printer_print_stats_filament_used;
std::string printer_print_stats_state;          // 这个状态很有用
std::string printer_print_stats_message;        // error detected, error message

// display status
std::string printer_display_status_message;
int printer_display_status_progress = 0;

// temperature_sensor sensor_name

// temperature_fan fan_name

// filament_switch_sensor sensor_name

// output_pin pin_name

// bed_mesh
float auto_level_dist = 0.05;
bool auto_level_finished = false;
bool auto_level_enabled = false;

float manual_level_dist = 0.05;
int manual_level_count = 15;
bool manual_level_finished = false;
// bool manual_level_enabled = false;

float printer_bed_mesh_mesh_min[2] = {0.0, 0.0};
float printer_bed_mesh_mesh_max[2] = {0.0, 0.0};
float printer_bed_mesh_profiles_mks_points[5][5] = {0.0};
float printer_bed_mesh_profiles_mks_mesh_params_tension = 0.0;
float printer_bed_mesh_profiles_mks_mesh_params_mesh_x_pps = 0;
std::string printer_bed_mesh_profiles_mks_mesh_params_algo = "";
float printer_bed_mesh_profiles_mks_mesh_params_min_x = 0;
float printer_bed_mesh_profiles_mks_mesh_params_min_y = 0;
float printer_bed_mesh_profiles_mks_mesh_params_x_count = 0;
float printer_bed_mesh_profiles_mks_mesh_params_y_count = 0;
float printer_bed_mesh_profiles_mks_mesh_params_mesh_y_pps = 0;
float printer_bed_mesh_profiles_mks_mesh_params_max_x = 0;
float printer_bed_mesh_profiles_mks_mesh_params_max_y = 0;

float page_set_zoffset_x_y_position[16][2] = {
    {30,         30},                           // 1
    {30,      93.33},                           // 2
    {30,     156.66},                           // 3
    {30,     219.99},                           // 4
    {93.33,      30},                           // 5
    {93.33,   93.33},                           // 6
    {93.33,  156.66},                           // 7
    {93.33,  219.99},                           // 8
    {156.66,     30},                           // 9
    {156.66,  93.33},                           // 10
    {156.66, 156.66},                           // 11
    {156.66, 219.99},                           // 12
    {219.99,     30},                           // 13
    {219.99,  93.33},                           // 14
    {219.99, 156.66},                           // 15
    {219.99, 219.99}                            // 16
};

float page_set_zoffset_z_position[16] = {0};

bool fresh_page_set_zoffset_data = false;
bool refresh_page_auto_finish_data = false;
int page_set_zoffset_index;

// pause resume

int printer_pause_taget;

bool printer_pause_resume_is_paused;

float printer_set_offset = 0.01;
float printer_z_offset = 0.0;
float printer_intern_z_offset = 0.0;
float printer_extern_z_offset = 0.0;

float printer_move_dist = 10.0;
int printer_filament_extruder_target = 0;
int printer_filament_extruedr_dist = 50;

// filament switch sensor fila
bool filament_switch_sensor_fila_filament_detected = false;
bool filament_switch_sensor_fila_enabled = false;

// output_pin caselight
float printer_caselight_value = 0;

// probe
float printer_probe_x_zoffset = 0.0;
float printer_probe_y_zoffset = 0.0;
float printer_probe_z_zoffset = 0.0;

// printer info software version
std::string printer_info_software_version;

// server history totals
int total_jobs = 0;
double total_time = 0.0;
double total_print_time = 0.0;
double total_filament_used = 0.0;

// oobe
float oobe_printer_set_offset = 0.05;

void parse_server_history_totals(nlohmann::json totals) {
    /*
    if (totals["total_time"] != nlohmann::detail::value_t::null) {
        total_time = totals["total_time"];
    }
    */
    if (totals["total_print_time"] != nlohmann::detail::value_t::null) {
        total_print_time = totals["total_print_time"];
    }
    std::cout << "total_print_time = " << (int)(total_print_time) << std::endl;
}

void parse_printer_probe(nlohmann::json probe) {
    if (probe["x_offset"] != nlohmann::detail::value_t::null) {
        printer_probe_x_zoffset = probe["x_offset"];
    }
    if (probe["y_offset"] != nlohmann::detail::value_t::null) {
        printer_probe_y_zoffset = probe["y_offset"];
    }
    if (probe["z_offset"] != nlohmann::detail::value_t::null) {
        printer_probe_z_zoffset = probe["z_offset"];
    }
}

void parse_printer_beep(nlohmann::json beep) {
    if (beep["value"] != nlohmann::detail::value_t::null) {
        printer_out_pin_beep_value = beep["value"];
        MKSLOG_BLUE("printer_out_pin_beep_value = %f", printer_out_pin_beep_value);
    }
}

void parse_printer_caselight(nlohmann::json caselight) {
    if (caselight["value"] != nlohmann::detail::value_t::null) {
        printer_caselight_value = caselight["value"];
        MKSLOG_BLUE("printer_caselight_value = %f", printer_caselight_value);
    }
}

void parse_printer_heater_fan_my_nozzle_fan1(nlohmann::json heater_fan_my_nozzle_fan1) {
    if (heater_fan_my_nozzle_fan1["speed"] != nlohmann::detail::value_t::null) {
        printer_heater_fan_my_nozzle_fan1_speed = heater_fan_my_nozzle_fan1["speed"];
    }
}

void parse_printer_out_pin_fan0(nlohmann::json out_pin_fan0) {
    if (out_pin_fan0["speed"] != nlohmann::detail::value_t::null) {
        printer_out_pin_fan0_value = out_pin_fan0["speed"];
    }
    std::cout << "printer_out_pin_fan0_value " << printer_out_pin_fan0_value << std::endl;
};

void parse_printer_out_pin_fan2(nlohmann::json out_pin_fan2) {
    if (out_pin_fan2["speed"] != nlohmann::detail::value_t::null) {
        printer_out_pin_fan2_value = out_pin_fan2["speed"];
    }
    std::cout << "printer_out_pin_fan2_value " << printer_out_pin_fan2_value << std::endl;
};

void parse_printer_out_pin_fan3(nlohmann::json out_pin_fan3) {
    if (out_pin_fan3["speed"] != nlohmann::detail::value_t::null) {
        printer_out_pin_fan3_value = out_pin_fan3["speed"];
    }
    std::cout << "printer_out_pin_fan3_value " << printer_out_pin_fan3_value << std::endl;
};

void parse_filament_switch_sensor_fila(nlohmann::json filament_switch_sensor) {
    if (filament_switch_sensor["filament_detected"] != nlohmann::detail::value_t::null) {
        filament_switch_sensor_fila_filament_detected = filament_switch_sensor["filament_detected"];
        std::cout << "!!!! filament_detected: " << filament_switch_sensor_fila_filament_detected << std::endl;
    }
    if (filament_switch_sensor["enabled"] != nlohmann::detail::value_t::null) {
        filament_switch_sensor_fila_enabled = filament_switch_sensor["enabled"];
        std::cout << "!!!! enabled: " << filament_switch_sensor_fila_enabled << std::endl;
    }
}

void parse_idle_timeout(nlohmann::json idle_timeout) {
    if (idle_timeout["state"] != nlohmann::detail::value_t::null) {
        printer_idle_timeout_state = idle_timeout["state"];
        std::cout << "idle_timeout: " << printer_idle_timeout_state << std::endl;
        MKSLOG_RED("idle_timeout 发生变化: %s", printer_idle_timeout_state.c_str());
    }
}

void parse_bed_mesh(nlohmann::json bed_mesh) {
    if (bed_mesh["mesh_min"] != nlohmann::detail::value_t::null) {
        printer_bed_mesh_mesh_min[0] = bed_mesh["mesh_min"][0];
        printer_bed_mesh_mesh_min[1] = bed_mesh["mesh_min"][1];
    }
    if (bed_mesh["mesh_max"] != nlohmann::detail::value_t::null) {
        printer_bed_mesh_mesh_max[0] = bed_mesh["mesh_max"][0];
        printer_bed_mesh_mesh_max[1] = bed_mesh["mesh_max"][1];
    }
    if (bed_mesh["profiles"] != nlohmann::detail::value_t::null) {
        if (bed_mesh["profiles"]["default"] != nlohmann::detail::value_t::null) {
            if (bed_mesh["profiles"]["default"]["mesh_params"] != nlohmann::detail::value_t::null) {
                if (bed_mesh["profiles"]["default"]["mesh_params"]["tension"] != nlohmann::detail::value_t::null) {
                    printer_bed_mesh_profiles_mks_mesh_params_tension = bed_mesh["profiles"]["default"]["mesh_params"]["tension"];
                }
                if (bed_mesh["profiles"]["default"]["mesh_params"]["mesh_x_pps"] != nlohmann::detail::value_t::null) {
                    printer_bed_mesh_profiles_mks_mesh_params_mesh_x_pps = bed_mesh["profiles"]["default"]["mesh_params"]["mesh_x_pps"];
                }
                if (bed_mesh["profiles"]["default"]["mesh_params"]["algo"] != nlohmann::detail::value_t::null) {
                    printer_bed_mesh_profiles_mks_mesh_params_algo = bed_mesh["profiles"]["default"]["mesh_params"]["algo"];
                }
                if (bed_mesh["profiles"]["default"]["mesh_params"]["min_x"] != nlohmann::detail::value_t::null) {
                    printer_bed_mesh_profiles_mks_mesh_params_min_x = bed_mesh["profiles"]["default"]["mesh_params"]["min_x"];
                    std::cout << "printer_bed_mesh_profiles_mks_mesh_params_min_x = " << printer_bed_mesh_profiles_mks_mesh_params_min_x << std::endl;
                }
                if (bed_mesh["profiles"]["default"]["mesh_params"]["min_y"] != nlohmann::detail::value_t::null) {
                    printer_bed_mesh_profiles_mks_mesh_params_min_y = bed_mesh["profiles"]["default"]["mesh_params"]["min_y"];
                }
                if (bed_mesh["profiles"]["default"]["mesh_params"]["y_count"] != nlohmann::detail::value_t::null) {
                    printer_bed_mesh_profiles_mks_mesh_params_y_count = bed_mesh["profiles"]["default"]["mesh_params"]["y_count"];
                }
                if (bed_mesh["profiles"]["default"]["mesh_params"]["mesh_y_pps"] != nlohmann::detail::value_t::null) {
                    printer_bed_mesh_profiles_mks_mesh_params_mesh_y_pps = bed_mesh["profiles"]["default"]["mesh_params"]["mesh_y_pps"];
                }
                if (bed_mesh["profiles"]["default"]["mesh_params"]["x_count"] != nlohmann::detail::value_t::null) {
                    printer_bed_mesh_profiles_mks_mesh_params_x_count = bed_mesh["profiles"]["default"]["mesh_params"]["x_count"];
                }
                if (bed_mesh["profiles"]["default"]["mesh_params"]["max_x"] != nlohmann::detail::value_t::null) {
                    printer_bed_mesh_profiles_mks_mesh_params_max_x = bed_mesh["profiles"]["default"]["mesh_params"]["max_x"];
                }
                if (bed_mesh["profiles"]["default"]["mesh_params"]["max_y"] != nlohmann::detail::value_t::null) {
                    printer_bed_mesh_profiles_mks_mesh_params_max_y = bed_mesh["profiles"]["default"]["mesh_params"]["max_y"];
                }
            }
            
            if (bed_mesh["profiles"]["default"]["points"] != nlohmann::detail::value_t::null) {
                for (int i = 0; i < printer_bed_mesh_profiles_mks_mesh_params_y_count; i++) {
                    if (i == 5) {
                        break;
                    }
                    for (int j = 0; j < printer_bed_mesh_profiles_mks_mesh_params_x_count; j++) {
                        if (j == 5) {
                            break;
                        }
                        printer_bed_mesh_profiles_mks_points[i][j] = bed_mesh["profiles"]["default"]["points"][i][j];
                    }
                }
            }
        }
    }
}

void parse_webhooks(nlohmann::json webhooks) {
    if (webhooks["state"] != nlohmann::detail::value_t::null) {
        printer_webhooks_state = webhooks["state"];
    }
    // MKSLOG_RED("Webhooks state: %s", printer_webhooks_state);
    if (webhooks["state_message"] != nlohmann::detail::value_t::null) {
        printer_webhooks_state_message = webhooks["state_message"];
    }
    MKSLOG_RED("State message: %s", printer_webhooks_state_message.c_str());
}

void parse_gcode_move(nlohmann::json gcode_move) {
    if (gcode_move["speed_factor"] != nlohmann::detail::value_t::null) {
        printer_gcode_move_speed_factor = gcode_move["speed_factor"];
    }
    if (gcode_move["speed"] != nlohmann::detail::value_t::null) {
        printer_gcode_move_speed = gcode_move["speed"];
    }
    if (gcode_move["extrude_factor"] != nlohmann::detail::value_t::null) {
        printer_gcode_move_extrude_factor = gcode_move["extrude_factor"];
    }
    if (gcode_move["homing_origin"] != nlohmann::detail::value_t::null) {
        printer_gcode_move_homing_origin[0] = gcode_move["homing_origin"][0];
        printer_gcode_move_homing_origin[1] = gcode_move["homing_origin"][1];
        printer_gcode_move_homing_origin[2] = gcode_move["homing_origin"][2];
        printer_gcode_move_homing_origin[3] = gcode_move["homing_origin"][3];
    }
    if (gcode_move["gcode_position"] != nlohmann::detail::value_t::null) {
        // printer_gcode_move_gcode_position[0] = gcode_move["gcode_position"][0];
        // printer_gcode_move_gcode_position[1] = gcode_move["gcode_position"][1];
        printer_gcode_move_gcode_position[2] = gcode_move["gcode_position"][2];
        // printer_gcode_move_gcode_position[3] = gcode_move["gcode_position"][3];

        // std::cout << "printer_gcode_move_gcode_position[2] " << printer_gcode_move_gcode_position[2] << std::endl;

        gcode_z_position = round(printer_gcode_move_gcode_position[2] * 1000) / 1000;
    }
}

void parse_toolhead(nlohmann::json toolhead) {
    if (toolhead["position"] != nlohmann::detail::value_t::null) {
        printer_toolhead_position[0] = toolhead["position"][0];
        x_position = round(printer_toolhead_position[0] * 10) / 10;
        printer_toolhead_position[1] = toolhead["position"][1];
        y_position = round(printer_toolhead_position[1] * 10) / 10;
        printer_toolhead_position[2] = toolhead["position"][2];
        // std::cout << "printer_toolhead_position[2] " << printer_toolhead_position[2] << std::endl;
        // std::cout << "printer_toolhead_position[2] " << std::to_string(printer_toolhead_position[2]) << std::endl;
        z_position = round(printer_toolhead_position[2] * 10) / 10;
        // std::cout << "z_position: " << z_position << std::endl;
        printer_toolhead_position[3] = toolhead["position"][3];
        // std::cout << "printer_toolhead_position z == " << printer_toolhead_position << std::endl;
    }

    if (toolhead["axis_minimum"] != nlohmann::detail::value_t::null) {
        printer_toolhead_axis_minimum[0] = toolhead["axis_minimum"][0];
        printer_toolhead_axis_minimum[1] = toolhead["axis_minimum"][1];
        printer_toolhead_axis_minimum[2] = toolhead["axis_minimum"][2];
        printer_toolhead_axis_minimum[3] = toolhead["axis_minimum"][3];
    }

    if (toolhead["axis_maximum"] != nlohmann::detail::value_t::null) {
        printer_toolhead_axis_maximum[0] = toolhead["axis_maximum"][0];
        printer_toolhead_axis_maximum[1] = toolhead["axis_maximum"][1];
        printer_toolhead_axis_maximum[2] = toolhead["axis_maximum"][2];
        printer_toolhead_axis_maximum[3] = toolhead["axis_maximum"][3];
    }
}

void parse_extruder(nlohmann::json extruder) {
    float temp;
    if (extruder["temperature"] != nlohmann::detail::value_t::null) {
        temp = extruder["temperature"];
        printer_extruder_temperature = (int)(temp + 0.5);
        // printer_extruder_temperature = extruder["temperature"];
    }
    if (extruder["target"] != nlohmann::detail::value_t::null) {
        temp = extruder["target"];
        printer_extruder_target = (int)(temp + 0.5);
        // printer_extruder_target = extruder["target"];
    }
}

void parse_heater_bed(nlohmann::json heater_bed) {
    float temp;
    if (heater_bed["temperature"] != nlohmann::detail::value_t::null) {
        temp = heater_bed["temperature"];
        printer_heater_bed_temperature = (int)(temp + 0.5);
    }
    if (heater_bed["target"] != nlohmann::detail::value_t::null) {
        temp = heater_bed["target"];
        printer_heater_bed_target = (int)(temp + 0.5);
    }
}

void parse_heater_generic_hot(nlohmann::json heater_generic_hot) {
    float temp;
    if (heater_generic_hot["temperature"] != nlohmann::detail::value_t::null) {
        temp = heater_generic_hot["temperature"];
        printer_hot_temperature = (int)(temp + 0.5);
    }
    if (heater_generic_hot["target"] != nlohmann::detail::value_t::null) {
        temp = heater_generic_hot["target"];
        printer_hot_target = (int)(temp + 0.5);
    }
}

void parse_fan(nlohmann::json fan) {
    if (fan["speed"] != nlohmann::detail::value_t::null) {
        printer_fan_speed = fan["speed"];
    }
}

void parse_heater_fan(nlohmann::json heater_fan) {
    if (heater_fan["speed"] != nlohmann::detail::value_t::null) {
        printer_heater_fan_speed = heater_fan["speed"];
    }
}

void parse_print_stats(nlohmann::json print_stats) {
    if (print_stats["state"] != nlohmann::detail::value_t::null) {
        printer_print_stats_state = print_stats["state"];
        std::cout << "\033[31;1m" << "printer_print_stats_state = " << printer_print_stats_state << "\033[0m" << std::endl;
    }
    if (print_stats["filename"] != nlohmann::detail::value_t::null) {
        printer_print_stats_filename = print_stats["filename"];
        std::cout << "\033[31;1m" << "printer_print_stats_filename = " << printer_print_stats_filename << "\033[0m" << std::endl;
    }
    if (print_stats["print_duration"] != nlohmann::detail::value_t::null) {
        printer_print_stats_print_duration = print_stats["print_duration"];
    }
    if (print_stats["total_duration"] != nlohmann::detail::value_t::null) {
        printer_print_stats_total_duration = print_stats["total_duration"];
    }
}

void parse_display_status(nlohmann::json display_status) {
    double temp = 0.0;
    if (display_status["progress"] != nlohmann::detail::value_t::null) {
        temp = display_status["progress"];
        printer_display_status_progress = (int)(temp * 100);
    }
}

void parse_pause_resume(nlohmann::json pause_resume) {
    std::cout << pause_resume << std::endl;
    if (pause_resume["is_paused"] != nlohmann::detail::value_t::null) {
        printer_pause_resume_is_paused = pause_resume["is_paused"];
    }
}

// 解析订阅对象状态
void parse_subscribe_objects_status(nlohmann::json status) {
    if (status["idle_timeout"] != nlohmann::detail::value_t::null) {
        parse_idle_timeout(status["idle_timeout"]);
    }
    if (status["bed_mesh"] != nlohmann::detail::value_t::null) {
        std::cout << status["bed_mesh"] << std::endl;
        parse_bed_mesh(status["bed_mesh"]);
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                std::cout << "############# Points :" << i << ", " << j << " " << printer_bed_mesh_profiles_mks_points[i][j] << std::endl;
            }
        }
    }
    if (status["webhooks"] != nlohmann::detail::value_t::null) {
        // MKSLOG("Webhooks update");
        parse_webhooks(status["webhooks"]);
    }
    if (status["gcode_move"] != nlohmann::detail::value_t::null) {
        // MKSLOG("Gcode move update");
        parse_gcode_move(status["gcode_move"]);
    }
    if (status["toolhead"] != nlohmann::detail::value_t::null) {
        // MKSLOG("toolhead update");
        parse_toolhead(status["toolhead"]);
    }
    if (status["extruder"] != nlohmann::detail::value_t::null) {
        // MKSLOG("Extruder update");
        parse_extruder(status["extruder"]);
    }
    if (status["heater_bed"] != nlohmann::detail::value_t::null) {
        // MKSLOG("Heater bed update");
        parse_heater_bed(status["heater_bed"]);
    }
    if (status["heater_generic chamber"] != nlohmann::detail::value_t::null) {
        parse_heater_generic_hot(status["heater_generic chamber"]);
    }
    if (status["fan"] != nlohmann::detail::value_t::null) {
        // MKSLOG("Fan update");
        parse_fan(status["fan"]);
    }
    if (status["heater_fan fan1"] != nlohmann::detail::value_t::null) {
        parse_heater_fan(status["heater_fan fan1"]);
    }
    if (status["pause_resume"] != nlohmann::detail::value_t::null) {
        parse_pause_resume(status["pause_resume"]);
    }
    if (status["print_stats"] != nlohmann::detail::value_t::null) {
        // MKSLOG("Print stats update");
        parse_print_stats(status["print_stats"]);
    }
    if (status["display_status"] != nlohmann::detail::value_t::null) {
        // MKSLOG("Display status update");
        parse_display_status(status["display_status"]);
    }
    if (status["heater_fan my_nozzle_fan1"] != nlohmann::detail::value_t::null) {
        parse_printer_heater_fan_my_nozzle_fan1(status["heater_fan my_nozzle_fan1"]);
    }
    if (status["fan_generic cooling_fan"] != nlohmann::detail::value_t::null) {
        parse_printer_out_pin_fan0(status["fan_generic cooling_fan"]);
    }
    if (status["fan_generic auxiliary_cooling_fan"] != nlohmann::detail::value_t::null) {
        parse_printer_out_pin_fan2(status["fan_generic auxiliary_cooling_fan"]);
    }
    if (status["fan_generic chamber_circulation_fan"] != nlohmann::detail::value_t::null) {
        parse_printer_out_pin_fan3(status["fan_generic chamber_circulation_fan"]);
    }
    if (status["filament_switch_sensor fila"] != nlohmann::detail::value_t::null) {
        parse_filament_switch_sensor_fila(status["filament_switch_sensor fila"]);
    }
    if (status["output_pin caselight"] != nlohmann::detail::value_t::null) {
        parse_printer_caselight(status["output_pin caselight"]);
    }
    if (status["output_pin sound"] != nlohmann::detail::value_t::null) {
        parse_printer_beep(status["output_pin sound"]);
    }
    if (status["probe"] != nlohmann::detail::value_t::null) {
        parse_printer_probe(status["probe"]);
    }
}

nlohmann::json subscribe_objects_status() {
    nlohmann::json objects;
    objects["extruder"];
    objects["heater_generic chamber"];
    objects["heater_bed"];
    objects["gcode_move"];
    objects["fan"] = {"speed"};
    objects["heater_fan fan1"] = {"speed"};
    objects["toolhead"];
    objects["print_stats"] = {"print_duration", "total_duration", "filament_used", "filename", "state", "message"};
    objects["display_status"] = {"progress", "message"};
    objects["idle_timeout"] = {"state"};
    objects["pause_resume"] = {"is_paused"};
    objects["webhooks"] = {"state", "state_message"};
    // objects["webhooks"];
    objects["firmware_retraction"] = {"retract_length", "retract_speed", "unretract_extra_length",
                                      "unretract_speed"};
    objects["bed_mesh"];
    objects["heater_fan my_nozzle_fan1"];
    objects["filament_switch_sensor fila"] = {"filament_detected", "enabled"};
    objects["fan_generic cooling_fan"] = {"speed", "rpm"};
    objects["fan_generic auxiliary_cooling_fan"] = {"speed", "rpm"};
    objects["fan_generic chamber_circulation_fan"] = {"speed", "rpm"};
    objects["output_pin caselight"];
    objects["output_pin sound"];
    objects["probe"];
    return objects;
}

int get_cal_printing_time(int print_time, int estimated_time, int progress) {
    int left_time;
    int total_time = 0;
    if (progress <= 10) {
        total_time = estimated_time;
        left_time = total_time - print_time;
    } else if (progress > 10) {
        total_time = (print_time) * 100 / (progress);
        left_time = total_time - print_time;
    }
    return left_time;
}

void parse_printer_info(nlohmann::json result) {
    if (result["software_version"] != nlohmann::detail::value_t::null) {
        // std::cout << result["software_version"] << std::endl;
        printer_info_software_version = result["software_version"];
        MKSLOG_RED("Version: %s", printer_info_software_version.c_str());
    }
}


