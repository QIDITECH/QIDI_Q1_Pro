#ifndef EVENT_H
#define EVENT_H
#include <vector>

void refresh_page_show();
void refresh_page_open_filament_video_2();
void refresh_page_open_leveling();
void refresh_page_wifi_keyboard();
void refresh_page_wifi_list();
void refresh_page_wifi_list_2();
void refresh_page_syntony_finish();
void refresh_page_auto_level();
void refresh_page_stopping();
void refresh_page_syntony_move();
void refresh_page_print_filament();
void refresh_page_auto_finish();
void refresh_page_auto_moving();
void refresh_page_move();
void refresh_page_offset(float intern_zoffset);
void refresh_page_printing_zoffset();
void refresh_page_printing();
void clear_page_printing_arg();
void refresh_page_preview();
void refresh_page_main();
void refresh_page_files_list();
void refresh_page_files(int pages);
void sub_object_status();
void get_object_status();
void get_file_estimated_time(std::string filename);
void delete_file(std::string filepath);
void start_printing(std::string filepath);
void set_target(std::string heater, int target);
void set_extruder_target(int target);
void set_heater_bed_target(int target);
void set_hot_target(int target);
void set_fan(int speed);
void set_fan0(int speed);
void set_fan2(int speed);
void set_fan3(int speed);
void set_target(std::string heater, int target);
void set_intern_zoffset(float offset);
void set_zoffset(bool positive);
void set_move_dist(float dist);
void set_printer_speed(int speed);
void set_printer_flow(int rate);
std::string show_time(int seconds);
void move_home();
void move_x_decrease();
void move_x_increase();
void move_y_decrease();
void move_y_increase();
void move_z_decrease();
void move_z_increase();
bool get_filament_detected();
bool get_filament_detected_enable();
bool get_print_pause_resume();
void set_print_pause_resume();
void set_print_pause();
void set_print_resume();
void cancel_print();
void sdcard_reset_file();
void set_auto_level_dist(float dist);
void open_calibrate_start();
void start_auto_level();
void start_auto_level_dist(bool positive);
void set_filament_extruder_target(bool positive);
void set_print_filament_dist(float dist);
void start_retract();
void start_extrude();
std::string get_ip(std::string net);
void move_home_tips();
void filament_tips();
void move_tips();
void reset_klipper();
void reset_firmware();
void finish_print();
void set_filament_sensor();
void motors_off();
void beep_on_off();
void led_on_off();
void shutdown_mcu();
void firmware_reset();
void go_to_page_power_off();
int get_mks_led_status();
void set_mks_led_status();
int get_mks_beep_status();
void set_mks_beep_status();
void get_mks_language_status();
void set_mks_language_status();
void get_mks_extruder_target();
void set_mks_extruder_target(int target);
void get_mks_heater_bed_target();
void set_mks_heater_bed_target(int target);
void get_mks_hot_target();
void set_mks_hot_target(int target);
void filament_extruder_target();
void filament_heater_bed_target();
void filament_hot_target();
void filament_fan0();
void filament_fan2();
void filament_fan3();
void go_to_reset();
void go_to_network();
void scan_ssid_and_show();
void get_wifi_list_ssid(int index);
void set_print_filament_target();
void complete_print();
void back_to_main();
void go_to_syntony_move();
void print_ssid_psk(char *psk);
void clear_page_preview();
void set_mks_babystep(std::string value);
void get_mks_babystep();
void clear_cp0_image();
void printer_set_babystep();
int get_mks_fila_status();
void set_mks_fila_status();
void init_mks_status();
int detect_disk();
void set_printing_shutdown();
void mks_get_version();
void wifi_save_config();
void disable_page_about_successed();
void finish_tjc_update();
void filament_load();
void filament_unload();
int get_cal_printed_time(int print_time);

int get_mks_total_printed_time();
void set_mks_total_printed_time(int printed_time);

void get_total_time();

void do_not_x_clear();

void do_x_clear();

void level_mode_printing_set_target();

void level_mode_printing_print_file();

void update_finished_tips();

bool get_mks_oobe_enabled();

void set_mks_oobe_enabled(bool enable);

void move_motors_off();

void open_more_level_finish();

void open_move_tip();

void open_set_print_filament_target();

void open_start_extrude();

void close_mcu_port();

void oobe_set_intern_zoffset(float offset);

void oobe_set_zoffset(bool positive);

void refresh_page_zoffset();

void refresh_page_auto_heaterbed();

void set_auto_level_heater_bed_target(bool positive);

void detect_error();

void clear_previous_data();

void print_start(); 

void open_heater_bed_up(); 

void refresh_page_open_heaterbed(); 

void refresh_page_open_leveling(); 

void bed_leveling_switch(bool positive); 

void refresh_page_filament_pop();

void save_current_zoffset();

void check_filament_type();

void refresh_page_preview_pop();

//4.4.2 CLL mates适配霍尔耗材宽度检测器
void check_filament_width();

//4.4.2 CLL 文件列表新增本地、U盘切换按钮
void refresh_page_files_list_2();

//4.4.3 CLL 修改更新按钮始终显示
void go_to_update();

//4.4.3 CLL 新增恢复出厂设置按钮
void restore_config();

void refresh_page_open_calibrate();

void refresh_page_filament_set_fan();

void go_to_adjust();
void go_to_setting();

void refresh_page_common_setting();

void print_log();

std::string replaceCharacters(const std::string& path, const std::string& searchChars, const std::string& replacement);

void refresh_page_bed_moving();
void bed_calibrate();
void bed_adjust(bool status);

void go_to_file_list();

void send_gcode(std::string command);

void refresh_files_list_picture(std::string path, int pixel, int i);

void refresh_files_list_picture_2(std::string path, int size, int i);

void refresh_page_filament();

void get_mks_connection_method();
void set_mks_connection_method(int target);
void refresh_page_show_qr();
void go_to_showqr();

struct Server_config
{
    std::string address;
    std::string name;
};
void get_mks_selected_server();
void go_to_server_set(int n);
void updateServerConfig(std::vector<std::string> &lines, const Server_config &config);
void update_server(int choice);
void refresh_page_server_set();

void local_update();
std::string run_python_code(const char* cmd);
void check_online_version();
void online_update();
void *recevice_progress_handle(void *arg);

void refresh_page_auto_unload();

int get_mks_ethernet();
void set_mks_ethernet(int target);

std::string hex_to_utf8(const std::string& hex);

void check_print_interrupted();

#endif
