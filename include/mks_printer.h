#ifndef MKS_PRINTER_H
#define MKS_PRINTER_H

#include "nlohmann/json.hpp"

#include "./mks_log.h"

void parse_server_history_totals(nlohmann::json totals);
void parse_printer_probe(nlohmann::json probe);
void parse_printer_beep(nlohmann::json beep);
void parse_printer_caselight(nlohmann::json caselight);
void parse_printer_heater_fan_my_nozzle_fan1(nlohmann::json heater_fan_my_nozzle_fan1);
void parse_printer_out_pin_fan0(nlohmann::json out_pin_fan0);
void parse_printer_out_pin_fan2(nlohmann::json out_pin_fan2);
void parse_printer_out_pin_fan3(nlohmann::json out_pin_fan3);
void parse_filament_switch_sensor_fila(nlohmann::json filament_switch_sensor);
void parse_idle_timeout(nlohmann::json idle_timeout);
void parse_bed_mesh(nlohmann::json bed_mesh);
void parse_webhooks(nlohmann::json webhooks);
void parse_gcode_move(nlohmann::json gcode_move);
void parse_toolhead(nlohmann::json toolhead);
void parse_extruder(nlohmann::json extruder);
void parse_heater_bed(nlohmann::json heater_bed);
void parse_heater_generic_hot(nlohmann::json heater_generic_hot);
void parse_fan(nlohmann::json fan);
void parse_heater_fan(nlohmann::json heater_fan);
void parse_print_stats(nlohmann::json print_stats);
void parse_display_status(nlohmann::json display_status);
void parse_pause_resume(nlohmann::json pause_resume);

void parse_subscribe_objects_status(nlohmann::json status);

nlohmann::json subscribe_objects_status();

int get_cal_printing_time(int print_time, int estimated_time, int progress);
void parse_printer_info(nlohmann::json result);



#endif
