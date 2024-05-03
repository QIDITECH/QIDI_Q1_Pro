#include <iostream>
#include <set>
#include <stack>
#include <algorithm>
// #include <boost/algorithm/string.hpp>

#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>

#include <math.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <fstream>
#include <string>
#include <sstream>
#include <map>

#include "../include/KlippyGcodes.h"
#include "../include/send_msg.h"
#include "../include/event.h"
#include "../include/mks_printer.h"
#include "../include/mks_file.h"
#include "../include/mks_update.h"
#include "../include/mks_wpa_cli.h"
#include "../include/mks_test.h"
#include "../include/MakerbaseSerial.h"
#include "../include/MakerbaseClient.h"
#include "../include/MakerbaseShell.h"
#include "../include/MakerbasePanel.h"
#include "../include/MakerbaseParseIni.h"
#include "../include/MakerbaseWiFi.h"
#include "../include/MakerbaseNetwork.h"
#include "../include/ui.h"
#include "../include/send_jpg.h"

#include <nlohmann/json.hpp>

#define DEFAULT_DIR "gcodes/"

extern int tty_fd;

extern MakerbaseClient *ep;

extern bool gimage_is_showed;
extern bool simage_is_showed;

extern int current_page_id;                    // 当前页面的id号
extern int previous_page_id;                   // 上一页面的id号
extern int next_page_id;                       // 下一页面的id号

/* Printer Extern Varible*/
extern bool mks_led_status;
extern bool mks_beep_status;
extern bool mks_fila_status;
extern int mks_language_status;
extern int mks_extruder_target;
extern int mks_heater_bed_target;
extern int mks_hot_target;
extern std::string mks_babystep_value;
extern std::string mks_adxl_offset;
extern std::string mks_version_soc;
extern std::string mks_version_mcu;
extern std::string mks_version_ui;

extern std::string printer_webhooks_state;             
extern std::string printer_webhooks_state_message;
extern std::string current_webhooks_state_message;
extern float printer_gcode_move_speed_factor;
float current_speed_factor = 0;
extern float printer_gcode_move_speed;
float current_extruder_factor = 0;
extern float printer_gcode_move_extrude_factor;
extern float printer_gcode_move_homing_origin[4];                  // [X, Y, Z, E] - 返回应用于每个轴的“gcode 偏移”。例如，可以检查“Z”轴以确定通过“babystepping”应用了多少偏移量。
extern float printer_gcode_move_position[4];
extern float printer_gcode_move_gcode_position[4];
extern std::string printer_toolhead_homed_axes;
extern float printer_toolhead_print_time;
extern float printer_toolhead_extimated_print_time;
extern double printer_toolhead_position[4];
extern double printer_toolhead_axis_minimum[4];
extern double printer_toolhead_axis_maximum[4];
extern double x_position;
extern double y_position;
extern double z_position;
extern float e_position;
extern double gcode_z_position;

extern int printer_extruder_temperature;
int current_extruder_temperature = 0;
extern int printer_extruder_target;
int current_extruder_target = 0;
extern int printer_heater_bed_temperature;
int current_heater_bed_temperature = 0;
extern int printer_heater_bed_target;
int current_heater_bed_target = 0;
extern int printer_hot_temperature;
int current_hot_temperature = 0;
extern int printer_hot_target;
int current_hot_target = 0;
extern float printer_fan_speed;
extern float printer_heater_fan_speed;
extern float printer_heater_fan_my_nozzle_fan1_speed;
extern float printer_out_pin_fan0_value;
float current_out_pin_fan0_value = 0;

extern float printer_out_pin_fan3_value;
float current_out_pin_fan3_value =0;

extern float printer_out_pin_fan2_value;
float current_out_pin_fan2_value = 0;
extern float printer_out_pin_beep_value;
extern std::string printer_idle_timeout_state;
extern float printer_printing_time;
extern std::string printer_print_stats_filename;
extern float printer_print_stats_total_duration;
extern float printer_print_stats_print_duration;
extern float printer_print_stats_filament_used;
extern std::string printer_print_stats_state;          // 这个状态很有用
extern std::string printer_print_stats_message;        // error detected, error message
extern std::string printer_display_status_message;
extern int printer_display_status_progress;

extern float auto_level_dist;
extern bool auto_level_finished;
extern bool auto_level_enabled;

std::string str_manual_level_offset;
extern float manual_level_dist;
extern int manual_level_count;
extern bool manual_level_finished;

extern float printer_bed_mesh_mesh_min[2];
extern float printer_bed_mesh_mesh_max[2];
extern float printer_bed_mesh_profiles_mks_points[5][5];
extern float printer_bed_mesh_profiles_mks_mesh_params_tension;
extern float printer_bed_mesh_profiles_mks_mesh_params_mesh_x_pps;
extern std::string printer_bed_mesh_profiles_mks_mesh_params_algo;
extern float printer_bed_mesh_profiles_mks_mesh_params_min_x;
extern float printer_bed_mesh_profiles_mks_mesh_params_min_y;
extern float printer_bed_mesh_profiles_mks_mesh_params_x_count;
extern float printer_bed_mesh_profiles_mks_mesh_params_y_count;
extern float printer_bed_mesh_profiles_mks_mesh_params_mesh_y_pps;
extern float printer_bed_mesh_profiles_mks_mesh_params_max_x;
extern float printer_bed_mesh_profiles_mks_mesh_params_max_y;

extern float page_set_zoffset_x_y_position[16][2];

extern float page_set_zoffset_z_position[16];

extern bool fresh_page_set_zoffset_data;
extern bool refresh_page_auto_finish_data;
extern int page_set_zoffset_index;

// filament switch sensor fila
extern bool filament_switch_sensor_fila_filament_detected;
extern bool filament_switch_sensor_fila_enabled;

extern float printer_caselight_value;

extern int printer_pause_taget;

extern bool printer_pause_resume_is_paused;

extern float printer_set_offset;
extern float printer_z_offset;
extern float printer_intern_z_offset;
extern float printer_extern_z_offset;

extern float printer_move_dist;
extern int printer_filament_extruder_target;
extern int printer_filament_extruedr_dist;

// probe
extern float printer_probe_x_zoffset;
extern float printer_probe_y_zoffset;
extern float printer_probe_z_zoffset;

// printer info software version
extern std::string printer_info_software_version;

/* wifi */
extern std::string page_wifi_ssid_list[5];
extern int page_wifi_current_pages;
extern int page_wifi_ssid_list_pages;
extern std::string get_wifi_name;
/* 文件列表处理相关变量 */
// extern std::set<std::string> page_files_dirname_filename_list;

extern bool filelist_changed;

extern int page_files_pages;
extern int page_files_current_pages;
extern int page_files_folder_layers;
extern std::string page_files_previous_path;
extern std::string page_files_root_path;
extern std::string page_files_path;

extern std::string page_files_list_name[9];                                     // 文件列表显示文件名称
extern std::string page_files_list_show_name[9];                                // 文件列表名称
extern std::string page_files_list_show_type[9];        // 文件类型: [f]或者[d]，或者[n]

extern std::stack<std::string> page_files_path_stack;          // 路径栈
extern std::string page_files_root_path;                       // Klippy根目录
extern std::string page_files_previous_path;                   // 之前的路径
extern std::string page_files_path;                            // 文件所在路径
extern std::string page_files_print_files_path;                // 要打印的文件路径

extern int file_metadata_estimated_time;
extern std::string file_metadata_filename;
extern float file_metadata_filament_total;
extern int file_metadata_object_height;
extern std::string file_metadata_filament_name;
extern std::string file_metadata_filament_type;
extern float file_metadata_filament_weight_total;
extern std::string file_metadata_gimage;
extern std::string file_metadata_simage;

extern int level_mode;

extern bool printing_keyboard_enabled;
// extern bool filament_keyboard_enabled;
extern bool auto_level_button_enabled;
extern bool manual_level_button_enabled;

extern bool show_preview_complete;
extern bool show_preview_gimage_completed;

/* 补偿值 */
float page_set_zoffset_number;

/* 共振补偿值 */
std::string page_syntony_shaper_freq_x = "";
std::string page_syntony_shaper_freq_y = "";
bool page_syntony_finished = false;

/* PID调节 */
bool page_pid_finished = false;

/* wifi */
extern struct mks_wifi_status_result_t status_result;
extern struct mks_wifi_status_t wpa_status;

std::string wifi_ip_address;

/* 打完关机 */
bool page_printing_shutdown_enable = false;

/* wifi键盘打开判断 */
extern bool printing_wifi_keyboard_enabled;


/* refresh about success*/
bool page_about_successed = false;

extern bool is_download_to_screen;          // main.cpp 里面的变量

/* update page */
extern int filesize;

/* server history totals */
extern int total_jobs;
extern double total_time;
extern double total_print_time;
extern double total_filament_used;

/* 800个小时 */
int mks_total_printed_minutes;

/* 设置打印调平的目标温度 */
extern int level_mode_printing_extruder_target;
extern int level_mode_printing_heater_bed_target;

extern bool level_mode_printing_is_printing_level;

/* wifi 相关 */
std::string current_connected_ssid_name = "";

extern bool page_wifi_list_ssid_button_enabled[5];

// page print filament
extern bool page_print_filament_extrude_restract_button;
extern bool page_filament_extrude_button;

// oobe开箱引导
bool mks_oobe_enabled = false;
bool current_mks_oobe_enabled = false;

// 调平
bool start_pre_auto_level = false;
bool start_pre_manual_level = false;

bool all_level_saving = false;

extern bool mks_file_parse_finished;
// reset
// extern bool page_reset_to_about;

// 正在保存页面
bool is_refresh_page_saving = false;

std::string temp_idle_state = "";

// oobe
extern float oobe_printer_set_offset;

//extern std::string printed_file_path;

bool jump_to_print = false;

int printer_auto_level_heater_bed_target;

extern std::string filament_message;

bool printer_ready = false;

extern bool page_filament_unload_button;

extern bool printer_bed_leveling;

//4.4.1 CLL 耗材确认弹窗新增不再提示按钮
extern bool preview_pop_1_on;
extern bool preview_pop_2_on;

//4.4.1 CLL 修复断料检测与退料冲突bug
bool previous_filament_sensor_state = false;

//4.4.1 CLL 修改调平
std::string previous_zoffset;

//4.4.2 CLL 新增息屏功能
extern bool previous_caselight_value;

//4.4.2 CLL 修改调平不再需要设置zoffset值
bool step_1 = false;
bool step_2 = false;
bool step_3 = false;
bool step_4 = false;

//4.4.2 CLL 适配mates和霍尔耗材宽度探测器
bool filament_detected = true;

//4.4.2 CLL 文件列表新增本地、U盘切换按钮
extern std::string file_mode;

extern int manual_count;

float bed_offset;

extern std::string adjust_mode;
extern std::string set_mode; 

extern bool move_fan_setting;

extern std::string thumbnail_path;

extern bool cache_clicked;

int unhomed_move_mode = 0; // CLL 用于保存上次按下的移动按钮，当触发需先归位操作时进行移动, 1(x up),2(x down),3(y up),4(y down),5(z up),6(z down)

// CLL 以下变量用于修复gcode响应函数和refresh函数之间的冲突(gcode响应函数需设置变量在refresh_page_show中统一改变页面，否则会产生冲突)
bool jump_to_move_pop_1 = false;
bool jump_to_move_pop_2 = false;
bool jump_to_detect_error = false;
bool jump_to_level_error = false;
bool jump_to_filament_pop_1 = false;
bool jump_to_print_low_temp = false;
bool jump_to_memory_warning = false;
bool jump_to_resume_print = false;

std::string error_message;

bool main_picture_detected = false; // CLL 此变量用于检测cache中图片是否存在
bool main_picture_refreshed = false; // CLL 此变量用于标记主页面图片是否已经刷新过，防止反复刷新

extern int load_target;

int connection_method; // CLL 此变量用于记录二维码是否是局域网刷新 0为局域网连接，1为互联网连接

std::map<int, Server_config> serverConfigs;
std::string selected_server;

int current_server_page;
int total_server_count;
std::string target_soc_version;

bool open_qr_refreshed = false; // CLL 第一次开机刷新时不使用脚本重新生成二维码图片
extern bool qr_refreshed;
bool open_reprint_asked = false; // CLL 开机后进入主页面发出检测断电续打指令

int mks_ethernet;

/* 更新页面处理 */
void refresh_page_show() {
    // CLL 以下if检测函数为无条件跳转，其在改变为true之前已经经过函数判断(跳转页面前需将对应变量置为false,否则会陷入死循环)
    if (jump_to_move_pop_1 == true) {
        jump_to_move_pop_1 = false;
        page_to(TJC_PAGE_MOVE_POP_1);
    }
    if (jump_to_move_pop_2 == true) {
        switch (unhomed_move_mode)
        {
        case 1: // X_UP
            ep->Send(json_run_a_gcode("SET_KINEMATIC_POSITION Z=150\nSET_KINEMATIC_POSITION X=150\nSET_KINEMATIC_POSITION Y=150\n"));
            ep->Send(json_run_a_gcode("G91\nG1 X10 F3000\nG90\nM84\n"));
            break;
        
        case 2: // X_DOWN
            ep->Send(json_run_a_gcode("SET_KINEMATIC_POSITION Z=150\nSET_KINEMATIC_POSITION X=150\nSET_KINEMATIC_POSITION Y=150\n"));
            ep->Send(json_run_a_gcode("G91\nG1 X-10 F3000\nG90\nM84\n"));
            break;

        case 3: // Y_UP
            ep->Send(json_run_a_gcode("SET_KINEMATIC_POSITION Z=150\nSET_KINEMATIC_POSITION X=150\nSET_KINEMATIC_POSITION Y=150\n"));
            ep->Send(json_run_a_gcode("G91\nG1 Y10 F3000\nG90\nM84\n"));
            break;

        case 4: // Y_DOWN
            ep->Send(json_run_a_gcode("SET_KINEMATIC_POSITION Z=150\nSET_KINEMATIC_POSITION X=150\nSET_KINEMATIC_POSITION Y=150\n"));
            ep->Send(json_run_a_gcode("G91\nG1 Y-10 F3000\nG90\nM84\n"));
            break;

        case 5: // Z_UP
            ep->Send(json_run_a_gcode("SET_KINEMATIC_POSITION Z=150\nSET_KINEMATIC_POSITION X=150\nSET_KINEMATIC_POSITION Y=150\n"));
            ep->Send(json_run_a_gcode("G91\nG1 Z-10 F600\nG90\nM84\n"));
            break;
        
        case 6: // Z_DOWN
            ep->Send(json_run_a_gcode("SET_KINEMATIC_POSITION Z=150\nSET_KINEMATIC_POSITION X=150\nSET_KINEMATIC_POSITION Y=150\n"));
            ep->Send(json_run_a_gcode("G91\nG1 Z10 F600\nG90\nM84\n"));
            break;
        
        default:
            break;
        }
        unhomed_move_mode = 0;
        jump_to_move_pop_2 = false;
        page_to(TJC_PAGE_MOVE_POP_2);
    }
    if (jump_to_detect_error == true) {
        jump_to_detect_error = false;
        page_to(TJC_PAGE_DETECT_ERROR);
        send_cmd_txt(tty_fd, "t0", error_message);
    }
    if (jump_to_level_error == true) {
        jump_to_level_error = false;
        page_to(TJC_PAGE_LEVEL_ERROR);
    }
    if (jump_to_filament_pop_1 == true) {
        jump_to_filament_pop_1 = false;
        page_to(TJC_PAGE_FILAMENT_POP_1);
    }
    if (jump_to_print_low_temp == true) {
        jump_to_print_low_temp = false;
        page_to(TJC_PAGE_PRINT_LOW_TEMP);
    }
    if (jump_to_resume_print == true) {
        jump_to_resume_print = false;
        page_to(TJC_PAGE_RESUME_PRINT);
    }
    if (jump_to_memory_warning == true) {
        jump_to_memory_warning = false;
        page_to(TJC_PAGE_MEMORY_WARNING);
    }
    
    if (current_page_id != TJC_PAGE_PRINTING) {
        switch (current_page_id)
        {
        case TJC_PAGE_PRINT_ZOFFSET:
            break;
        
        case TJC_PAGE_PRINT_FILAMENT:
            break;
        
        case TJC_PAGE_PRINT_STOP:
        case TJC_PAGE_PRINT_NO_FILAMENT:
        case TJC_PAGE_PRINT_NO_FILAMENT_2:
        case TJC_PAGE_SHUTDOWN:
        case TJC_PAGE_PRINT_STOPPING:
        case TJC_PAGE_MOVE_POP_1:
        case TJC_PAGE_GCODE_ERROR:
        case TJC_PAGE_DETECT_ERROR:
        case TJC_PAGE_RESET:
        case TJC_PAGE_PREVIEW:
        case TJC_PAGE_PREVIEW_POP_1:
        case TJC_PAGE_PREVIEW_POP_2:
        case TJC_PAGE_PRINTING_2:
        case TJC_PAGE_FILAMENT_POP_2:
        case TJC_PAGE_FILAMENT_POP_3:
            break;
        
        default:
            if (printer_print_stats_state == "printing") {
                if (printer_print_stats_filename != "") {
                    main_picture_detected = false;
                    main_picture_refreshed = false;
                    MKSLOG_BLUE("跳入到打印函数\n");
                    sleep(1);
                    get_file_estimated_time(printer_print_stats_filename);
                    sleep(1);
                    //4.4.2 CLL 新增息屏功能
                    if (previous_caselight_value == true) {
                        led_on_off();
                        previous_caselight_value = false;
                    }
                    jump_to_print = true;
                    printer_ready = false;
                    page_to(TJC_PAGE_PREVIEW);
                    break;
                }
            }
            break;
        }
    }

    if (current_page_id != TJC_PAGE_RESET) {
        switch (current_page_id)
        {
        case TJC_PAGE_GCODE_ERROR:
        case TJC_PAGE_DETECT_ERROR:
        case TJC_PAGE_LEVEL_ERROR:
        case TJC_PAGE_SHUTDOWN:
        case TJC_PAGE_SERVICE:
        case TJC_PAGE_LANGUAGE:
        case TJC_PAGE_COMMON_SETTING:
        case TJC_PAGE_SLEEP_MODE:
        case TJC_PAGE_INTERNET:
        case TJC_PAGE_WIFI_LIST:
        case TJC_PAGE_WIFI_KB:
        case TJC_PAGE_WIFI_CONNECT:
        case TJC_PAGE_WIFI_FAILED:
        case TJC_PAGE_WIFI_SUCCESS:
        case TJC_PAGE_WIFI_SAVING:
        case TJC_PAGE_UPDATE_FOUND:
        case TJC_PAGE_UPDATE_NOT_FOUND:
        case TJC_PAGE_UPDATING:
        case TJC_PAGE_UPDATE_FINISH:
        case TJC_PAGE_RESTORE_CONFIG:
        case TJC_PAGE_SHOW_QR:
        case TJC_PAGE_SERVER_SET:
        case TJC_PAGE_UPDATE_MODE:
        case TJC_PAGE_ONLINE_UPDATE:
        case TJC_PAGE_SEARCH_SERVER:
            break;
        
        default:
            // 喷头板断开的情况下跳到重启页面
            if (printer_webhooks_state == "shutdown" || printer_webhooks_state == "error") {
                if (printer_webhooks_state == "shutdown" && (current_page_id == TJC_PAGE_AUTO_MOVING || current_page_id == TJC_PAGE_OPEN_CALIBRATE)) {
                    break;
                }
                if (previous_caselight_value == true) {
                    led_on_off();
                    previous_caselight_value = false;
                }
                page_to(TJC_PAGE_RESET);
                std::cout << "重启页面" << std::endl;
                if (current_webhooks_state_message != printer_webhooks_state_message) {
                    current_webhooks_state_message = printer_webhooks_state_message;
                    std::string temp = printer_webhooks_state_message;
                    replace(temp.begin(), temp.end(), '\n', '.');
                    replace(temp.begin(), temp.end(), '\'', ' ');
                    replace(temp.begin(), temp.end(), '\"', ' ');
                    send_cmd_txt(tty_fd, "t0", temp);
                }
            }
            break;
        }
    } else if (current_page_id == TJC_PAGE_RESET) {
        if (printer_webhooks_state == "shutdown" || printer_webhooks_state == "error") {
            
            if (current_webhooks_state_message != printer_webhooks_state_message) {
                current_webhooks_state_message = printer_webhooks_state_message;
                std::string temp = printer_webhooks_state_message;
                replace(temp.begin(), temp.end(), '\n', '.');
                replace(temp.begin(), temp.end(), '\'', ' ');
                replace(temp.begin(), temp.end(), '\"', ' ');
                send_cmd_txt(tty_fd, "t0", temp);
            }
        }
        if (printer_webhooks_state == "ready") {
            page_to(TJC_PAGE_SYS_OK);
        }
    }

    switch (current_page_id)
    {
    case TJC_PAGE_MAIN:
        refresh_page_main();
        break;

    case TJC_PAGE_PREVIEW:
        refresh_page_preview();
        break;

    case TJC_PAGE_PRINTING:
    case TJC_PAGE_PRINTING_2:
        refresh_page_printing();
        break;

    case TJC_PAGE_PRINT_FILAMENT:
        refresh_page_print_filament();
        break;

    case TJC_PAGE_MOVE:
        refresh_page_move();
        break;

    case TJC_PAGE_PRINT_ZOFFSET:
        refresh_page_printing_zoffset();
        break;

    case TJC_PAGE_AUTO_MOVING:
        refresh_page_auto_moving();
        break;

    case TJC_PAGE_AUTO_FINISH:
        refresh_page_auto_finish();
        break;

    case TJC_PAGE_SYNTONY_MOVE:
        refresh_page_syntony_move();
        break;

    case TJC_PAGE_SYNTONY_FINISH:
        // refresh_page_syntony_finish();
        break;

    case TJC_PAGE_PRINT_STOPPING:
        refresh_page_stopping();
        break;

    case TJC_PAGE_PRE_BED_CALIBRATION:
        refresh_page_auto_level();
        break;

    case TJC_PAGE_WIFI_LIST:
        refresh_page_wifi_list_2();
        break;

    case TJC_PAGE_OPEN_FILAMENTVIDEO_2:
        refresh_page_open_filament_video_2();
        break;

    case TJC_PAGE_ZOFFSET:
        refresh_page_zoffset();
        break;

    case TJC_PAGE_AUTO_HEATERBED:
        refresh_page_auto_heaterbed();
        break;

    case TJC_PAGE_OPEN_HEATERBED:
        refresh_page_open_heaterbed();
        break;

    case TJC_PAGE_FILAMENT_POP_2:
    case TJC_PAGE_FILAMENT_POP_3:
        refresh_page_filament_pop();
        break;

    case TJC_PAGE_PREVIEW_POP_1:
    case TJC_PAGE_PREVIEW_POP_2:
        refresh_page_preview_pop();
        break;
    
    //4.4.2 CLL 文件列表新增本地、U盘切换按钮
    case TJC_PAGE_FILE_LIST:
        // refresh_page_files_list_2();
        break;

    case TJC_PAGE_BED_MOVING:
        refresh_page_bed_moving();
        break;

    case TJC_PAGE_OPEN_CALIBRATE:
        refresh_page_open_calibrate();
        break;

    case TJC_PAGE_COMMON_SETTING:
        refresh_page_common_setting();
        break;

    case TJC_PAGE_FILAMENT_SET_FAN:
        refresh_page_filament_set_fan();
        break;

    case TJC_PAGE_WIFI_KB:
        refresh_page_wifi_keyboard();
        break;

    case TJC_PAGE_FILAMENT:
        refresh_page_filament();
        break;

    case TJC_PAGE_SHOW_QR:
        refresh_page_show_qr();
        break;

    case TJC_PAGE_SERVER_SET:
        refresh_page_server_set();
        break;

    case TJC_PAGE_AUTO_UNLOAD:
        refresh_page_auto_unload();
        break;

    default:
        break;
    }
}

void refresh_page_open_filament_video_2() {
    if (printer_extruder_target == 0) {
        send_cmd_pco(tty_fd, "t0", "65535");
        send_cmd_pco(tty_fd, "n0", "65535");
        send_cmd_picc(tty_fd, "b3", "9");
        send_cmd_picc2(tty_fd, "b3", "25");
    } else {
        send_cmd_pco(tty_fd, "t0", "63488");
        send_cmd_pco(tty_fd, "n0", "63488");
        send_cmd_picc(tty_fd, "b3", "10");
        send_cmd_picc2(tty_fd, "b3", "24");
    }

    send_cmd_txt(tty_fd, "t0", std::to_string(printer_extruder_temperature) + "/");
    send_cmd_val(tty_fd, "n0", std::to_string(printer_extruder_target));
}

void refresh_page_wifi_keyboard() {
    if (printing_wifi_keyboard_enabled == true) {
        send_cmd_txt(tty_fd, "t0", get_wifi_name);
    }
}

void refresh_page_syntony_finish() {
    MKSLOG_BLUE("Printer ide_timeout state: %s", printer_idle_timeout_state.c_str());
    MKSLOG_BLUE("Printer webhooks state: %s", printer_webhooks_state.c_str());
    if (page_syntony_finished == false) {
        page_syntony_finished = true;
        all_level_saving = false;
    }
    
    if (printer_idle_timeout_state == "Ready" && printer_webhooks_state == "ready") {
        MKSLOG_BLUE("Printer webhooks state: %s", printer_webhooks_state.c_str());
        sleep(10);
        system("sync");             // 保存好配置文件
        
        all_level_saving = false;
        init_mks_status();      // 重启后初始化之前保存的参数
        sub_object_status();
        get_object_status();
        sleep(10);
        page_to(TJC_PAGE_LEVEL_MODE);
        MKSLOG_RED("从739行跳出去");
    }
}

void refresh_page_auto_level() {
    if (auto_level_dist == (float)0.01) {
        send_cmd_picc(tty_fd, "b0", "113");
        send_cmd_picc(tty_fd, "b1", "114");
        send_cmd_picc(tty_fd, "b2", "114");
        send_cmd_picc(tty_fd, "b3", "114");
        send_cmd_picc2(tty_fd, "b0", "116");
        send_cmd_picc2(tty_fd, "b1", "115");
        send_cmd_picc2(tty_fd, "b2", "115");
        send_cmd_picc2(tty_fd, "b3", "115");
    } else if (auto_level_dist == (float)0.05) {
        send_cmd_picc(tty_fd, "b0", "114");
        send_cmd_picc(tty_fd, "b1", "113");
        send_cmd_picc(tty_fd, "b2", "114");
        send_cmd_picc(tty_fd, "b3", "114");
        send_cmd_picc2(tty_fd, "b0", "115");
        send_cmd_picc2(tty_fd, "b1", "116");
        send_cmd_picc2(tty_fd, "b2", "115");
        send_cmd_picc2(tty_fd, "b3", "115");
    } else if (auto_level_dist == (float)0.1) {
        send_cmd_picc(tty_fd, "b0", "114");
        send_cmd_picc(tty_fd, "b1", "114");
        send_cmd_picc(tty_fd, "b2", "113");
        send_cmd_picc(tty_fd, "b3", "114");
        send_cmd_picc2(tty_fd, "b0", "115");
        send_cmd_picc2(tty_fd, "b1", "115");
        send_cmd_picc2(tty_fd, "b2", "116");
        send_cmd_picc2(tty_fd, "b3", "115");
    } else if (auto_level_dist == (float)0.5) {
        send_cmd_picc(tty_fd, "b0", "114");
        send_cmd_picc(tty_fd, "b1", "114");
        send_cmd_picc(tty_fd, "b2", "114");
        send_cmd_picc(tty_fd, "b3", "113");
        send_cmd_picc2(tty_fd, "b0", "115");
        send_cmd_picc2(tty_fd, "b1", "115");
        send_cmd_picc2(tty_fd, "b2", "115");
        send_cmd_picc2(tty_fd, "b3", "116");
    }
}

void refresh_page_stopping() {
    MKSLOG_BLUE("Printer ide_timeout state: %s", printer_idle_timeout_state.c_str());
    MKSLOG_BLUE("Printer webhooks state: %s", printer_webhooks_state.c_str());
    if (printer_idle_timeout_state == "Ready") {
        clear_previous_data();
        sleep(5);
        save_current_zoffset();
        page_to(TJC_PAGE_MAIN);
    }
}

void refresh_page_syntony_move() {
    if (temp_idle_state != printer_idle_timeout_state) {
        temp_idle_state = printer_idle_timeout_state;
        MKSLOG_BLUE("Printer ide_timeout state: %s", printer_idle_timeout_state.c_str());
        MKSLOG_BLUE("Printer webhooks state: %s", printer_webhooks_state.c_str());
    }
    
    if (step_1 == true) {
        sleep(15);
        page_to(TJC_PAGE_SYNTONY_FINISH);
        step_1 = false;
    }
}

void refresh_page_print_filament() {
    send_cmd_txt(tty_fd, "t5", printer_print_stats_filename.substr(printer_print_stats_filename.rfind("/") +1 ));

    if (printer_extruder_target == 0) {
        send_cmd_pco(tty_fd, "t2", "65535");
        send_cmd_picc(tty_fd, "b0", "63");
        send_cmd_picc2(tty_fd, "b0", "64");
    } else {
        send_cmd_pco(tty_fd, "t2", "63488");
        send_cmd_picc(tty_fd, "b0", "62");
        send_cmd_picc2(tty_fd, "b0", "65");
    }

    send_cmd_val(tty_fd, "j0", std::to_string(printer_display_status_progress));
    send_cmd_val(tty_fd, "n7", std::to_string(printer_display_status_progress));
    send_cmd_txt(tty_fd, "t2", std::to_string(printer_extruder_temperature));
    send_cmd_txt(tty_fd, "t3", std::to_string(printer_extruder_target));
    send_cmd_txt(tty_fd, "t0", show_time((int)(printer_print_stats_print_duration)));
    send_cmd_txt(tty_fd, "t1", show_time(get_cal_printing_time((int)(printer_print_stats_print_duration), file_metadata_estimated_time, printer_display_status_progress)));
    
    if (printer_print_stats_state == "paused") {
        printer_ready = true;
    }

    //4.4.2 CLL 适配mates和霍尔耗材宽度检测器
    if (filament_detected == false) {
        sleep(1);
        printer_ready = false;
        set_print_pause();
        page_to(TJC_PAGE_PRINT_NO_FILAMENT);
    }

    if (printer_print_stats_state == "printing") {
        if (printer_ready == true) {
            printer_ready = false;
            page_to(TJC_PAGE_PRINTING);
        }
    }

    if (printer_print_stats_state == "standby") {
        page_to(TJC_PAGE_PRINT_STOPPING);
    }

    if (printer_print_stats_state == "error") {
        page_to(TJC_PAGE_GCODE_ERROR);
        cancel_print();
        clear_previous_data();
        send_cmd_txt(tty_fd, "t0", "gcode error:" + error_message);
    }

    //4.4.2 CLL 长时间暂停使打印停止后会跳转页面
    if (printer_idle_timeout_state == "Idle") {
        ep->Send(json_run_a_gcode("G28\n"));
        cancel_print();
    }
}

void refresh_page_auto_finish() {
    if ( printer_idle_timeout_state == "Idle" && printer_webhooks_state == "ready" ) {
        auto_level_finished = true;
    }
}

void refresh_page_auto_moving() {
    send_cmd_txt(tty_fd, "t5", "(" + std::to_string(printer_heater_bed_temperature) + "/" + std::to_string(printer_heater_bed_target) + ")");
    if (step_1 == true) {
        send_cmd_picc(tty_fd, "q0", "109");
        send_cmd_pco(tty_fd, "t1", "65535");
        send_cmd_pco(tty_fd, "t0", "38066");
        send_cmd_vis(tty_fd, "gm1", "0");
        send_cmd_vis(tty_fd, "gm2", "1");
        step_1 = false;
    }
    if (step_2 == true) {
        send_cmd_picc(tty_fd, "q0", "110");
        send_cmd_pco(tty_fd, "t2", "65535");
        send_cmd_pco(tty_fd, "t1", "38066");
        send_cmd_vis(tty_fd, "gm2", "0");
        send_cmd_vis(tty_fd, "gm3", "1");
        step_2 = false;
    }
    if (step_3 == true) {
        send_cmd_picc(tty_fd, "q0", "111");
        send_cmd_pco(tty_fd, "t3", "65535");
        send_cmd_pco(tty_fd, "t2", "38066");
        send_cmd_vis(tty_fd, "gm3", "0");
        send_cmd_vis(tty_fd, "gm4", "1");
        step_3 = false;
        printer_idle_timeout_state = "Printing";
        get_mks_heater_bed_target();
        set_heater_bed_target(mks_heater_bed_target);
        ep->Send(json_run_a_gcode("M190 S" + std::to_string(mks_heater_bed_target) + "\n"));
        sleep(1);
        ep->Send(json_run_a_gcode("M4027\n"));
    }
    if (step_4 == true) {
        sleep(15);
        page_to(TJC_PAGE_AUTO_FINISH);
        step_4 = false;
    }
}

void refresh_page_move() {
    std::string x_pos = std::to_string(x_position);
    x_pos = x_pos.substr(0, x_pos.find(".") + 2);
    std::string y_pos = std::to_string(y_position);
    y_pos = y_pos.substr(0, y_pos.find(".") + 2);
    std::string z_pos = std::to_string(z_position);
    z_pos = z_pos.substr(0, z_pos.find(".") + 2);

    send_cmd_txt(tty_fd, "t0", x_pos);
    send_cmd_txt(tty_fd, "t1", y_pos);
    send_cmd_txt(tty_fd, "t2", z_pos);

    // CLL 刷新所选择距离
    if (printer_move_dist == (float)0.1) {
        send_cmd_picc(tty_fd, "b0", "77");
        send_cmd_picc2(tty_fd, "b0", "74");
        send_cmd_picc(tty_fd, "b1", "76");
        send_cmd_picc2(tty_fd, "b1", "75");
        send_cmd_picc(tty_fd, "b2", "76");
        send_cmd_picc2(tty_fd, "b2", "75");
    } else if (printer_move_dist == (float)1.0) {
        send_cmd_picc(tty_fd, "b0", "76");
        send_cmd_picc2(tty_fd, "b0", "75");
        send_cmd_picc(tty_fd, "b1", "77");
        send_cmd_picc2(tty_fd, "b1", "74");
        send_cmd_picc(tty_fd, "b2", "76");
        send_cmd_picc2(tty_fd, "b2", "75");
    } else if (printer_move_dist == (float)10) {
        send_cmd_picc(tty_fd, "b0", "76");
        send_cmd_picc2(tty_fd, "b0", "75");
        send_cmd_picc(tty_fd, "b1", "76");
        send_cmd_picc2(tty_fd, "b1", "75");
        send_cmd_picc(tty_fd, "b2", "77");
        send_cmd_picc2(tty_fd, "b2", "74");
    }
    
}

void refresh_page_offset(float intern_zoffset) {
    printer_intern_z_offset = intern_zoffset;
    printer_z_offset = printer_intern_z_offset + printer_extern_z_offset;
}

void refresh_page_printing_zoffset() {
    std::string z_offset = std::to_string(printer_gcode_move_homing_origin[2]);
    std::string show_gcode_z = std::to_string(gcode_z_position);
    z_offset = z_offset.substr(0, z_offset.find(".") + 4);
    show_gcode_z = show_gcode_z.substr(0, show_gcode_z.find(".") + 4);
    send_cmd_txt(tty_fd, "t4", printer_print_stats_filename.substr(printer_print_stats_filename.rfind("/") +1 ));
    if (z_offset != mks_babystep_value) {
        mks_babystep_value = z_offset;
        set_mks_babystep(mks_babystep_value);
    }
    send_cmd_txt(tty_fd, "t2", show_gcode_z);
    send_cmd_txt(tty_fd, "t3", z_offset);
    send_cmd_txt(tty_fd, "t0", show_time((int)(printer_print_stats_print_duration)));
    send_cmd_val(tty_fd, "n7", std::to_string(printer_display_status_progress));
    send_cmd_txt(tty_fd, "t1", show_time(get_cal_printing_time((int)(printer_print_stats_print_duration), file_metadata_estimated_time, printer_display_status_progress)));
    send_cmd_val(tty_fd, "j0", std::to_string(printer_display_status_progress));

    if (printer_set_offset == (float)0.01) {
        send_cmd_picc(tty_fd, "b1", std::to_string(57));
        send_cmd_picc2(tty_fd, "b1", std::to_string(60));
        send_cmd_picc(tty_fd, "b2", std::to_string(58));
        send_cmd_picc2(tty_fd, "b2", std::to_string(59));
        send_cmd_picc(tty_fd, "b3", std::to_string(58));
        send_cmd_picc2(tty_fd, "b3", std::to_string(59));
        send_cmd_picc(tty_fd, "b4", std::to_string(58));
        send_cmd_picc2(tty_fd, "b4", std::to_string(59));
    } else if (printer_set_offset == (float)0.05) {
        send_cmd_picc(tty_fd, "b1", std::to_string(58));
        send_cmd_picc2(tty_fd, "b1", std::to_string(59));
        send_cmd_picc(tty_fd, "b2", std::to_string(57));
        send_cmd_picc2(tty_fd, "b2", std::to_string(60));
        send_cmd_picc(tty_fd, "b3", std::to_string(58));
        send_cmd_picc2(tty_fd, "b3", std::to_string(59));
        send_cmd_picc(tty_fd, "b4", std::to_string(58));
        send_cmd_picc2(tty_fd, "b4", std::to_string(59));
    } else if (printer_set_offset == (float)0.1) {
        send_cmd_picc(tty_fd, "b1", std::to_string(58));
        send_cmd_picc2(tty_fd, "b1", std::to_string(59));
        send_cmd_picc(tty_fd, "b2", std::to_string(58));
        send_cmd_picc2(tty_fd, "b2", std::to_string(59));
        send_cmd_picc(tty_fd, "b3", std::to_string(57));
        send_cmd_picc2(tty_fd, "b3", std::to_string(60));
        send_cmd_picc(tty_fd, "b4", std::to_string(58));
        send_cmd_picc2(tty_fd, "b4", std::to_string(59));
    }  else if (printer_set_offset == (float)0.5) {
        send_cmd_picc(tty_fd, "b1", std::to_string(58));
        send_cmd_picc2(tty_fd, "b1", std::to_string(59));
        send_cmd_picc(tty_fd, "b2", std::to_string(58));
        send_cmd_picc2(tty_fd, "b2", std::to_string(59));
        send_cmd_picc(tty_fd, "b3", std::to_string(58));
        send_cmd_picc2(tty_fd, "b3", std::to_string(59));
        send_cmd_picc(tty_fd, "b4", std::to_string(57));
        send_cmd_picc2(tty_fd, "b4", std::to_string(60));
    }

    if (printer_print_stats_state == "printing") {
        printer_ready = true;
    }
    
    if (filament_switch_sensor_fila_enabled == true) {
        if (filament_switch_sensor_fila_filament_detected == false) {
            printer_ready = false;
            set_print_pause();
            page_to(TJC_PAGE_PRINT_NO_FILAMENT_2);
        }
    }

    //4.4.2 CLL 适配mates与霍尔耗材宽度检测器
    if (filament_detected == false) {
        sleep(1);
        printer_ready = false;
        set_print_pause();
        page_to(TJC_PAGE_PRINT_NO_FILAMENT);
    }

    if (printer_print_stats_state == "standby") {
        page_to(TJC_PAGE_PRINT_STOPPING);
    }

    if (printer_print_stats_state == "paused") {
        if (printer_ready == true) {
            printer_ready = false;
            page_to(TJC_PAGE_PRINT_FILAMENT);
        }
    }
    
    if (printer_print_stats_state == "complete") {
            std::string time_duration = show_time((int)(printer_print_stats_print_duration));
            complete_print();
            clear_previous_data();
            sleep(5);
            save_current_zoffset();
            page_to(TJC_PAGE_PRINT_FINISH);
            send_cmd_txt(tty_fd, "t1", time_duration);
    }

    if (printer_print_stats_state == "error") {
        page_to(TJC_PAGE_GCODE_ERROR);
        cancel_print();
        clear_previous_data();
        send_cmd_txt(tty_fd , "t0", "gcode error:" + error_message);
    }
}

void refresh_page_printing() {
    std::string z_offset = std::to_string(printer_gcode_move_homing_origin[2]);
    z_offset = z_offset.substr(0, z_offset.find(".") + 4);

    send_cmd_val(tty_fd, "j0", std::to_string(printer_display_status_progress));
    send_cmd_val(tty_fd, "n7", std::to_string(printer_display_status_progress));
    send_cmd_txt(tty_fd, "t2", show_time((int)(printer_print_stats_print_duration)));
    send_cmd_txt(tty_fd, "t3", show_time(get_cal_printing_time((int)(printer_print_stats_print_duration), file_metadata_estimated_time, printer_display_status_progress)));
    send_cmd_txt(tty_fd, "t4", printer_print_stats_filename.substr(printer_print_stats_filename.rfind("/") +1 ));
    
    if (printing_keyboard_enabled == false) { // CLL 当未进入键盘时重复执行刷新函数
        if (current_page_id == TJC_PAGE_PRINTING) {
            // CLL 刷新风扇速率
            send_cmd_val(tty_fd, "n4", std::to_string((int)(printer_out_pin_fan0_value * 100)));
            send_cmd_val(tty_fd, "n5", std::to_string((int)(printer_out_pin_fan2_value * 100)));
            send_cmd_val(tty_fd, "n6", std::to_string((int)(printer_out_pin_fan3_value * 100)));

            send_cmd_txt(tty_fd, "t0", std::to_string(printer_extruder_temperature));
            send_cmd_val(tty_fd, "n0", std::to_string(printer_extruder_target));
            if (printer_extruder_target == 0) { // CLL 根据喷头是否加热改变按钮和数字颜色
                send_cmd_pco(tty_fd, "t0", "65535");
                send_cmd_picc(tty_fd, "b0", "53");
                send_cmd_picc2(tty_fd, "b0", "51");
            } else {
                send_cmd_pco(tty_fd, "t0", "63488");
                send_cmd_picc(tty_fd, "b0", "54");
                send_cmd_picc2(tty_fd, "b0", "52");
            }

            send_cmd_txt(tty_fd, "t1", std::to_string(printer_heater_bed_temperature));
            send_cmd_val(tty_fd, "n1", std::to_string(printer_heater_bed_target));
            if (printer_heater_bed_target == 0) { // CLL 根据热床是否加热改变按钮和数字颜色
                send_cmd_pco(tty_fd, "t1", "65535");
                send_cmd_picc(tty_fd, "b1", "53");
                send_cmd_picc2(tty_fd, "b1", "51");
            } else {
                send_cmd_pco(tty_fd, "t1", "63488");
                send_cmd_picc(tty_fd, "b1", "54");
                send_cmd_picc2(tty_fd, "b1", "52");
            }

            if (printer_caselight_value == 0) { // CLL 判断当前LED状态
                send_cmd_picc(tty_fd, "b3", "53");
                send_cmd_picc2(tty_fd, "b3", "52");
            } else {
                send_cmd_picc(tty_fd, "b3", "54");
                send_cmd_picc2(tty_fd, "b3", "51");
            }

            send_cmd_val(tty_fd, "n2", std::to_string(printer_hot_target)); // CLL 获取热腔温度
            send_cmd_txt(tty_fd, "t5", std::to_string(printer_hot_temperature));
            if (printer_hot_target == 0) { // CLL 根据热床是否加热改变按钮和数字颜色
                send_cmd_pco(tty_fd, "t5", "65535");
                send_cmd_picc(tty_fd, "b7", "53");
                send_cmd_picc2(tty_fd, "b7", "51");
            } else {
                send_cmd_pco(tty_fd, "t5", "63488");
                send_cmd_picc(tty_fd, "b7", "54");
                send_cmd_picc2(tty_fd, "b7", "52");
            }

            if (show_preview_gimage_completed == true) {
                send_cmd_vis(tty_fd, "cp0", "1");
                send_cmd_val(tty_fd, "v999", "1");
            } else {
                send_cmd_vis(tty_fd, "cp0", "0");
                send_cmd_val(tty_fd, "v999", "0");
            }
        } else if (current_page_id == TJC_PAGE_PRINTING_2) {
            send_cmd_txt(tty_fd, "t1", z_offset); //刷新zoffset值

            if (current_speed_factor != printer_gcode_move_speed_factor) { // CLL 刷新移动速率
                current_speed_factor = printer_gcode_move_speed_factor;
                send_cmd_val(tty_fd, "n2", std::to_string((int)(round(printer_gcode_move_speed_factor * 100))));
            }

            if (current_extruder_factor != printer_gcode_move_extrude_factor) { // CLL 刷新挤出速率
                current_extruder_factor = printer_gcode_move_extrude_factor;
                send_cmd_val(tty_fd, "n3", std::to_string((int)(round(printer_gcode_move_extrude_factor * 100))));
            }
        }
    }
    
    if (printer_print_stats_state == "printing") {
        printer_ready = true;
    }

    if (filament_switch_sensor_fila_enabled == true) {
        if (filament_switch_sensor_fila_filament_detected == false) {
            printer_ready = false;
            set_print_pause();
            page_to(TJC_PAGE_PRINT_NO_FILAMENT_2);
        }
    }

    if (filament_detected == false) {
        sleep(1);
        printer_ready = false;
        set_print_pause();
        page_to(TJC_PAGE_PRINT_NO_FILAMENT);
    }

    if (printer_print_stats_state == "complete") {
        std::string time_duration = show_time((int)(printer_print_stats_print_duration));
        complete_print();
        clear_previous_data();
        sleep(5);
        save_current_zoffset();
        page_to(TJC_PAGE_PRINT_FINISH);
        send_cmd_txt(tty_fd, "t1", time_duration);
    }

    if (printer_print_stats_state == "paused") {
        if (printer_ready == true) {
            printer_ready = false;
            page_to(TJC_PAGE_PRINT_FILAMENT);
        }
    }

    if (printer_print_stats_state == "standby") {
        page_to(TJC_PAGE_PRINT_STOPPING);
    }

    if (printer_print_stats_state == "error") {
        page_to(TJC_PAGE_GCODE_ERROR);
        cancel_print();
        clear_previous_data();
        send_cmd_txt(tty_fd, "t0", "gcode error:" + error_message);
    }
}

void clear_page_printing_arg() {
    current_extruder_temperature = 0;
    current_extruder_target = 0;
    current_heater_bed_target = 0;
    current_heater_bed_temperature = 0;
    current_hot_target = 0;
    current_hot_temperature = 0;
    current_out_pin_fan0_value = 0;
    current_out_pin_fan2_value = 0;
    current_out_pin_fan3_value = 0;

    current_speed_factor = 0;
    current_extruder_factor = 0;
}

void refresh_page_preview() {
    if (printer_bed_leveling == true) {
        send_cmd_picc(tty_fd, "b2", "45");
        send_cmd_picc2(tty_fd, "b2", "45");
    }else {
        send_cmd_picc(tty_fd, "b2", "46");
        send_cmd_picc2(tty_fd, "b2", "46");
    }
    if (mks_file_parse_finished == true) {
        if (show_preview_complete == false) {
            //4.4.2 CLL 修改当前在预览图界面仅能看到文件名称
            send_cmd_txt(tty_fd, "t0", file_metadata_filename.substr(file_metadata_filename.rfind("/") + 1));
            if (file_metadata_estimated_time) {
                send_cmd_txt(tty_fd, "t1", show_time(file_metadata_estimated_time));
            } else {
                send_cmd_txt(tty_fd, "t1", "-");
            }

            if (file_metadata_filament_weight_total) {
                std::string temp = std::to_string(file_metadata_filament_weight_total);
                send_cmd_txt(tty_fd, "t2", temp.substr(0, temp.find(".") + 2) + "g");
            } else {
                send_cmd_txt(tty_fd, "t2", "-");
            }

            if (file_metadata_filament_total) {
                std::string temp = std::to_string(file_metadata_filament_total / 1000);
                send_cmd_txt(tty_fd, "t3", temp.substr(0, temp.find(".") + 2) + "m");
            } else {
                send_cmd_txt(tty_fd, "t3", "-");
            }

            if (file_metadata_filament_type != "") {
                send_cmd_txt(tty_fd, "t4", file_metadata_filament_type);
            } else if (file_metadata_filament_name != ""){
                send_cmd_txt(tty_fd, "t4", file_metadata_filament_name);
            } else {
                send_cmd_txt(tty_fd, "t4", "-");
            }
            
            bool path_found = false;
            std::string picture_path;
            if (jump_to_print == true) {
                picture_path = "/.cache/.thumbs/" + printer_print_stats_filename.substr(printer_print_stats_filename.rfind("/") + 1, printer_print_stats_filename.rfind(".") - (printer_print_stats_filename.rfind("/") + 1));
            } else if (cache_clicked == true) {
                picture_path = page_files_path_stack.top() + "/.cache/.thumbs/" + file_metadata_filename.substr(file_metadata_filename.rfind("/") + 1, file_metadata_filename.rfind(".") - (file_metadata_filename.rfind("/") + 1));
                cache_clicked = false;
            } else {
                picture_path = page_files_path_stack.top() + "/.thumbs/" + file_metadata_filename.substr(file_metadata_filename.rfind("/") + 1, file_metadata_filename.rfind(".") - (file_metadata_filename.rfind("/") + 1));
            }
            if (path_found == false) {
                picture_path = picture_path.substr(1);
                MKSLOG_RED("picture_path:%s", picture_path.c_str());
                char relative_path[1024];
                sprintf(relative_path, "/home/mks/gcode_files/%s-160x160.png", picture_path.c_str()); // CLL 先在目录下寻找168x168像素图片
                MKSLOG("relative_path:%s", relative_path);
                if (access(relative_path, F_OK) == 0) {
                    path_found = true;
                    picture_path = picture_path + "-160x160.png";
                } else {
                    sprintf(relative_path, "/home/mks/gcode_files/%s-160x160.jpg",picture_path.c_str()); // CLL 再在目录下寻找32x32像素图片
                    if (access(relative_path, F_OK) == 0) {
                        path_found = true;
                        picture_path = picture_path + "-160x160.jpg";
                    }
                }
            }
            MKSLOG_BLUE("图片路径:%s", picture_path.c_str());
            if (picture_path == "") {
                path_found = false;
            }

            if (path_found == true) {
                // 刷小图
                if (show_preview_gimage_completed == false) {
                    output_imgdata(picture_path, 160);
                    std::ifstream simage_infile("/home/mks/tjc");
                    if (!simage_infile) {
                        std::cerr << "无法打开文件 " << "/home/mks/tjc" << std::endl;
                        show_preview_complete = true;
                        return;
                    }
                    std::stringstream simage_buffer;
                    simage_buffer << simage_infile.rdbuf();
                    file_metadata_simage = simage_buffer.str();
                    simage_infile.close();
                    send_cmd_txt(tty_fd, "preview.cp0_text", "");
                    send_cmd_txt(tty_fd, "preview.add", "");
                    if (file_metadata_simage != "") { 
                        send_cmd_baud(tty_fd, 921600);
                        usleep(50000);
                        set_option(tty_fd, 921600, 8, 'N', 1);
                        std::cout << "刷小图" << std::endl;
                        int num = 2048;
                        int len = file_metadata_simage.length();
                        int end = num;
                        std::string s;
                        for (int start = 0; start < len;) {
                            if (end > len) {
                                s = file_metadata_simage.substr(start, len - start);
                                send_cmd_txt(tty_fd, "add", s);
                                tcdrain(tty_fd);
                                send_cmd_txt_plus(tty_fd, "cp0_text", "cp0_text", "add");
                                tcdrain(tty_fd);
                                break;
                            }
                            s = file_metadata_simage.substr(start, num);
                            start = end;
                            end = end + num;
                            send_cmd_txt(tty_fd, "add", s);
                            tcdrain(tty_fd);
                            send_cmd_txt_plus(tty_fd, "cp0_text", "cp0_text", "add");
                            tcdrain(tty_fd);
                            // usleep(50000);
                        }
                        send_cmd_baud(tty_fd, 115200);
                        usleep(50000);
                        set_option(tty_fd, 115200, 8, 'N', 1);
                    }

                    // 刷大图
                    if (jump_to_print == false) {
                        // output_imgdata(thumbnail_path, 168);
                        std::ifstream infile("/home/mks/tjc"); // 打开文件
                        if (!infile) {
                            std::cerr << "无法打开文件 " << "/home/mks/tjc" << std::endl;
                            show_preview_complete = true;
                            return;
                        }
                        std::stringstream buffer;
                        buffer << infile.rdbuf();
                        file_metadata_gimage = buffer.str();
                        infile.close();
                        send_cmd_baud(tty_fd, 921600);
                        usleep(50000);
                        set_option(tty_fd, 921600, 8, 'N', 1);
                        send_cmd_cp_close(tty_fd, "preview.cp0");
                        if (file_metadata_gimage != "") {
                            std::cout << "刷大图" << std::endl;
                            // std::cout << "图片字符串的大小为: " << file_metadata_gimage.length() << std::endl;
                            // std::cout << "图片的字节大小为：" << file_metadata_gimage.size() << std::endl;
                            int num = 2048;
                            int len = file_metadata_gimage.length();
                            int end = num;
                            std::string g;
                            // int i = 0;
                            for (int start = 0; start < len;) {
                                if (end > len) {
                                g = file_metadata_gimage.substr(start, len - start);
                                // std::cout << "字串" << i <<  ":" << s << std::endl;
                                tcdrain(tty_fd);
                                send_cmd_cp_image(tty_fd, "cp0", g);
                                break;
                            }
                                g = file_metadata_gimage.substr(start, num);
                                start = end;
                                end = end + num;
                                // std::cout << "字串" << i <<  ":" << s << std::endl;
                                // i++;
                                tcdrain(tty_fd);
                                send_cmd_cp_image(tty_fd, "cp0", g);
                                // usleep(50000);
                            }
                        }
                        send_cmd_baud(tty_fd, 115200);
                        usleep(50000);
                        set_option(tty_fd, 115200, 8, 'N', 1);
                        bed_leveling_switch(true);
                    }
                    show_preview_gimage_completed = true;
                }
            }

            if (show_preview_gimage_completed == true) {
                send_cmd_vis(tty_fd, "cp0", "1");
            } else {
                send_cmd_vis(tty_fd, "cp0", "0");
            }

            show_preview_complete = true;
            if (jump_to_print == true) {
                //printed_file_path = "/" + printer_print_stats_filename;
                check_filament_type();
                jump_to_print = false;
            }
        }
    }
}

void refresh_page_main() {
    send_cmd_val(tty_fd, "n0", std::to_string(printer_extruder_temperature));
    send_cmd_val(tty_fd, "n1", std::to_string(printer_heater_bed_temperature));
    send_cmd_val(tty_fd, "n2", std::to_string(printer_hot_temperature));

    if (detect_disk() == 0) {                       // CLL 是否已插入U盘
        send_cmd_picc(tty_fd, "q1", "32");
    } else {
        send_cmd_picc(tty_fd, "q1", "33");
    }

    if (strcmp(status_result.wpa_state, "COMPLETED") == 0) {       // CLL 是否已连接wifi
        send_cmd_picc(tty_fd, "q0", "32");
    } else {
        send_cmd_picc(tty_fd, "q0", "33");
    }

    if (printer_caselight_value == 0) {             // LED灯的logo
        send_cmd_picc(tty_fd, "b0", "32");
        send_cmd_picc2(tty_fd, "b0", "30");
    } else {
        send_cmd_picc(tty_fd, "b0", "33");
        send_cmd_picc2(tty_fd, "b0", "31");
    }

    if (printer_out_pin_beep_value == 0) {
        send_cmd_picc(tty_fd, "b1", "32");
        send_cmd_picc2(tty_fd, "b1", "30");
    } else {
        send_cmd_picc(tty_fd, "b1", "33");
        send_cmd_picc2(tty_fd, "b1", "31");
    }

    if (printer_extruder_target == 0) { // CLL 主页喷嘴是否加热刷新
        send_cmd_pco(tty_fd, "n0", "65535");
        send_cmd_picc(tty_fd, "b3", "32");
        send_cmd_picc2(tty_fd, "b3", "30");
    } else {
        send_cmd_pco(tty_fd, "n0", "63488");
        send_cmd_picc(tty_fd, "b3", "33");
        send_cmd_picc2(tty_fd, "b3", "31");
    }

    if (printer_heater_bed_target == 0) { // CLL 主页热床按钮是否加热刷新
        send_cmd_pco(tty_fd, "n1", "65535");
        send_cmd_picc(tty_fd, "b4", "32");
        send_cmd_picc2(tty_fd, "b4", "30");
    } else {
        send_cmd_pco(tty_fd, "n1", "63488");
        send_cmd_picc(tty_fd, "b4", "33");
        send_cmd_picc2(tty_fd, "b4", "31");
    }

    if (printer_hot_target == 0) { // CLL 主页热腔按钮是否加热刷新
        send_cmd_pco(tty_fd, "n2", "65535");
        send_cmd_picc(tty_fd, "b5", "32");
        send_cmd_picc2(tty_fd, "b5", "30");
    } else {
        send_cmd_pco(tty_fd, "n2", "63488");
        send_cmd_picc(tty_fd, "b5", "33");
        send_cmd_picc2(tty_fd, "b5", "31");
    }
    
    // CLL 每次开机或打印后执行图片刷新
    
    if (main_picture_refreshed == false) {
        // CLL 获取文件信息
        page_files_pages = 0;
        page_files_current_pages = 0;
        page_files_folder_layers = 0;
        page_files_previous_path = "";
        page_files_root_path = DEFAULT_DIR;
        page_files_path = "";
        refresh_page_files(page_files_current_pages);
        if (page_files_list_show_type[0] == "[c]") {
            send_cmd_txt(tty_fd, "t0", page_files_list_show_name[0]);
            std::string picture_path = page_files_path + "/.cache/.thumbs/" + page_files_list_show_name[0].substr(0, page_files_list_show_name[0].rfind("."));
            picture_path = picture_path.substr(1);
            MKSLOG_RED("图片路径:%s", picture_path.c_str()); 
            char relative_path[1024];
            sprintf(relative_path, "/home/mks/gcode_files/%s-160x160.png", picture_path.c_str());
            if (access(relative_path, F_OK) == 0) {
                MKSLOG_RED("找到png图片");
                send_cmd_pic(tty_fd, "b[0]", "29");
                send_cmd_picc(tty_fd, "b6", "29");
                send_cmd_picc2(tty_fd, "b6", "30");
                send_cmd_vis(tty_fd, "cp0", "1");
                refresh_files_list_picture(picture_path + "-160x160.png", 160, 0);
                main_picture_detected = true;
            } else {
                sprintf(relative_path, "/home/mks/gcode_files/%s-160x160.jpg", picture_path.c_str());
                if (access(relative_path, F_OK) == 0) {
                    MKSLOG_RED("找到jpg图片");
                    send_cmd_pic(tty_fd, "b[0]", "29");
                    send_cmd_picc(tty_fd, "b6", "29");
                    send_cmd_picc2(tty_fd, "b6", "30");
                    refresh_files_list_picture(picture_path + "-160x160.jpg", 160, 0);
                    main_picture_detected = true;
                } else {
                    send_cmd_pic(tty_fd, "b[0]", "26");
                    send_cmd_picc(tty_fd, "b6", "26");
                    send_cmd_picc2(tty_fd, "b6", "31");
                    send_cmd_vis(tty_fd, "cp0", "0");
                }
            }
        } else {
            send_cmd_pic(tty_fd, "b[0]", "26");
            send_cmd_picc(tty_fd, "b6", "26");
            send_cmd_picc2(tty_fd, "b6", "31");
            send_cmd_txt(tty_fd, "t0", "");
            send_cmd_vis(tty_fd, "cp0", "0");
        }
        main_picture_refreshed = true;
    }

    // CLL 每次开机发送断电续打
    if (open_reprint_asked == false) {
        check_print_interrupted();
        open_reprint_asked = true;
    }
}

void refresh_page_files_list() {
    send_cmd_vis(tty_fd, "q0", "0");
    for (int i = 0; i < 4; i++) {
        send_cmd_txt(tty_fd, "t" + std::to_string(i + 1), page_files_list_show_name[i]);
        send_cmd_vis(tty_fd, "cp" + std::to_string(i), "0");
        if (page_files_list_show_type[i] =="[c]") {
            send_cmd_picc(tty_fd, "b" + std::to_string(i+1), "40");
            send_cmd_picc2(tty_fd, "b" + std::to_string(i+1), "37");
            send_cmd_vis(tty_fd, "q0", "1");
        } else if (page_files_list_show_type[i] == "[d]") {
            send_cmd_picc(tty_fd, "b" + std::to_string(i + 1), "41");
            send_cmd_picc2(tty_fd, "b" + std::to_string(i + 1), "39");
        } else if (page_files_list_show_type[i] == "[f]") {
            send_cmd_picc(tty_fd, "b" + std::to_string(i + 1), "40");
            send_cmd_picc2(tty_fd, "b" + std::to_string(i + 1), "37");
            
        } else if (page_files_list_show_type[i] == "[n]") {
            send_cmd_picc(tty_fd, "b" + std::to_string(i + 1), "34");
            send_cmd_picc2(tty_fd, "b" + std::to_string(i + 1), "34");
        }
    }

    //4.4.2 CLL 文件列表界面新增本地、U盘文件按钮
    if (file_mode == "Local") {
        send_cmd_picc(tty_fd, "b12", "35");
        send_cmd_picc2(tty_fd, "b12", "37");
        send_cmd_picc(tty_fd, "b13", "35");
        send_cmd_picc2(tty_fd, "b13", "37");
    } else if (file_mode == "USB") {
        send_cmd_picc(tty_fd, "b12", "34");
        send_cmd_picc2(tty_fd, "b12", "36");
        send_cmd_picc(tty_fd, "b13", "34");
        send_cmd_picc2(tty_fd, "b13", "36");
        if (detect_disk() == -1) {
            send_cmd_vis(tty_fd, "t0", "1");
        }
    } /* else if (file_mode == "NULL") {
        send_cmd_picc(tty_fd, "b12", "34");
        send_cmd_picc2(tty_fd, "b12", "36");
        send_cmd_picc(tty_fd, "b13", "34");
        send_cmd_picc2(tty_fd, "b13", "36");
        send_cmd_vis(tty_fd, "t0", "1");
    }
    */
    if (page_files_current_pages == 0) {
        send_cmd_picc(tty_fd, "b10", std::to_string(40));
        send_cmd_picc2(tty_fd, "b10", std::to_string(39));
    } else {
        send_cmd_picc(tty_fd, "b10", std::to_string(41));
        send_cmd_picc2(tty_fd, "b10", std::to_string(37));
    }
    if (page_files_current_pages == page_files_pages) {
        send_cmd_picc(tty_fd, "b11", std::to_string(40));
        send_cmd_picc2(tty_fd, "b11", std::to_string(39));
    } else {
        send_cmd_picc(tty_fd, "b11", std::to_string(41));
        send_cmd_picc2(tty_fd, "b11", std::to_string(37));
    }
    if (page_files_folder_layers == 0 || (page_files_folder_layers == 1 && file_mode != "Local")) {
        send_cmd_picc(tty_fd, "b0", std::to_string(40));
        send_cmd_picc2(tty_fd, "b0", std::to_string(39));
    } else {
        send_cmd_picc(tty_fd, "b0", std::to_string(41));
        send_cmd_picc2(tty_fd, "b0", std::to_string(37));
    }
    delete_small_jpg();
    std::string picture_path;
    for (int i = 0; i < 4; i++) { // CLL 其他控件刷新完后统一刷新图片
        have_64_jpg[i] = false;
        have_64_png_path[i].clear();
        if (page_files_list_show_type[i] =="[c]" || page_files_list_show_type[i] == "[f]") {
            if (page_files_list_show_type[i]== "[c]") {
                picture_path = page_files_path + "/.cache/.thumbs/" + page_files_list_show_name[i].substr(0, page_files_list_show_name[i].rfind("."));
            } else {
                picture_path = page_files_path + "/.thumbs/" + page_files_list_show_name[i].substr(0, page_files_list_show_name[i].rfind("."));
            }
            picture_path = picture_path.substr(1);
            MKSLOG_RED("图片路径:%s", picture_path.c_str()); 
            char relative_path[1024];
            sprintf(relative_path, "/home/mks/gcode_files/%s-112x112_QD.jpg", picture_path.c_str());
            if (access(relative_path, F_OK) == 0) {
                have_64_jpg[i] = true;
                have_64_png_path[i] = picture_path + "-112x112_QD.jpg";
                refresh_files_list_picture_2(picture_path + "-112x112_QD.jpg", 112, i);
            } /* else {
                sprintf(relative_path, "/home/mks/gcode_files/%s-32x32.png", picture_path.c_str());
                if (access(relative_path, F_OK) == 0) {
                    have_64_jpg[i] = true;
                    have_64_png_path[i] = picture_path + "-32x32.png";
                    refresh_files_list_picture_2(picture_path + "-32x32.png", 112, i);
                }
            }
            */
        }
    }
}

void refresh_page_files(int pages) {
    get_page_files_filelist(page_files_root_path + page_files_path);
    set_page_files_show_list(pages);
}

/* 订阅内容处理 */
void sub_object_status() {
    ep->Send(json_subscribe_to_printer_object_status(subscribe_objects_status()));
}

void get_object_status() {
    ep->Send(json_query_printer_object_status(subscribe_objects_status()));
}

void get_file_estimated_time(std::string filename) {
    ep->Send(json_get_gcode_metadata(filename));
}

void delete_file(std::string filepath) {
    filelist_changed = false;
    ep->Send(json_file_delete(filepath));
    while (!filelist_changed);
}

void start_printing(std::string filepath) {
    ep->Send(json_print_a_file(filepath));
}

void set_target(std::string heater, int target) {
    ep->Send(json_run_a_gcode(set_heater_temp(heater, target)));
}

void set_extruder_target(int target) {
    set_target("extruder", target);
}

void set_heater_bed_target(int target) {
    set_target("heater_bed", target);
}

void set_hot_target(int target) {
    ep->Send(json_run_a_gcode("M141 S" + std::to_string(target)));
}

void set_fan(int speed) {
    ep->Send(json_run_a_gcode(set_fan_speed(speed)));
}

void set_fan0(int speed) {
    ep->Send(json_run_a_gcode(set_fan0_speed(speed)));
}

void set_fan2(int speed) {
    ep->Send(json_run_a_gcode(set_fan2_speed(speed)));
}

void set_fan3(int speed) {
    ep->Send(json_run_a_gcode(set_fan3_speed(speed)));
}

void set_intern_zoffset(float offset) {
    printer_set_offset = offset;
    
}

void set_zoffset(bool positive) {
    // std::string command;
    // if (positive) {
    //     command = "curl -s -X POST http://127.0.0.1:7125/printer/modifybabystep?ADJUST=" + std::to_string(printer_set_offset);
    // } else {
    //     command = "curl -s -X POST http://127.0.0.1:7125/printer/modifybabystep?ADJUST=-" + std::to_string(printer_set_offset);
    // }
    // system(command.c_str());
    if (positive == true) {
        ep->Send(json_run_a_gcode("SET_GCODE_OFFSET Z_ADJUST=+" + std::to_string(printer_set_offset) + " MOVE=1"));
    } else {
        ep->Send(json_run_a_gcode("SET_GCODE_OFFSET Z_ADJUST=-" + std::to_string(printer_set_offset) + " MOVE=1"));
    }
}

void set_move_dist(float dist) {
    printer_move_dist = dist;
}

void set_printer_speed(int speed) {
    std::cout << "Rate = " << std::to_string(speed) << std::endl;
    ep->Send(json_run_a_gcode(set_speed_rate(std::to_string(speed))));
}

void set_printer_flow(int rate) {
    ep->Send(json_run_a_gcode("M221 S" + (std::to_string(rate))));
}

std::string show_time(int seconds) {
    return std::to_string((int)(seconds/3600)) + "h" + std::to_string((int)((seconds % 3600) / 60)) + "m";
}

void move_home() {
    ep->Send(json_run_a_gcode("G28\n"));
}

void move_x_decrease() {
    ep->Send(move(AXIS_X, "-" + std::to_string(printer_move_dist), 130));
    unhomed_move_mode = 2;
}

void move_x_increase() {
    ep->Send(move(AXIS_X, "+" + std::to_string(printer_move_dist), 130));
    unhomed_move_mode = 1;
}

void move_y_decrease() {
    ep->Send(move(AXIS_Y, "-" + std::to_string(printer_move_dist), 130));
    unhomed_move_mode = 4;
}

void move_y_increase() {
    ep->Send(move(AXIS_Y, "+" + std::to_string(printer_move_dist), 130));
    unhomed_move_mode = 3;
}

void move_z_decrease() {
    ep->Send(move(AXIS_Z, "-" + std::to_string(printer_move_dist), 10));
    unhomed_move_mode = 5;
}

void move_z_increase() {
    ep->Send(move(AXIS_Z, "+" + std::to_string(printer_move_dist), 10));
    unhomed_move_mode = 6;
}

bool get_filament_detected() {
    return filament_switch_sensor_fila_filament_detected;
}

bool get_filament_detected_enable() {
    return filament_switch_sensor_fila_enabled;
}

bool get_print_pause_resume() {
    return printer_pause_resume_is_paused;
}

void set_print_pause_resume() {
    if (printer_pause_resume_is_paused == false) {
        ep->Send(json_run_a_gcode("PAUSE"));
    } else {
        ep->Send(json_run_a_gcode("RESUME"));
    }
}

void set_print_pause() {
    ep->Send(json_run_a_gcode("PAUSE"));
}

void set_print_resume() {
    ep->Send(json_run_a_gcode("RESUME"));
}

void cancel_print() {
    printer_print_stats_filename = "";
    system("curl -X POST http://127.0.0.1:7125/printer/breakmacro");
    system("curl -X POST http://127.0.0.1:7125/printer/breakheater");
    ep->Send(json_run_a_gcode("CANCEL_PRINT"));
    int printed_minutes = get_cal_printed_time((int)(printer_print_stats_print_duration));
    get_mks_total_printed_time();
    mks_total_printed_minutes = mks_total_printed_minutes + printed_minutes;
    std::cout << "@@@@@@ " << printed_minutes << std::endl;
    set_mks_total_printed_time(mks_total_printed_minutes);
    usleep(10000);
    sdcard_reset_file();
}

void sdcard_reset_file() {
    ep->Send(json_run_a_gcode("SDCARD_RESET_FILE"));
}

void set_auto_level_dist(float dist) {
    MKSLOG_BLUE("SET");
    auto_level_dist = dist;
}

void start_auto_level() {
    step_1 = false;
    step_2 = false;
    step_3 = false;
    step_4 = false;
    if (start_pre_auto_level == false) {
        printer_idle_timeout_state = "Printing";
    }
    page_to(TJC_PAGE_AUTO_MOVING);
    set_heater_bed_target(mks_heater_bed_target);
    ep->Send(json_run_a_gcode("M4029"));
}

//4.4.1 CLL 修改调平
void start_auto_level_dist(bool positive) {
    if (positive == true) {
        //ep->Send(json_run_a_gcode("TESTZ Z=" + std::to_string(auto_level_dist)));
        ep->Send(json_run_a_gcode("SET_GCODE_OFFSET Z_ADJUST=" + std::to_string(auto_level_dist) + " MOVE=1"));
    } else {
        //ep->Send(json_run_a_gcode("TESTZ Z=-" + std::to_string(auto_level_dist)));
        ep->Send(json_run_a_gcode("SET_GCODE_OFFSET Z_ADJUST=-" + std::to_string(auto_level_dist) + " MOVE=1"));
    }
}

void set_filament_extruder_target(bool positive) {
    get_mks_extruder_target();
    printer_filament_extruder_target = mks_extruder_target;
    if (positive == true) {
        printer_filament_extruder_target += 3;
    } else {
        printer_filament_extruder_target -= 3;
    }
    
    if (printer_filament_extruder_target >= 350) {
        printer_filament_extruder_target = 350;
    }

    if (printer_filament_extruder_target < 0) {
        printer_filament_extruder_target = 0;
        set_extruder_target(0);
        set_mks_extruder_target(0);
    } else {
        set_extruder_target(printer_filament_extruder_target);
        set_mks_extruder_target(printer_filament_extruder_target);
    }
}

void set_print_filament_dist(float dist) {
    printer_filament_extruedr_dist = dist;
}

void start_retract() {
    ep->Send(json_run_a_gcode("M83\nG1 E-" + std::to_string(printer_filament_extruedr_dist) + " F300\n"));
}

void start_extrude() {
    ep->Send(json_run_a_gcode("M83\nG1 E" + std::to_string(printer_filament_extruedr_dist) + " F300\n"));
}

std::string get_ip(std::string net) {
    char result[MAX_FILE_LEN] = {0};
    std::string cmd = "ifconfig " + net + " | awk 'NR==2{print $2}' | tr -d '\n\r'";
    execute_cmd(cmd.data(), result);
    return result;
}

void move_home_tips() {
    jump_to_move_pop_2 = true;
}

void filament_tips() {
    switch (current_page_id)
    {
    case TJC_PAGE_OPEN_FILAMENTVIDEO_3:
        break;
    
    case TJC_PAGE_PRINT_FILAMENT:
        jump_to_print_low_temp = true;
        break;

    default:
        jump_to_filament_pop_1 = true;
        break;
    }
}

void move_tips() {
    jump_to_move_pop_1 = true;
    if (current_page_id == TJC_PAGE_PRINTING || current_page_id == TJC_PAGE_PRINT_ZOFFSET || current_page_id == TJC_PAGE_PRINT_FILAMENT || current_page_id == TJC_PAGE_PRINTING_2) {
        cancel_print();
    }
}

void reset_klipper() {
    ep->Send(json_run_a_gcode("RESTART\n"));
}

void reset_firmware() {
    ep->Send(json_run_a_gcode("FIRMWARE_RESTART\n"));
}

void finish_print() {
    sdcard_reset_file();
    clear_cp0_image();
    clear_page_preview();
    show_preview_complete = false;
    page_to(TJC_PAGE_MAIN);
}

void set_filament_sensor() {
    std::cout << "filament_switch_sensor fila = " << filament_switch_sensor_fila_enabled << std::endl; 
    if (filament_switch_sensor_fila_enabled == 0) {
        ep->Send(json_run_a_gcode("SET_FILAMENT_SENSOR SENSOR=fila ENABLE=1\n"));
        mks_fila_status = true;
        set_mks_fila_status();
    } else {
        ep->Send(json_run_a_gcode("SET_FILAMENT_SENSOR SENSOR=fila ENABLE=0\n"));
        mks_fila_status = false;
        set_mks_fila_status();
    }
}

void motors_off() {
    // ep->Send(json_run_a_gcode("M84\n"));
    ep->Send(json_emergency_stop());
    sleep(1);
    ep->Send(json_run_a_gcode("FIRMWARE_RESTART\n"));     // 电机解锁改为急停
}

void beep_on_off() {
    if (printer_out_pin_beep_value == 0) {
        ep->Send(json_run_a_gcode("beep_on"));
        mks_beep_status = true;
        set_mks_beep_status();
    } else {
        ep->Send(json_run_a_gcode("beep_off"));
        mks_beep_status = false;
        set_mks_beep_status();
    }
}

void led_on_off() {
    if (printer_caselight_value == 0) {
        ep->Send(json_run_a_gcode("SET_PIN PIN=caselight VALUE=1"));
        if (current_page_id != TJC_PAGE_SCREEN_SLEEP) {
            mks_led_status = true;
            set_mks_led_status();
        }
    } else {
        ep->Send(json_run_a_gcode("SET_PIN PIN=caselight VALUE=0"));
        if (current_page_id != TJC_PAGE_SCREEN_SLEEP){
            mks_led_status = false;
            set_mks_led_status();
        }
    }
}

void shutdown_mcu() {
    system("echo \"SET_PIN PIN=pwc VALUE=0\" > /root/mcu_shutdown.txt");
    ep->Send(json_run_a_gcode("SET_PIN PIN=pwc VALUE=0"));
}

void firmware_reset() {
    ep->Send(json_run_a_gcode("FIRMWARE_RESTART\n"));
}

void go_to_page_power_off() {
    page_to(TJC_PAGE_SHUTDOWN);
}

int get_mks_led_status() {
    mksini_load();
    mks_led_status = mksini_getboolean("led", "enable", 0);
    mksini_free();
    return mks_led_status;
}

void set_mks_led_status() {
    mksini_load();
    mksini_set("led", "enable", std::to_string(mks_led_status));
    mksini_save();
    mksini_free();
}

int get_mks_beep_status() {
    mksini_load();
    mks_beep_status = mksini_getboolean("beep", "enable", 0);
    mksini_free();
    return mks_beep_status;
}

void set_mks_beep_status() {
    mksini_load();
    mksini_set("beep", "enable", std::to_string(mks_beep_status));
    mksini_save();
    mksini_free();
    system("sync");
}

void get_mks_language_status() {
    mksini_load();
    mks_language_status = mksini_getint("system", "language", 0);
    mksini_free();
}

void set_mks_language_status() {
    mksini_load();
    mksini_set("system", "language", std::to_string(mks_language_status));
    mksini_save();
    mksini_free();
}

void get_mks_extruder_target() {
    mksini_load();
    mks_extruder_target = mksini_getint("target", "extruder", 200);
    mksini_free();
}

void set_mks_extruder_target(int target) {
    if (target != 0) {
    mksini_load();
    mksini_set("target", "extruder", std::to_string(target));
    mksini_save();
    mksini_free();
    system("sync");
    }
}

void get_mks_heater_bed_target() {
    mksini_load();
    mks_heater_bed_target = mksini_getint("target", "heaterbed", 40);
    mksini_free();
}

void set_mks_heater_bed_target(int target) {
    if (target != 0) {
    mksini_load();
    std::cout << "######## " << target << std::endl;
    mksini_set("target", "heaterbed", std::to_string(target));
    mksini_save();
    mksini_free();
    system("sync");
    }
}

void get_mks_hot_target() {
    mksini_load();
    mks_hot_target = mksini_getint("target", "hot", 40);
    mksini_free();
}

void set_mks_hot_target(int target) {
    mksini_load();
    std::cout << "######## " << target << std::endl;
    mksini_set("target", "hot", std::to_string(target));
    mksini_save();
    mksini_free();
    system("sync");
}

void filament_extruder_target() {
    get_mks_extruder_target();
    if (printer_extruder_target == 0) {
        set_extruder_target(mks_extruder_target);
    } else {
        set_extruder_target(0);
    }
}

void filament_heater_bed_target() {
    get_mks_heater_bed_target();
    if (0 == printer_heater_bed_target) {
        set_heater_bed_target(mks_heater_bed_target);
    } else {
        set_heater_bed_target(0);
    }
}

void filament_hot_target() {
    get_mks_hot_target();
    if (0 == printer_hot_target) {
        set_hot_target(mks_hot_target);
    } else {
        set_hot_target(0);
    }
}

void filament_fan0() {
    if (printer_out_pin_fan0_value == 0) {
        set_fan0(100);
    } else {
        set_fan0(0);
    }
}

void filament_fan2() {
    if (printer_out_pin_fan2_value == 0) {
        set_fan2(100);
    } else {
        set_fan2(0);
    }
}

void filament_fan3() {
    if (printer_out_pin_fan3_value == 0) {
        set_fan3(100);
    } else {
        set_fan3(0);
    }
}

void go_to_reset() {
    if (printer_webhooks_state == "shutdown") {
        page_to(TJC_PAGE_RESET);
    } else {
        std::ifstream infile("/dev_info.txt");
        if (!infile) {
            std::cerr << "无法打开文件 " << "/dev_info.txt" << std::endl;
            return;
        }
        std::stringstream buffer;
        buffer << infile.rdbuf();
        std::string machine_type = buffer.str();
        infile.close();
        machine_type = machine_type.substr(machine_type.find("@") + 1);
        page_to(TJC_PAGE_SYS_OK);
        std::cout << "machine_type:" << machine_type << std::endl;
        send_cmd_txt(tty_fd, "t2", machine_type);
    }
}

void go_to_network() {
    if (detected_wlan0()) {
        mks_wpa_cli_open_connection();
        page_wifi_list_ssid_button_enabled[0] = false;
        page_wifi_list_ssid_button_enabled[1] = false;
        page_wifi_list_ssid_button_enabled[2] = false;
        page_wifi_list_ssid_button_enabled[3] = false;
        page_wifi_list_ssid_button_enabled[4] = false;
        page_wifi_ssid_list_pages = 0;
        page_wifi_current_pages = 0;
        page_to(TJC_PAGE_WIFI_LIST);
        //4.4.1 CLL 修复WiFi刷新
        scan_ssid_and_show();
        get_wlan0_status();
        if (strcmp(status_result.wpa_state, "COMPLETED") == 0) {
            current_connected_ssid_name = hex_to_utf8(status_result.ssid);       // 如果已经连接wifi，获取wifi的名字
        } else if (strcmp(status_result.wpa_state, "INACTIVE")) {
            current_connected_ssid_name.clear();                    // 如果没连接wifi，清除掉当前已连接wifi的名字
        }
    } else {
        page_to(TJC_PAGE_INTERNET);
    }
}

void scan_ssid_and_show() {
    if (access("/var/run/wpa_supplicant/wlan0", F_OK) == 0){
        get_wlan0_status();
        mks_wpa_scan_scanresults();
        get_ssid_list_pages();
        page_wifi_current_pages = 0;
        set_page_wifi_ssid_list(page_wifi_current_pages);
        refresh_page_wifi_list();
    } else {
        page_to(TJC_PAGE_INTERNET);
    }
}

void refresh_page_wifi_list() {
    for (int i = 0; i < 5; i++) {
        std::cout << "刷新的wifi: " << page_wifi_ssid_list[i] << std::endl;
        if (0 == page_wifi_current_pages) {
            if (0 == i) {
                if (strcmp(status_result.wpa_state, "COMPLETED") == 0) {
                    send_cmd_txt(tty_fd, "t" + std::to_string(i+1), hex_to_utf8(status_result.ssid));
                } else {
                    send_cmd_txt(tty_fd, "t" + std::to_string(i+1), page_wifi_ssid_list[i]);
                }
            } else {
                send_cmd_txt(tty_fd, "t" + std::to_string(i+1), page_wifi_ssid_list[i]);
            }
        } else {
            send_cmd_txt(tty_fd, "t" + std::to_string(i+1), page_wifi_ssid_list[i]);
        }
        MKSLOG_BLUE("%d/%d", page_wifi_current_pages + 1, page_wifi_ssid_list_pages);
        if (page_wifi_current_pages == 0) {
            if (strcmp(status_result.wpa_state, "COMPLETED") == 0) {
                send_cmd_picc(tty_fd, "wifi1", "126");
                send_cmd_picc2(tty_fd, "wifi1", "124");
                page_wifi_list_ssid_button_enabled[0] = false;
            } else {
                send_cmd_picc(tty_fd, "wifi1", "125");
                send_cmd_picc2(tty_fd, "wifi1", "123");
                page_wifi_list_ssid_button_enabled[0] = true;
            }
        } else {
            send_cmd_picc(tty_fd, "wifi1", "125");
            send_cmd_picc2(tty_fd, "wifi1", "123");
            page_wifi_list_ssid_button_enabled[0] = true;
        }

        if (page_wifi_ssid_list[i] == "") {
            std::cout << "刷新为没有锁的图片" << std::endl;
            send_cmd_picc(tty_fd, "wifi" + std::to_string(i + 1), "122");
            send_cmd_picc2(tty_fd, "wifi" + std::to_string(i + 1), "122");
            page_wifi_list_ssid_button_enabled[i] = false;
        } else {
            std::cout << "刷新为有锁的图片" << std::endl;
            send_cmd_picc(tty_fd, "wifi" + std::to_string(i + 1), "125");
            send_cmd_picc2(tty_fd, "wifi" + std::to_string(i + 1), "123");
            page_wifi_list_ssid_button_enabled[i] = true;
        }

        if (page_wifi_ssid_list_pages == 0) {
            send_cmd_picc(tty_fd, "b1", "126");
            send_cmd_picc2(tty_fd, "b1", "123");
            send_cmd_picc(tty_fd, "b2", "126");
            send_cmd_picc2(tty_fd, "b2", "123");
        } else {
            if (page_wifi_current_pages == 0) {
                send_cmd_picc(tty_fd, "b1", "126");
                send_cmd_picc2(tty_fd, "b1", "123");
            } else {
                send_cmd_picc(tty_fd, "b1", "125");
                send_cmd_picc2(tty_fd, "b1", "124");
            }
            if (page_wifi_current_pages == page_wifi_ssid_list_pages - 1) {
                send_cmd_picc(tty_fd, "b2", "126");
                send_cmd_picc2(tty_fd, "b2", "123");
            } else {
                send_cmd_picc(tty_fd, "b2", "125");
                send_cmd_picc2(tty_fd, "b2", "124");
            }
        }
    }
}

void refresh_page_wifi_list_2() {
    send_cmd_txt(tty_fd, "t0", status_result.ip_address);
    if (strcmp(status_result.wpa_state, "COMPLETED") == 0) {

    } else if (strcmp(status_result.wpa_state, "INACTIVE")) {

    }
}

void get_wifi_list_ssid(int index) {
    get_wifi_name.clear();
    get_wifi_name = page_wifi_ssid_list[index];
}

void set_print_filament_target() {
    if (0 == printer_extruder_target) {
        get_mks_extruder_target();
        set_extruder_target(mks_extruder_target);
    } else {
        set_extruder_target(0);
    }
}

void complete_print() {
    if (page_printing_shutdown_enable == false) {
        ep->Send(json_run_a_gcode("PRINT_END"));
    } else {
        ep->Send(json_run_a_gcode("PRINT_END_POWEROFF"));
    }
    int printed_minutes = get_cal_printed_time((int)(printer_print_stats_print_duration));
    get_mks_total_printed_time();
    mks_total_printed_minutes = mks_total_printed_minutes + printed_minutes;
    std::cout << "@@@@@@ " << printed_minutes << std::endl;
    set_mks_total_printed_time(mks_total_printed_minutes);
}

void back_to_main() {
    clear_previous_data();
    page_to(TJC_PAGE_MAIN);
}

void go_to_syntony_move() {
    step_1 = false;
    page_syntony_finished = false;
    printer_idle_timeout_state = "Printing";
    page_to(TJC_PAGE_SYNTONY_MOVE);
    ep->Send(json_run_a_gcode("M901\n"));
}

void print_ssid_psk(char *psk) {
    MKSLOG_RED("SSID is %s", get_wifi_name.c_str());
    MKSLOG_RED("PSK is %s", psk);
    mks_set_ssid(const_cast<char*>(get_wifi_name.c_str()));
    mks_set_psk(psk);
}

void clear_page_preview() {
    file_metadata_filename = "";
    file_metadata_estimated_time = 0;
    file_metadata_filament_weight_total = 0;
    file_metadata_filament_name = "";
    file_metadata_filament_type = "";
    file_metadata_simage = "";
    file_metadata_gimage = "";
}

void set_mks_babystep(std::string value) {
    mksini_load();
    mksini_set("babystep", "value", value);
    mksini_save();
    mksini_free();
    system("sync");
}

void get_mks_babystep() {
    mksini_load();
    mks_babystep_value = mksini_getstring("babystep", "value", "0.000");
    mks_adxl_offset = mksini_getstring("babystep", "adxl_offset", "0.000");
    mksini_free();
}

void clear_cp0_image() {
    send_cmd_cp_close(tty_fd, "preview.cp0");
    send_cmd_txt(tty_fd, "preview.cp0_text", "");
    send_cmd_txt(tty_fd, "preview.add", "");
    show_preview_gimage_completed = false;
    mks_file_parse_finished = false;
    file_metadata_simage.clear();
    file_metadata_gimage.clear();
}

void printer_set_babystep() {
    get_mks_babystep();
    ep->Send(json_run_a_gcode("SET_GCODE_OFFSET Z=" + mks_babystep_value + " MOVE=0"));
}

int get_mks_fila_status() {
    mksini_load();
    mks_fila_status = mksini_getboolean("fila", "enable", 0);
    mksini_free();
    return mks_fila_status;
}

void set_mks_fila_status() {
    mksini_load();
    mksini_set("fila", "enable", std::to_string(mks_fila_status));
    mksini_save();
    mksini_free();
    system("sync");
}

void init_mks_status() {
    get_mks_total_printed_time();
    get_mks_babystep();
    get_mks_connection_method();
    get_mks_ethernet();
    // 不再使用xindi获取zoffset
    // printer_set_babystep();
    /*
    if (get_mks_beep_status() == 0) {
		MKSLOG_RED("关闭蜂鸣器");
		if (get_mks_led_status() == 0) {
			MKSLOG_BLUE("关掉LED灯");
            ep->Send(json_run_a_gcode("beep_off\nSET_PIN PIN=caselight VALUE=0\n"));
            if (get_mks_fila_status() == 0) {
				MKSLOG_BLUE("关掉断料检测");
				ep->Send(json_run_a_gcode("beep_off\nSET_PIN PIN=caselight VALUE=0\nSET_FILAMENT_SENSOR SENSOR=fila ENABLE=0\n"));
			} else {
				MKSLOG_BLUE("打开断料检测");
				ep->Send(json_run_a_gcode("beep_off\nSET_PIN PIN=caselight VALUE=0\nSET_FILAMENT_SENSOR SENSOR=fila ENABLE=1\n"));
			}
		} else {
			MKSLOG_BLUE("打开LED灯");
            ep->Send(json_run_a_gcode("beep_off\nSET_PIN PIN=caselight VALUE=1\n"));
            if (get_mks_fila_status() == 0) {
				MKSLOG_BLUE("关掉断料检测");
				ep->Send(json_run_a_gcode("beep_off\nSET_PIN PIN=caselight VALUE=1\nSET_FILAMENT_SENSOR SENSOR=fila ENABLE=0\n"));
			} else {
				MKSLOG_BLUE("打开断料检测");
				ep->Send(json_run_a_gcode("beep_off\nSET_PIN PIN=caselight VALUE=1\nSET_FILAMENT_SENSOR SENSOR=fila ENABLE=1\n"));
			}
		}
	} else {
		MKSLOG_RED("打开蜂鸣器");
		if (get_mks_led_status() == 0) {
			MKSLOG_BLUE("关掉LED灯");
			ep->Send(json_run_a_gcode("beep_on\nSET_PIN PIN=caselight VALUE=0\n"));
            if (get_mks_fila_status() == 0) {
				MKSLOG_BLUE("关掉断料检测");
				ep->Send(json_run_a_gcode("beep_on\nSET_PIN PIN=caselight VALUE=0\nSET_FILAMENT_SENSOR SENSOR=fila ENABLE=0\n"));
			} else {
				MKSLOG_BLUE("打开断料检测");
				ep->Send(json_run_a_gcode("beep_on\nSET_PIN PIN=caselight VALUE=0\nSET_FILAMENT_SENSOR SENSOR=fila ENABLE=1\n"));
			}
		} else {
			MKSLOG_BLUE("打开LED灯");
            ep->Send(json_run_a_gcode("beep_on\nSET_PIN PIN=caselight VALUE=1\n"));
			if (get_mks_fila_status() == 0) {
				MKSLOG_BLUE("关掉断料检测");
				ep->Send(json_run_a_gcode("beep_on\nSET_PIN PIN=caselight VALUE=1\nSET_FILAMENT_SENSOR SENSOR=fila ENABLE=0\n"));
			} else {
				MKSLOG_BLUE("打开断料检测");
				ep->Send(json_run_a_gcode("beep_on\nSET_PIN PIN=caselight VALUE=1\nSET_FILAMENT_SENSOR SENSOR=fila ENABLE=1\n"));
			}
		}
	}
    */
}

int detect_disk() {
    if (access("/dev/sda", F_OK) == 0) {
        if (access("/dev/sda1", F_OK) == 0) {
            if (access("/home/mks/gcode_files/sda1", F_OK) != 0) {
                system("/usr/bin/systemctl --no-block restart makerbase-automount@sda1.service");
                sleep(1);
            }
        }
        return 0;
    } else {
        return -1;
    }
}

void set_printing_shutdown() {
    if (page_printing_shutdown_enable == false) {
        page_printing_shutdown_enable = true;
    } else {
        page_printing_shutdown_enable = false;
    }
}

void mks_get_version() {
    mksversion_load();
    mks_version_soc = mksversion_soc("V1.1.1");
    mks_version_mcu = mksversion_mcu("V0.10.0");
    mks_version_ui = mksversion_ui("V1.1.1");
    mksversion_free();
}

void wifi_save_config() {
    page_to(TJC_PAGE_WIFI_SAVING);
    mks_save_config();
    sleep(2);
    get_wlan0_status();
}

void disable_page_about_successed() {
    page_about_successed = true;
}

void finish_tjc_update() {
    if (access("/root/800_480.tft", F_OK) == 0) {
        system("mv /root/800_480.tft /root/800_480.tft.bak; sync");
    }
}

void filament_load() {
    send_cmd_vis(tty_fd, "b2", "0");
    send_cmd_vis(tty_fd, "t4", "1");
    send_cmd_vis(tty_fd, "b3", "0");
    send_cmd_picc(tty_fd, "q0", "194");
    send_cmd_pco(tty_fd, "t0", "65535");
    send_cmd_pco(tty_fd, "t3", "38066");
    send_cmd_vis(tty_fd, "gm1", "0");
    send_cmd_vis(tty_fd, "gm2", "1");
    printer_idle_timeout_state = "Printing";
    ep->Send(json_run_a_gcode("M109 S" + std::to_string(load_target) + "\n"));
    ep->Send(json_run_a_gcode("M604\n"));
}

void filament_unload() {
    // send_cmd_vis(tty_fd, "b2", "0");
    // send_cmd_vis(tty_fd, "t4", "1");
    // send_cmd_vis(tty_fd, "b3", "0");
    // send_cmd_picc(tty_fd, "q0", "194");
    // send_cmd_pco(tty_fd, "t0", "65535");
    // send_cmd_pco(tty_fd, "t3", "38066");
    // send_cmd_vis(tty_fd, "gm1", "0");
    // send_cmd_vis(tty_fd, "gm2", "1");
    printer_idle_timeout_state = "Printing";
    ep->Send(json_run_a_gcode("M109 S" + std::to_string(load_target) + "\n"));
    ep->Send(json_run_a_gcode("M603\n"));
}

int get_cal_printed_time(int print_time) {
    int printed_time = 0;
    printed_time = print_time / 60;
    return printed_time;
}

int get_mks_total_printed_time() {
    mksini_load();
    mks_total_printed_minutes = mksini_getint("total", "time", 0);
    mksini_free();
    return mks_total_printed_minutes;
}

void set_mks_total_printed_time(int printed_time) {
    mksini_load();
    std::cout << "######## " << printed_time << std::endl;
    mksini_set("total", "time", std::to_string(printed_time));
    mksini_save();
    mksini_free();
    system("sync");
}

void get_total_time() {
    ep->Send(json_get_job_totals());
}

void do_not_x_clear() {
    set_mks_total_printed_time(36000);
}

void do_x_clear() {
    set_mks_total_printed_time(0);
}

void level_mode_printing_set_target() {
    set_heater_bed_target(level_mode_printing_heater_bed_target);
    ep->Send(json_run_a_gcode("M190 S" + std::to_string(level_mode_printing_heater_bed_target)));
    set_extruder_target(level_mode_printing_extruder_target);
    ep->Send(json_run_a_gcode("M109 S" + std::to_string(level_mode_printing_extruder_target)));
}

void level_mode_printing_print_file() {
    start_printing("LEVEL_PRINTING.gcode");
}

void update_finished_tips() {
    // page_to(TJC_PAGE_UPDATE_FINISH);
    sleep(5);
    system("sync");
    system("systemctl restart makerbase-client.service");
}

bool get_mks_oobe_enabled() {
    mksini_load();
    mks_oobe_enabled = mksini_getboolean("oobe", "enable", 0);
    mksini_free();
    return mks_oobe_enabled;
}

void set_mks_oobe_enabled(bool enable) {
    mksini_load();
    mksini_set("oobe", "enable", std::to_string(enable));
    mksini_save();
    mksini_free();
    system("sync");
}

void move_motors_off() {
    ep->Send(json_run_a_gcode("M84\n"));
}

void open_more_level_finish() {
    init_mks_status();
    // ep->Send(json_run_a_gcode("G28\n"));
    set_mks_oobe_enabled(false);            // 完成关闭OOBE的操作
    get_object_status();
    page_to(TJC_PAGE_MAIN);
}

void open_move_tip() {
    ep->Send(json_run_a_gcode("G91\nG1 Z-100 F600\nG1 X-100 Y-100 F1200\nG90"));
}

void open_set_print_filament_target() {
    if (0 == printer_extruder_target) {
        get_mks_extruder_target();
        set_extruder_target(mks_extruder_target);
    } else {
        set_extruder_target(0);
    }
}

void open_start_extrude() {
    ep->Send(json_run_a_gcode("M83\nG1 E20 F300\n"));
}

void open_calibrate_start() {
    step_1 = false; // CLL 当该值为true表示打印机已完成平台和喷嘴位置初始化
    step_2 = false; // CLL 当该值为true表示打印机已完成采集补偿值
    step_3 = false; // CLL 当该值为true表示打印机已完成共振补偿
    printer_idle_timeout_state = "Printing";
    page_to(TJC_PAGE_OPEN_CALIBRATE);
    ep->Send(json_run_a_gcode("M4028")); // printer.cfg中文件自定义gcode "M4028"
}

void close_mcu_port() {
    ep->Send(json_run_a_gcode("CLOSE_MCU_PORT\n"));
}

void oobe_set_intern_zoffset(float offset) {
    oobe_printer_set_offset = offset;
}

void oobe_set_zoffset(bool positive) {
    if (positive == true) {
        ep->Send(json_run_a_gcode("SET_GCODE_OFFSET Z_ADJUST=+" + std::to_string(oobe_printer_set_offset) + " MOVE=1"));
    } else {
        ep->Send(json_run_a_gcode("SET_GCODE_OFFSET Z_ADJUST=-" + std::to_string(oobe_printer_set_offset) + " MOVE=1"));
    }
}

void refresh_page_zoffset() {
    std::string temp[5][5];
    for (int i = 0; i < printer_bed_mesh_profiles_mks_mesh_params_y_count; i++) {
        if (i == 5) {
            break;
        }
        for (int j = 0; j < printer_bed_mesh_profiles_mks_mesh_params_x_count; j++) {
            if (j == 5) {
                break;
            }
            temp[i][j] = std::to_string(printer_bed_mesh_profiles_mks_points[i][j]);
            temp[i][j] = temp[i][j].substr(0, temp[i][j].find(".") + 3);
            send_cmd_txt(tty_fd, "t" + std::to_string(5 * i + j), temp[i][j]);
        }
    }
}

void refresh_page_auto_heaterbed() {
    send_cmd_txt(tty_fd, "t0", std::to_string(printer_heater_bed_temperature) + "/");
    send_cmd_val(tty_fd, "n0", std::to_string(printer_heater_bed_target));
    if (printer_heater_bed_target > 0) {
        send_cmd_picc(tty_fd, "b2", "106");
        send_cmd_picc2(tty_fd, "b2", "105");
        send_cmd_pco(tty_fd, "t0", "63488");
        send_cmd_pco(tty_fd, "n0", "63488");
    } else {
        send_cmd_picc(tty_fd, "b2", "107");
        send_cmd_picc2(tty_fd, "b2", "104");
        send_cmd_pco(tty_fd, "t0", "65535");
        send_cmd_pco(tty_fd, "n0", "65535");
    }
}

void set_auto_level_heater_bed_target(bool positive) {
    get_mks_heater_bed_target();
    printer_auto_level_heater_bed_target = mks_heater_bed_target;
    if (positive == true) {
        printer_auto_level_heater_bed_target += 3;
    }else {
        printer_auto_level_heater_bed_target -= 3;
    }
    if (printer_auto_level_heater_bed_target >120) {
        printer_auto_level_heater_bed_target =120;
    }
    if (printer_auto_level_heater_bed_target < 0) {
        printer_auto_level_heater_bed_target = 0;
    }
    set_heater_bed_target(printer_auto_level_heater_bed_target);
    set_mks_heater_bed_target(printer_auto_level_heater_bed_target);
}

void detect_error() {
    switch (current_page_id)
    {
    case TJC_PAGE_PRINTING:
    case TJC_PAGE_PRINT_ZOFFSET:
    case TJC_PAGE_PRINT_FILAMENT:
    case TJC_PAGE_PRINTING_2:
    case TJC_PAGE_GCODE_ERROR:
    case TJC_PAGE_LEVEL_ERROR:
    case TJC_PAGE_DETECT_ERROR:
    case TJC_PAGE_UPDATING:
        break;
    
    case TJC_PAGE_OPEN_CALIBRATE:
    case TJC_PAGE_AUTO_MOVING:
        ep->Send(json_run_a_gcode("RESTART"));
        jump_to_level_error = true;
        break;

    default:
        if (printer_webhooks_state != "shutdown" && printer_webhooks_state != "error") {
            //4.1.5 新增息屏功能
            if (previous_caselight_value == true) {
                led_on_off();
                previous_caselight_value = false;
            }
            jump_to_detect_error = true;
        }
        break;
    }
}

void clear_previous_data() {
    sdcard_reset_file();
    clear_cp0_image();
    clear_page_preview();
    show_preview_complete = false;
    printing_keyboard_enabled = false;
}

void print_start() {
    if (printer_bed_leveling == true) {
        ep->Send(json_run_a_gcode("G31\n"));
    } else {
        ep->Send(json_run_a_gcode("G32\n"));
    }
}

void open_heater_bed_up() {
    page_to(TJC_PAGE_OPEN_FILAMENTVIDEO_1);
    ep->Send(json_run_a_gcode("SET_KINEMATIC_POSITION Z=150\nSET_KINEMATIC_POSITION X=150\nSET_KINEMATIC_POSITION Y=150\n"));
    ep->Send(json_run_a_gcode("G91\nG1 Z-30 F600\nG1 X-30 Y-30 F1200\nG90\nM84\n"));
}

void refresh_page_open_heaterbed() {
    send_cmd_txt(tty_fd, "t0", std::to_string(printer_heater_bed_temperature) + "/");
    send_cmd_val(tty_fd, "n0", std::to_string(printer_heater_bed_target));
    if (printer_heater_bed_target > 0) {
        send_cmd_picc(tty_fd, "b0", "18");
        send_cmd_picc2(tty_fd, "b0", "14");
        send_cmd_pco(tty_fd, "t0", "63488");
        send_cmd_pco(tty_fd, "n0", "63488");
    } else {
        send_cmd_picc(tty_fd, "b0", "15");
        send_cmd_picc2(tty_fd, "b0", "13");
        send_cmd_pco(tty_fd, "t0", "65535");
        send_cmd_pco(tty_fd, "n0", "65535");
    }
}

void bed_leveling_switch(bool positive) {
    if (positive == true) {
        ep->Send(json_run_a_gcode("G31"));
        printer_bed_leveling = true;
    }
    if (positive == false) {
        ep->Send(json_run_a_gcode("G32"));
        printer_bed_leveling = false;
    }
}

void save_current_zoffset() {
    std::string z_offset = std::to_string(printer_gcode_move_homing_origin[2]);
    z_offset = z_offset.substr(0, z_offset.find(".") + 4);
    switch (current_page_id)
    {
    case TJC_PAGE_AUTO_MOVING:
    case TJC_PAGE_OPEN_CALIBRATE:
        printer_idle_timeout_state = "Printing";
        float z;
        get_mks_babystep();
        z = std::stof(mks_babystep_value) + std::stof(mks_adxl_offset);
        if (z > -5 && z < 5) { // CLL 仅保存-5~5的zoffset值
            mks_babystep_value = std::to_string(z);
            set_mks_babystep(mks_babystep_value);
            MKSLOG_RED("当前zoffset值保存为%s",mks_babystep_value.c_str());
        }
        break;
    
    default:
        if (z_offset != mks_babystep_value && z_offset.find("0.000") != -1) {
            if (std::stof(z_offset) > -5 && std::stof(z_offset) < 5) {
                mks_babystep_value = z_offset;
                set_mks_babystep(mks_babystep_value);
                MKSLOG_RED("当前zoffset值保存为:%s",mks_babystep_value.c_str());
            }
        }
        break;
    }
}

void refresh_page_filament_pop() {
    send_cmd_txt(tty_fd, "t4", "(" + std::to_string(printer_extruder_temperature) + "/" + std::to_string(printer_extruder_target) + "℃)");
    if (step_1 == true) {
        step_1 = false;
        send_cmd_picc(tty_fd, "q0", "195");
        send_cmd_pco(tty_fd, "t1", "65535");
        send_cmd_pco(tty_fd, "t0", "38066");
        send_cmd_pco(tty_fd, "t4", "38066");
        send_cmd_vis(tty_fd, "gm2", "0");
        send_cmd_vis(tty_fd, "gm3", "1");
    }
    if (step_2 == true && printer_idle_timeout_state == "Ready") {
        step_2 = false;
        send_cmd_picc(tty_fd, "q0", "197");
        send_cmd_pco(tty_fd, "t2", "65535");
        send_cmd_pco(tty_fd, "t1", "38066");
        send_cmd_vis(tty_fd, "b0", "1");
        send_cmd_vis(tty_fd, "b1", "1");
        send_cmd_vis(tty_fd, "gm3", "0");
    }
}

void check_filament_type() {
    std::string filament_type;
    if (file_metadata_filament_type != "") {
        filament_type = file_metadata_filament_type;
    } else {
        filament_type = file_metadata_filament_name;
    }
    std::transform(filament_type.begin(), filament_type.end(), filament_type.begin(), tolower);
    MKSLOG_YELLOW("filament_type : %s",filament_type.c_str());
    //4.4.1 CLL 耗材确认弹窗新增不再提示按钮
    if ((filament_type.find("pla") != -1 || filament_type.find("petg") != -1) && preview_pop_1_on == true) {
        page_to(TJC_PAGE_PREVIEW_POP_1);
    }else if (filament_type.find("abs") != -1 && preview_pop_2_on == true) {
        page_to(TJC_PAGE_PREVIEW_POP_2);
    }else {
        page_to(TJC_PAGE_PRINTING);
    }
}

void refresh_page_preview_pop() {
    //4.4.2 使mates与霍尔耗材宽度探测器适配
    if (filament_detected == false) {
        sleep(1);
        set_print_pause();
        page_to(TJC_PAGE_PRINT_NO_FILAMENT);
    }
    if (printer_print_stats_state == "standby") {
        page_to(TJC_PAGE_PRINT_STOPPING);
    }
    if (printer_print_stats_state == "error") {
        page_to(TJC_PAGE_GCODE_ERROR);
        cancel_print();
        clear_previous_data();
        send_cmd_txt(tty_fd, "t0", "gcode error:" + error_message);
    }
}

std::string replaceCharacters(const std::string& path, const std::string& searchChars, const std::string& replacement) {
    std::string result = path;
    for (char c : searchChars) {
        std::size_t found = result.find(c);
        while (found != std::string::npos) {
            result.replace(found, 1, replacement);
            found = result.find(c, found + replacement.length());
        }
    }
    return result;
}

//4.4.2 使mates适配霍尔宽度检测器
void check_filament_width() {
    if (filament_message.find("// Filament dia (measured mm):") != -1) {
        float filament_width = std::stof(filament_message.substr(31));
        MKSLOG("耗材宽度：%f",filament_width);
        if (filament_width < 0.3){
            filament_detected = false;
        } else {
            filament_detected = true;
        }
    } else if (filament_message.find("// Filament NOT present") != -1 || filament_message.find("echo: Filament run out") != -1) {
        filament_detected = false;
    }
}

//4.4.2 CLL 新增刷新文件列表函数
void refresh_page_files_list_2() {
    if (file_mode == "USB") {
        if (detect_disk() == -1) {
            file_mode = "NULL";
            page_to(TJC_PAGE_FILE_LIST);
            page_files_pages = 0;
            page_files_current_pages = 0;
            page_files_folder_layers = 1;
            page_files_previous_path = "";
            page_files_root_path = "gcodes/";
            page_files_path = "/sda1";
            refresh_page_files(page_files_current_pages);
            refresh_page_files_list();
            get_object_status();
        }
    } else if (file_mode == "NULL") {
        if (detect_disk() == 0) {
            sleep(1);
            file_mode ="USB";
            page_to(TJC_PAGE_FILE_LIST);
            page_files_pages = 0;
            page_files_current_pages = 0;
            page_files_folder_layers = 1;
            page_files_previous_path = "";
            page_files_root_path = "gcodes/";
            page_files_path = "/sda1";
            refresh_page_files(page_files_current_pages);
            refresh_page_files_list();
            get_object_status();
        }
    }
}

void go_to_update() {
    get_mks_connection_method();
    page_to(TJC_PAGE_UPDATE_MODE);
    send_cmd_txt(tty_fd, "t1", mks_version_soc);
    if (connection_method == 0)
        send_cmd_vis(tty_fd, "b2", "0");
}

void restore_config() {
    system("rm /home/mks/gcode_files/.cache/*");
    main_picture_refreshed = false;
    system("curl -X POST http://127.0.0.1:7125/server/history/reset_totals");
    system("curl -X DELETE 'http://127.0.0.1:7125/server/history/job?all=true'");
    system("cp /root/config.mksini /home/mks/klipper_config/config.mksini");
    system("cp /home/mks/klipper_config/saved_variables.cfg.bak /home/mks/klipper_config/saved_variables.cfg");
    ep->Send(json_run_a_gcode("SAVE_VARIABLE VARIABLE=z_offset VALUE=0"));
    page_to(TJC_PAGE_MAIN);
}

void refresh_page_bed_moving() {
    if (printer_idle_timeout_state == "Ready") {
        if (manual_count == 3) {
            page_to(TJC_PAGE_PRE_BED_CALIBRATION);
        } else if (manual_count == -2) {
            page_to(TJC_PAGE_BED_FINISH);
        } else {
            page_to(TJC_PAGE_BED_CALIBRATION);
        }
    }
}

void bed_calibrate() {
    if (manual_count == 4) {
        bed_offset = 0;
        printer_idle_timeout_state = "Printing";
        ep->Send(json_run_a_gcode("ABORT\n"));
        ep->Send(json_run_a_gcode("M4030"));
        page_to(TJC_PAGE_BED_MOVING);
    }else if (manual_count == 3) {
        printer_idle_timeout_state = "Printing";
        ep->Send(json_run_a_gcode("G1 Z10 F600"));
        ep->Send(json_run_a_gcode("BED_SCREWS_ADJUST\n"));
        ep->Send(json_run_a_gcode("G1 Z" + std::to_string(bed_offset) + " F600\n"));
        MKSLOG_BLUE("当前bed_offset:%f", bed_offset); 
        page_to(TJC_PAGE_BED_MOVING);
    } else if (manual_count > 0) {
        printer_idle_timeout_state = "Printing";
        ep->Send(json_run_a_gcode("ACCEPT\n"));
        ep->Send(json_run_a_gcode("G1 Z" + std::to_string(bed_offset) + " F600\n"));
        page_to(TJC_PAGE_BED_MOVING);
    } else if (manual_count == 0) {
        ep->Send(json_run_a_gcode("ACCEPT\n"));
        ep->Send(json_run_a_gcode("G1 Z10 F600\nG1 X0 Y0 F9000\n"));
        init_mks_status();
        page_to(TJC_PAGE_BED_FINISH);
    } else {
        ep->Send(json_run_a_gcode("G1 Z10 F600\n"));
        page_to(TJC_PAGE_BED_FINISH);
    }
    manual_count--;
}

void bed_adjust(bool status) {
    if (status == true) {
        ep->Send(json_run_a_gcode("G91\nG1 Z" + std::to_string(-auto_level_dist) + " F600\nG90\n"));
        bed_offset = bed_offset - auto_level_dist;
        MKSLOG_BLUE("当前bed_offset:%f", bed_offset);
    } else if (status == false) {
        ep->Send(json_run_a_gcode("G91\nG1 Z" + std::to_string(auto_level_dist) + " F600\nG90\n"));
        bed_offset = bed_offset + auto_level_dist;
        MKSLOG_BLUE("当前bed_offset:%f", bed_offset);
    }
}

void go_to_file_list() {
    if (file_mode == "Local") {
        page_to(TJC_PAGE_FILE_LIST);
        page_files_pages = 0;
        page_files_current_pages = 0;
        page_files_folder_layers = 0;
        page_files_previous_path = "";
        page_files_root_path = DEFAULT_DIR;
        page_files_path = "";
        refresh_page_files(page_files_current_pages);
        refresh_page_files_list();
        get_object_status();
    } else {
        page_to(TJC_PAGE_FILE_LIST);
        page_files_pages = 0;
        page_files_current_pages = 0;
        page_files_folder_layers = 1;
        page_files_previous_path = "";
        page_files_root_path = DEFAULT_DIR;
        page_files_path = "/sda1";
        refresh_page_files(page_files_current_pages);
        refresh_page_files_list();
        get_object_status();
    }
}

void send_gcode(std::string command) {
    ep->Send(json_run_a_gcode(command));
}

void refresh_page_open_calibrate() {
    if (step_3 == true) {
        step_3 = false;
        system("sync"); // CLL 保存系统信息后跳转至装料页面
        sleep(10);
        get_object_status();
        sub_object_status();
        page_to(TJC_PAGE_OPEN_FILAMENTVIDEO_0);
    }
    if (step_2 == true && printer_webhooks_state == "ready") {
        step_2 = false;
        send_cmd_picc(tty_fd, "q0", "21");
        send_cmd_pco(tty_fd, "t3", "65535");
        sleep(5);
        ep->Send(json_run_a_gcode("M901")); // CLL 平台调平完成后进行共振补偿
    }
    if (step_1 == true && printer_idle_timeout_state == "Ready") {
        step_1 = false;
        send_cmd_picc(tty_fd, "q0", "20");
        send_cmd_pco(tty_fd, "t2", "65535");
        get_mks_heater_bed_target();
        set_heater_bed_target(mks_heater_bed_target);
        ep->Send(json_run_a_gcode("M190 S" + std::to_string(mks_heater_bed_target) + "\n"));
        sleep(5);
        ep->Send(json_run_a_gcode("M4027")); // CLL 平台和喷嘴位置初始化完成后进行调平
    }
}

void refresh_page_filament_set_fan() {
    if (move_fan_setting == false) { // CLL 当检测到风扇滑块未被按下时执行刷新函数
        send_cmd_val(tty_fd, "h0", std::to_string((int)(printer_out_pin_fan0_value * 100)));
        send_cmd_val(tty_fd, "n0", std::to_string((int)(printer_out_pin_fan0_value * 100)));
        send_cmd_val(tty_fd, "h1", std::to_string((int)(printer_out_pin_fan2_value * 100)));
        send_cmd_val(tty_fd, "n1", std::to_string((int)(printer_out_pin_fan2_value * 100)));
        send_cmd_val(tty_fd, "h2", std::to_string((int)(printer_out_pin_fan3_value * 100)));
        send_cmd_val(tty_fd, "n2", std::to_string((int)(printer_out_pin_fan3_value * 100)));
        if (printer_out_pin_fan0_value == 0) {
            send_cmd_picc(tty_fd, "b0", "201");
            send_cmd_picc2(tty_fd, "b0", "199");
        } else {
            send_cmd_picc(tty_fd, "b0", "202");
            send_cmd_picc2(tty_fd, "b0", "200");
        }

        if (printer_out_pin_fan2_value == 0) {
            send_cmd_picc(tty_fd, "b1", "201");
            send_cmd_picc2(tty_fd, "b1", "199");
        } else {
            send_cmd_picc(tty_fd, "b1", "202");
            send_cmd_picc2(tty_fd, "b1", "200");
        }

        if (printer_out_pin_fan3_value == 0) {
            send_cmd_picc(tty_fd, "b2", "201");
            send_cmd_picc2(tty_fd, "b2", "199");
        } else {
            send_cmd_picc(tty_fd, "b2", "202");
            send_cmd_picc2(tty_fd, "b2", "200");
        }
    }
}

// CLL 记住上次进入调整页面时的选择
void go_to_adjust() {
    if (adjust_mode == "Filament") {
        page_to(TJC_PAGE_FILAMENT);
    } else {
        page_to(TJC_PAGE_MOVE);
    }
}
void go_to_setting() {
    if (set_mode == "Level_mode") {
        page_to(TJC_PAGE_LEVEL_MODE);
    } else {
        page_to(TJC_PAGE_COMMON_SETTING);
    }
}

void refresh_page_common_setting() {
    current_mks_oobe_enabled = get_mks_oobe_enabled();
    send_cmd_txt(tty_fd, "t7", mks_version_soc);
    if (current_mks_oobe_enabled == false) {
        send_cmd_picc(tty_fd, "b6", "137");
        send_cmd_picc2(tty_fd, "b6", "135");
    } else {
        send_cmd_picc(tty_fd, "b6", "138");
        send_cmd_picc2(tty_fd, "b6", "136");
        send_cmd_txt(tty_fd, "t6", "开机引导");
    }
}

// CLL 该函数用于执行导出日志操作
void print_log() {
    if (detect_disk() == -1) {
        page_to(TJC_PAGE_PRINT_LOG_F); // CLL 若U盘未插入，提示导出失败，请客户检查U盘插入
    } else {
        system("mkdir /home/mks/gcode_files/sda1/QD_Log");
        system("bash -c 'cp /home/mks/klipper_logs/klippy.log* /home/mks/gcode_files/sda1/QD_Log/'");
        system("bash -c 'cp /home/mks/klipper_logs/moonraker.log* /home/mks/gcode_files/sda1/QD_Log/'");
        system("bash -c 'cp /home/mks/klipper_logs/auto_update.log* /home/mks/gcode_files/sda1/QD_Log/'");
        system("bash -c 'cp /root/frp/frpc.log* /home/mks/gcode_files/sda1/QD_Log/'");
        system("bash -c 'cp /root/frp/frpc.*.log /home/mks/gcode_files/sda1/QD_Log/'");
        system("cp /root/frp/frpc.toml /home/mks/gcode_files/sda1/QD_Log/server.cfg");
        page_to(TJC_PAGE_PRINT_LOG_S);
    }
}

void refresh_files_list_picture(std::string path, int pixel, int i) {
    file_metadata_simage.clear();
    file_metadata_gimage.clear();
    mks_file_parse_finished = false;
    output_imgdata(path, pixel);
    std::ifstream infile("/home/mks/tjc"); // 打开文件
    if (!infile) {
        std::cerr << "无法打开文件 " << "/home/mks/tjc" << std::endl;
        show_preview_complete = true;
        return;
    }
    std::stringstream buffer;
    buffer << infile.rdbuf();
    file_metadata_gimage = buffer.str();
    infile.close();
    send_cmd_baud(tty_fd, 921600);
    //usleep(50000);
    usleep(10000);
    set_option(tty_fd, 921600, 8, 'N', 1);
    send_cmd_cp_close(tty_fd, "cp"+std::to_string(i));
    if (file_metadata_gimage != "") {
        std::cout << "刷文件图" << std::endl;
        int num = 2048;
        int len = file_metadata_gimage.length();
        int end = num;
        std::string g;
        for (int start = 0; start < len;) {
            if (end > len) {
                g = file_metadata_gimage.substr(start, len - start);
                tcdrain(tty_fd);
                send_cmd_cp_image(tty_fd, "cp" + std::to_string(i), g);
                break;
            }
            g = file_metadata_gimage.substr(start, num);
            start = end;
            end = end + num;
            tcdrain(tty_fd);
            send_cmd_cp_image(tty_fd, "cp" + std::to_string(i), g);
        }
    }
    send_cmd_baud(tty_fd, 115200);
    //usleep(50000);
    usleep(10000);
    set_option(tty_fd, 115200, 8, 'N', 1);
    send_cmd_vis(tty_fd, "cp" + std::to_string(i), "1");
}

std::string input_path;
int input_size;
bool start_path = false;
void refresh_files_list_picture_2(std::string path, int size, int i) {
    input_path = path;
    input_size = size;
    begin_show_64_jpg = true;
}

int refresh_files_list_picture_3(const char *inputPath, int size, int i) {
    return 0;
}

void refresh_page_filament() {
    send_cmd_txt(tty_fd, "t0", std::to_string(printer_extruder_temperature));
    send_cmd_val(tty_fd, "n0", std::to_string(printer_extruder_target));
    send_cmd_txt(tty_fd, "t1", std::to_string(printer_heater_bed_temperature));
    send_cmd_val(tty_fd, "n1", std::to_string(printer_heater_bed_target));
    send_cmd_txt(tty_fd, "t2", std::to_string(printer_hot_temperature));
    send_cmd_val(tty_fd, "n2", std::to_string(printer_hot_target));
    if (printer_extruder_target > 0) {// CLL 根据喷头是否加热改变按钮显示状态
        send_cmd_picc(tty_fd, "b2", "177");
        send_cmd_picc2(tty_fd, "b2", "174");
        send_cmd_picc(tty_fd, "b0", "177");
        send_cmd_picc2(tty_fd, "b0", "174");
        send_cmd_pco(tty_fd, "t0", "63488");
    } else {
        send_cmd_picc(tty_fd, "b2", "176");
        send_cmd_picc2(tty_fd, "b2", "175");
        send_cmd_picc(tty_fd, "b0", "176");
        send_cmd_picc2(tty_fd, "b0", "175");
        send_cmd_pco(tty_fd, "t0", "65535");
    }

    if (printer_heater_bed_target > 0) { // CLL 根据热床是否加热改变按钮显示状态
        send_cmd_picc(tty_fd, "b3", "177");
        send_cmd_picc2(tty_fd, "b3", "174");
        send_cmd_picc(tty_fd, "b1", "177");
        send_cmd_picc2(tty_fd, "b1", "174");
        send_cmd_pco(tty_fd, "t1", "63488");
    } else {
        send_cmd_picc(tty_fd, "b3", "176");
        send_cmd_picc2(tty_fd, "b3", "175");
        send_cmd_picc(tty_fd, "b1", "176");
        send_cmd_picc2(tty_fd, "b1", "175");
        send_cmd_pco(tty_fd, "t1", "65535");
    }

    if (printer_hot_target > 0) {
        send_cmd_picc(tty_fd, "b12", "177");
        send_cmd_picc2(tty_fd, "b12", "174");
        send_cmd_picc(tty_fd, "b13", "177");
        send_cmd_picc2(tty_fd, "b13", "174");
        send_cmd_pco(tty_fd, "t2", "63488");
    } else {
        send_cmd_picc(tty_fd, "b12", "176");
        send_cmd_picc2(tty_fd, "b12", "175");
        send_cmd_picc(tty_fd, "b13", "176");
        send_cmd_picc2(tty_fd, "b13", "175");
        send_cmd_pco(tty_fd, "t2", "65535");
    }

    if (printer_filament_extruedr_dist == 10) {
        send_cmd_picc(tty_fd, "b9", "177");
        send_cmd_picc2(tty_fd, "b9", "174");
        send_cmd_picc(tty_fd, "b10", "176");
        send_cmd_picc2(tty_fd, "b10", "175");
        send_cmd_picc(tty_fd, "b11", "176");
        send_cmd_picc2(tty_fd, "b11", "175");
    } else if (printer_filament_extruedr_dist == 50) {
        send_cmd_picc(tty_fd, "b9", "176");
        send_cmd_picc2(tty_fd, "b9", "175");
        send_cmd_picc(tty_fd, "b10", "177");
        send_cmd_picc2(tty_fd, "b10", "174");
        send_cmd_picc(tty_fd, "b11", "176");
        send_cmd_picc2(tty_fd, "b11", "175");
    } else if (printer_filament_extruedr_dist == 100) {
        send_cmd_picc(tty_fd, "b9", "176");
        send_cmd_picc2(tty_fd, "b9", "175");
        send_cmd_picc(tty_fd, "b10", "176");
        send_cmd_picc2(tty_fd, "b10", "175");
        send_cmd_picc(tty_fd, "b11", "177");
        send_cmd_picc2(tty_fd, "b11", "174");
    }
}

void get_mks_connection_method() {
    mksini_load();
    connection_method = mksini_getint("app_connection", "method", 0);
    mksini_free();
}

void set_mks_connection_method(int target) {
    qr_refreshed = false;
    mksini_load();
    std::cout << "######## " << target << std::endl;
    mksini_set("app_connection", "method", std::to_string(target));
    mksini_save();
    mksini_free();
    system("sync");
}

void refresh_page_show_qr() {
    if (mks_ethernet == 0) {
        send_cmd_txt(tty_fd, "t0", status_result.ip_address);
        send_cmd_picc(tty_fd, "b4", "268");
        send_cmd_picc2(tty_fd, "b4", "214");
    } else {
        std::string local_ip = get_eth0_ip();
        send_cmd_txt(tty_fd, "t0", local_ip);
        send_cmd_picc(tty_fd, "b4", "269");
        send_cmd_picc2(tty_fd, "b4", "215");
    }
}

void go_to_showqr() {
    std::string qrmessage;
    get_mks_connection_method();
    page_to(TJC_PAGE_SHOW_QR);
    if (strcmp(status_result.wpa_state, "COMPLETED") == 0 || mks_ethernet) {
        if (qr_refreshed == false) {
            send_cmd_cp_close(tty_fd, "cp0");
            if (open_qr_refreshed == true || access("/home/mks/qrcode/qrcode.jpg", F_OK) == -1)
                qrmessage = run_python_code("python3 /home/mks/qrcode/qrcode_QD.py 176\n");
            std::cout << "qrmessage:" << qrmessage << std::endl;
            if (qrmessage.find("Missing or invalid") != -1) {
                send_cmd_txt(tty_fd, "t4", qrmessage);
                send_cmd_vis(tty_fd, "t4", "1");
            } else if (qrmessage.find("No") != -1) {
                send_cmd_vis(tty_fd, "t4", "1");
            } else {
                open_qr_refreshed = true;
                refresh_files_list_picture("/home/mks/qrcode/qrcode.jpg", 176, 0);
                qr_refreshed = true;
            }
        }
    } else {
        send_cmd_vis(tty_fd, "t4", "1");
        send_cmd_cp_close(tty_fd, "cp0");
    }
}

void get_mks_selected_server() {
    mksini_load();
    selected_server = mksini_getstring("app_server", "name", "");
    mksini_free();
    std::cout << selected_server << std::endl;
}

void go_to_server_set(int n) {
    current_server_page = n;
    total_server_count = 0;
    serverConfigs.clear();
    if (connection_method == 1 && strcmp(status_result.wpa_state, "COMPLETED") == 0) {
        page_to(TJC_PAGE_SEARCH_SERVER);
        update_server(0);
        get_mks_selected_server();
        get_mks_connection_method();
    }
    page_to(TJC_PAGE_SERVER_SET);
}

void updateServerConfig(std::vector<std::string> &lines, const Server_config &config)
{
    for (size_t i = 0; i < lines.size(); ++i)
    {
        if (lines[i] == "[app_server]")
        {
            // 检查是否已经到达文件末尾
            if (i + 1 < lines.size())
            {
                // 直接更新下一行，无需考虑空格数量
                lines[i + 1] = "name = " + config.name;
                return;
            }
        }
    }

    // 如果没有找到[app_server]节，就在文件末尾添加
    lines.push_back("[app_server]");
    lines.push_back("name = " + config.name);
}

void update_server(int choice)
{
    // CLL 连接服务器获取json文件 
    if (choice == 0) {
        // system("curl -s -S -L -o /root/frp/server_list.json http://www.aws.qidi3dprinter.com:5050/downloads/server_list.json");
        get_mks_selected_server();
        std::string server_for_command = selected_server.empty() ? "aws" : selected_server;
        std::string command = "curl -s -S -L -o /root/frp/server_list.json http://www." + server_for_command + ".qidi3dprinter.com:5050/downloads/server_list.json";
        std::cout << "Executing command: " << command << std::endl;
        system(command.c_str());
    } else {
        qr_refreshed = false; // CLL 当choice不为0时，该函数用于切换服务器，需要重新刷新二维码
    }

    total_server_count = 0;
    const std::string FRPC_CONFIG_PATH = "/root/frp/frpc.toml";
    const std::string MKSCONFIG_PATH = "/home/mks/klipper_config/config.mksini";
    const std::string SERVER_LIST_PATH = "/root/frp/server_list.json"; // JSON 文件路径
    
    try {
        // 读取 JSON 文件并解析为服务器配置
        std::ifstream jsonFile(SERVER_LIST_PATH);
        nlohmann::json serverList;
        jsonFile >> serverList;
        jsonFile.close();
        
        for (const auto& item : serverList.items())
        {
            const std::string &key = item.key();  
            const auto &value = item.value();  

            int id = std::stoi(key);
            Server_config config = {value["address"], value["name"]};
            std::cout << id << ":" << config.name << std::endl;
            serverConfigs[id] = config;
            total_server_count++;
        }
    }
    catch(const std::exception& e)
	{
		std::cerr << "fail_to_read:" << e.what() << '\n';
	}

    if (choice != 0) {
        if (serverConfigs.find(choice) == serverConfigs.end())
        {
            std::cout << "Invalid choice. Please enter a valid option.\n";
            return;
        }

        Server_config config = serverConfigs[choice];

        // 停止frpc.service
        std::cout << "Stopping frpc.service...\n";
        system("sudo systemctl stop frpc.service");

        // 更新frpc.toml
        std::ifstream frpcIn(FRPC_CONFIG_PATH);
        std::stringstream buffer;
        buffer << frpcIn.rdbuf();
        std::string content = buffer.str();
        frpcIn.close();

        size_t pos = content.find("serverAddr = ");
        if (pos != std::string::npos)
        {
            size_t endPos = content.find('\n', pos);
            content.replace(pos, endPos - pos, "serverAddr = \"" + config.address + "\"");
        }

        std::ofstream frpcOut(FRPC_CONFIG_PATH);
        frpcOut << content;
        frpcOut.close();

        // 读取config.mksini到vector中
        std::vector<std::string> lines;
        std::ifstream mksIn(MKSCONFIG_PATH);
        std::string line;
        while (getline(mksIn, line))
        {
            lines.push_back(line);
        }
        mksIn.close();

        // 更新app_server配置
        updateServerConfig(lines, config);

        // 将更新后的内容写回config.mksini
        std::ofstream mksOut(MKSCONFIG_PATH);
        for (const auto &outputLine : lines)
        {
            mksOut << outputLine << std::endl;
        }
        mksOut.close();

        // 重启frpc.service
        std::cout << "Starting frpc.service...\n";
        system("sudo systemctl start frpc.service");

        std::cout << "Configuration updated to " << config.name << " successfully.\n";
        get_mks_selected_server();
    }
}

void refresh_page_server_set() {
    if (connection_method == 0 || strcmp(status_result.wpa_state, "COMPLETED") != 0) {
        send_cmd_picc(tty_fd, "b2", "217");
        send_cmd_picc2(tty_fd, "b2", "220");
        send_cmd_vis(tty_fd, "t3", "1");
        send_cmd_vis(tty_fd, "t0", "0");
        send_cmd_vis(tty_fd, "b3", "0");
        send_cmd_vis(tty_fd, "b4", "0");
        send_cmd_vis(tty_fd, "b0", "0");
    } else {
        send_cmd_picc(tty_fd, "b2", "218");
        send_cmd_picc2(tty_fd, "b2", "219");
        send_cmd_vis(tty_fd, "t3", "0");
        send_cmd_vis(tty_fd, "t0", "1");
        send_cmd_vis(tty_fd, "b3", "1");
        send_cmd_vis(tty_fd, "b4", "1");
        send_cmd_vis(tty_fd, "b0", "1");

    }
    
    if (current_server_page == 0) {
        send_cmd_picc(tty_fd, "b3", "218");
        send_cmd_picc2(tty_fd, "b3", "219");
    } else {
        send_cmd_picc(tty_fd, "b3", "217");
        send_cmd_picc2(tty_fd, "b3", "220");
    }

    if ((current_server_page + 1) * 4 >= total_server_count) {
        send_cmd_picc(tty_fd, "b4", "218");
        send_cmd_picc2(tty_fd, "b4", "219");
    } else {
        send_cmd_picc(tty_fd, "b4", "217");
        send_cmd_picc2(tty_fd, "b4", "220");
    }
    for (int i = 0; i < 4; i++) {
        if (i + current_server_page * 4 + 1 > total_server_count)
            break;
        send_cmd_txt(tty_fd, "t" + std::to_string(i + 5), serverConfigs[1 + i + current_server_page * 4].name);
        if (selected_server == serverConfigs[1 + i + current_server_page * 4].name) {
            std::cout << "selected_server:" << serverConfigs[i + 1 + current_server_page * 4].name << std::endl;
            send_cmd_picc(tty_fd, "b" + std::to_string(i + 5), "217");
            send_cmd_picc2(tty_fd, "b" + std::to_string(i + 5), "219");
        } else {
            std::cout << "unselected_server:" << serverConfigs[i + 1 + current_server_page * 4].name << std::endl;
            send_cmd_picc(tty_fd, "b" + std::to_string(i + 5), "218");
            send_cmd_picc2(tty_fd, "b" + std::to_string(i + 5), "220");
        }
    }
}

void local_update () {
    ep->Send(json_get_klippy_host_information());
    if (detect_update()) {
        page_to(TJC_PAGE_UPDATE_FOUND);
    } else {
        page_to(TJC_PAGE_UPDATE_NOT_FOUND);
    }
}

// CLL 此函数用于调用系统函数后接收返回值
std::string run_python_code(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

void check_online_version() {
    page_to(TJC_PAGE_SEARCH_SERVER);
    if (connection_method == 0)
        return;
    target_soc_version = run_python_code("python3 /root/auto_update/version_check.py");
    std::cout << "服务器版本:" << target_soc_version << std::endl;
    if (target_soc_version.find("0") == 0) {
        page_to(TJC_PAGE_UPDATE_MODE);
        send_cmd_vis(tty_fd, "t2", "1");
        send_cmd_vis(tty_fd, "t3", "0");
    } else if (target_soc_version.find("-1") == 0) {
        page_to(TJC_PAGE_UPDATE_MODE);
        send_cmd_vis(tty_fd, "t3", "1");
        send_cmd_vis(tty_fd, "t2", "0");
    } else {
        page_to(TJC_PAGE_ONLINE_UPDATE);
        send_cmd_txt(tty_fd, "t1", mks_version_soc);
        send_cmd_txt(tty_fd, "t2", target_soc_version);
        updateini_load();
        std::string cn_info = mksini_getstring("cn", "content", "NULL"); // CLL 获取中文更新说明
        std::string ru_info = mksini_getstring("ru", "content", "NULL"); // CLL 获取俄语更新说明
        std::string en_info = mksini_getstring("en", "content", "NULL"); // CLL 获取英语更新说明
        std::string jp_info = mksini_getstring("jp", "content", "NULL"); // CLL 获取日语更新说明
        std::string fr_info = mksini_getstring("fr", "content", "NULL"); // CLL 获取法语更新说明
        std::string gr_info = mksini_getstring("gr", "content", "NULL"); // CLL 获取德语更新说明
        std::string it_info = mksini_getstring("it", "content", "NULL"); // CLL 获取意大利语更新说明
        std::string sp_info = mksini_getstring("sp", "content", "NULL"); // CLL 获取西班牙语更新说明
        std::string kr_info = mksini_getstring("kr", "content", "NULL"); // CLL 获取韩语更新说明
        std::string pr_info = mksini_getstring("pr", "content", "NULL"); // CLL 获取葡萄牙语更新说明
        std::string ar_info = mksini_getstring("ar", "content", "NULL"); // CLL 获取阿拉伯语更新说明
        std::string tr_info = mksini_getstring("tr", "content", "NULL"); // CLL 获取土耳其语更新说明
        std::string hb_info = mksini_getstring("hb", "content", "NULL"); // CLL 获取希伯来语更新说明
        send_cmd_txt(tty_fd, "t_cn", cn_info);
        send_cmd_txt(tty_fd, "t_ru", ru_info);
        send_cmd_txt(tty_fd, "t_en", en_info);
        send_cmd_txt(tty_fd, "t_jp", jp_info);
        send_cmd_txt(tty_fd, "t_fr", fr_info);
        send_cmd_txt(tty_fd, "t_gr", gr_info);
        send_cmd_txt(tty_fd, "t_it", it_info);
        send_cmd_txt(tty_fd, "t_sp", sp_info);
        send_cmd_txt(tty_fd, "t_kr", kr_info);
        send_cmd_txt(tty_fd, "t_pr", pr_info);
        send_cmd_txt(tty_fd, "t_ar", ar_info);
        send_cmd_txt(tty_fd, "t_tr", tr_info);
        send_cmd_txt(tty_fd, "t_hb", hb_info);
        mksini_free();
    }
}

void online_update() {
    page_to(TJC_PAGE_UPDATING);
    send_cmd_vis(tty_fd, "j0", "1");
    send_cmd_vis(tty_fd, "t1", "1");
    pthread_t recevice_progress_pthread;
    pthread_create(&recevice_progress_pthread, NULL, recevice_progress_handle, NULL);
    system("rm /home/mks/gcode_files/.cache/*\n");
    system("python3 /root/auto_update/download_update.py\n");
    // page_to(TJC_PAGE_UPDATE_FINISH);
    system("sync\n");
    system("systemctl restart makerbase-client\n");
}

// CLL 创建线程用于接收在线更新进度
void *recevice_progress_handle(void *arg) {
    while(1) {
        if (current_page_id == TJC_PAGE_UPDATING) {
            progressini_load();
            int update_progress = mksini_getint("progress", "value", update_progress);
            std::string progress_name = mksini_getstring("filename", "name", progress_name);
            mksini_free();
            if (progress_name.find("Installing") == -1) {
                send_cmd_txt(tty_fd, "t2", progress_name);
                send_cmd_val(tty_fd, "j0", std::to_string(update_progress));
                send_cmd_txt(tty_fd, "t1", std::to_string(update_progress) + "%");
            } else {
                page_to(TJC_PAGE_INSTALLING);
                break;
            }
        }
    }
    return NULL;
}

void refresh_page_auto_unload() {
    send_cmd_txt(tty_fd, "t4", "(" + std::to_string(printer_extruder_temperature) + "/" + std::to_string(printer_extruder_target) + "℃)");
    if (step_1 == true) {
        step_1 = false;
        send_cmd_vis(tty_fd, "gm1", "0");
        send_cmd_vis(tty_fd, "gm2", "1");
        send_cmd_picc(tty_fd, "q0", "231");
        send_cmd_pco(tty_fd, "t1", "65535");
        send_cmd_pco(tty_fd, "t0", "38066");
    }
    if (step_2 == true) {
        step_2 = false;
        send_cmd_vis(tty_fd, "gm2", "0");
        send_cmd_picc(tty_fd, "q0", "232");
        send_cmd_pco(tty_fd, "t2", "65535");
        send_cmd_pco(tty_fd, "t1", "38066");
        send_cmd_vis(tty_fd, "b0", "1");
        send_cmd_vis(tty_fd, "b1", "1");
    }
}

int get_mks_ethernet() {
    mksini_load();
    mks_ethernet = mksini_getboolean("mks_ethernet", "enable", 0);
    mksini_free();
    return mks_ethernet;
}

void set_mks_ethernet(int target) {
    std::cout << "设置以太网:" << target << std::endl;
    mksini_load();
    mksini_set("mks_ethernet", "enable", std::to_string(target));
    mksini_save();
    mksini_free();
    mks_ethernet = target;
}

std::string hex_to_utf8(const std::string& hex) {
    std::ostringstream utf8;
    size_t i = 0;
    while (i < hex.size()) {
        if (i + 3 < hex.size() && hex[i] == '\\' && hex[i + 1] == 'x') {
            std::string hex_byte = hex.substr(i + 2, 2);
            int value;
            std::istringstream(hex_byte) >> std::hex >> value;
            utf8 << static_cast<char>(value);
            i += 4;
        } else {
            utf8 << hex[i++];
        }
    }
    return utf8.str();
}

void check_print_interrupted() {
    std::ifstream infile("/home/mks/klipper_config/saved_variables.cfg");
    if (!infile) {
        std::cerr << "无法打开文件 " << "/home/mks/klipper_config/saved_variables.cfg" << std::endl;
        return;
    }
    std::stringstream buffer;
    buffer << infile.rdbuf();
    std::string printer_variables = buffer.str();
    infile.close();
    std::string print_interrupted_status = printer_variables.substr(printer_variables.find("was_interrupted =") + 18, 5);
    if (print_interrupted_status != "False") {
        ep->Send(json_run_a_gcode("DETECT_INTERRUPTION\n"));
        jump_to_resume_print = true;
    }
}