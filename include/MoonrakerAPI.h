/**
 * @file MoonrakerAPI.h
 * @author Kenneth Lin (kenneth.lin.gd.cn@gmail.com)
 * @brief Create APIs by reading moonraker.readthedocs.io
 * @version 0.1
 * @date 2022-07-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef MOONRAKERAPI_H
#define MOONRAKERAPI_H

#include <iostream>
#include "nlohmann/json.hpp"

nlohmann::json string2json(std::string response);

// API definition
// 👇 makerbase client 发起连接的json消息
#define STRING_IDENTIFY_CONNECTION "{\"jsonrpc\":\"2.0\",\"method\":\"server.connection.identify\",\"params\":{\"client_name\":\"makerbase-client\",\"version\":\"0.0.1\",\"type\":\"web\",\"url\":\"http://makerbase.com/test\"},\"id\":4656}"
#define STRING_GET_KLIPPY_HOST_INFORMATION "{\"jsonrpc\":\"2.0\",\"method\":\"printer.info\",\"id\":5445}"

// Function for creating json
inline std::string create_json_without_params(int cmd);
inline std::string create_json(int cmd, nlohmann::json params);

// Printer Administration
// std::string json_identify_connection(std::string client_name, std::string json_version, std::string type, std::string url);
std::string json_get_websocket_id();
std::string json_get_klippy_host_information();
std::string json_emergency_stop();
std::string json_host_restart();
std::string json_firmware_restart();

// Print Status
std::string json_list_available_printer_objects();
std::string json_query_printer_object_status(nlohmann::json objects);
std::string json_subscribe_to_printer_object_status(nlohmann::json objects);
std::string json_query_endstops();
std::string json_query_server_info();
std::string json_get_server_configuration();
std::string json_request_cached_temperature_data();
std::string json_request_cached_gcode_responses();
std::string json_restart_server();

// GCode APIs
std::string json_run_a_gcode(std::string script);       // script 参数调用 KlippyGcodes.h 里面的相关函数执行
std::string json_get_gcode_help();

// Print Management
std::string json_print_a_file(std::string filename);
std::string json_pause_a_print();
std::string json_resume_a_print();
std::string json_cancel_a_print();

// Machine Commands
std::string json_get_system_info();
std::string json_shutdown_the_operating_system();
std::string json_reboot_the_operating_system();
std::string json_restart_a_system_service();
std::string json_stop_a_system_service();
std::string json_start_a_system_service();
std::string json_get_moonraker_process_stats();

// File Operations
std::string json_list_available_files();
std::string json_get_gcode_metadata(std::string path);
std::string json_get_directory_information(bool extended);
std::string json_create_directory(std::string path);
std::string json_delete_directory();
std::string json_move_a_file_or_directory();
std::string json_copy_a_file_or_directory();
std::string json_file_download();
std::string json_file_upload();
std::string json_file_delete(std::string path);
// std::string json_download_klippy_log();
// std::string json_download_moonraker_log();

// Authorization
// std::string json_login_user();
// std::string json_logout_current_user();
// std::string json_get_current_user();
// std::string json_create_user();
// std::string json_delete_user();
// std::string json_list_available_users();
// std::string json_reset_user_password();
// std::string json_generate_a_oneshot_token();
// std::string json_retrieve_information_about_authorization_endpoints();
// std::string json_get_the_current_api_key();
// std::string json_generate_a_new_api_key();

// Database APIs
std::string json_list_namespaces();
std::string json_get_database_item(std::string nsp, std::string key);
std::string json_add_database_item(std::string nsp, std::string key, int value);
std::string json_delete_database_item(std::string nsp, std::string key);

// Job Queue APIs
std::string json_retrieve_the_job_queue_status();
std::string json_enqueue_a_job(std::vector<std::string> filenames);       // 需要传递字符串数组作为参数，后面再进行实现
std::string json_remove_a_job(std::vector<std::string> job_ids);
std::string json_pause_the_job_queue();
std::string json_start_the_job_queue();

// Announcement APIs
std::string json_list_announcements(bool incluede_dismissed);
std::string json_update_announcements();
std::string json_dismiss_an_announcement(std::string entry_id, int wake_time);
std::string json_list_announcement_feeds();
std::string json_add_an_announcement_feed(std::string name);
std::string json_remove_an_announcement_feed(std::string name);

// Webcam APIs
std::string json_list_webcams();
std::string json_get_webcam_information(std::string name);
std::string json_add_or_update_a_webcam(std::string name);
std::string json_delete_a_webcam(std::string name);
std::string json_test_a_webcam(std::string name);

// Update Manager APIs
std::string json_get_update_status(bool refresh);

// Perform a full update
std::string json_perform_a_full_update();
std::string json_update_moonraker();
std::string json_update_klipper();
std::string json_update_client(std::string name);
std::string json_update_system_packages();
std::string json_recover_a_corrupt_repo(std::string name, bool hard);

// Power APIs
std::string json_get_device_list();
std::string json_get_device_status(std::string device);
std::string json_get_device_state(std::string device, std::string action);
std::string json_get_batch_device_status(std::string dev_x[]);
std::string json_batch_power_on_devices(std::string dev_x[]);       // 需要根据api做特殊处理
std::string json_batch_power_off_devices(std::string dev_x[]);      // 需要根据api做特殊处理

// WLED APIs
std::string json_get_strips();
std::string json_get_strip_status();
std::string json_turn_strip_on();
std::string json_turn_strip_off();
std::string json_toggle_strip_on_off_state();
// std::string control_individual_strip_state();       // 这个比较特殊，后面再研究一下

// OctoPrint API emulation

// History APIs
std::string json_get_job_list(int limit, int start, float since, float before, std::string order = "desc");
std::string json_get_job_totals();
std::string json_reset_totals();
std::string json_get_a_single_job(std::string uid);
std::string json_delete_job(std::string uid);

// MQTT APIs
std::string json_publish_a_topic(std::string topic, std::string payload, int qos, bool retain, int timeout);
std::string json_subscribe_to_a_topic(std::string topic, int qos, int timeout);

// Extension APIs
// std::string json_list_extensions();
// std::string json_call_an_extension_method();
std::string json_send_an_agent_event();

// Websocket notifications


#endif
