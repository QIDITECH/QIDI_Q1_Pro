#include "../include/MoonrakerAPI.h"
#include "../include/MakerbaseCommand.h"
#include "nlohmann/json.hpp"

extern int response_type_id;

nlohmann::json string2json(std::string response) {
    return nlohmann::json::parse(response);
}

inline std::string create_json_without_params(int cmd) {
    response_type_id = cmd;
    nlohmann::json api;
    api["jsonrpc"] = "2.0";
    api["method"] = method2command[cmd];
    api["id"] = method2id[cmd];
    return api.dump();
}

inline std::string create_json(int cmd, nlohmann::json params) {
    response_type_id = cmd;
    nlohmann::json api;
    api["jsonrpc"] = "2.0";
    api["method"] = method2command[cmd];
    api["params"] = params;
    api["id"] = method2id[cmd];
    return api.dump();
}

std::string json_get_klippy_host_information() {
    return create_json_without_params(0x03);
}

std::string json_emergency_stop() {
    // return create_json(0x04, no_params, 4564);
    return create_json_without_params(0x04);
}

std::string json_host_restart() {
    return create_json_without_params(0x05);
}

std::string json_firmware_restart() {
    return create_json_without_params(0x06);
}

std::string json_list_available_printer_objects() {
    return create_json_without_params(0x11);
}

std::string json_query_printer_object_status(nlohmann::json objects) {
    nlohmann::json params;
    params["objects"] = objects;
    return create_json(0x12, params);
}

std::string json_subscribe_to_printer_object_status(nlohmann::json objects) {
    nlohmann::json params;
    params["objects"] = objects;
    return create_json(0x13, params);
}

std::string json_query_endstops() {
    return create_json_without_params(0x14);
}

std::string json_query_server_info() {
    return create_json_without_params(0x15);
}

std::string json_get_server_configuration() {
    return create_json_without_params(0x16);
}

std::string json_request_cached_temperature_data() {
    return create_json_without_params(0x17);
}

std::string json_request_cached_gcode_responses(int count) {
    nlohmann::json params;
    params["count"] = count;
    return create_json(0x18, params);
}

std::string json_restart_server() {
    return create_json_without_params(0x19);
}

/* GCode APIs */
std::string json_run_a_gcode(std::string script) {
    nlohmann::json params;
    params["script"] = script;
    return create_json(0x21, params);
}

std::string json_get_gcode_help() {
    return create_json_without_params(0x22);
}

/* Print Management */
std::string json_print_a_file(std::string filename) {
    nlohmann::json params;
    params["filename"] = filename;
    return create_json(0x31, params);
}

std::string json_pause_a_print() {
    return create_json_without_params(0x32);
}

std::string json_resume_a_print() {
    return create_json_without_params(0x33);
}

std::string json_cancel_a_print() {
    return create_json_without_params(0x34);
}

/* Machine Commands */
std::string json_get_system_info() {
    return create_json_without_params(0x41);
}

std::string json_shutdown_the_operating_system(){
    return create_json_without_params(0x42);
}

std::string json_reboot_the_operating_system() {
    return create_json_without_params(0x43);
}

std::string json_restart_a_system_service(std::string name) {
    nlohmann::json params;
    params["service"] = name;
    return create_json(0x44, params);
}

std::string json_stop_a_system_service(std::string name) {
    nlohmann::json params;
    params["service"] = name;
    return create_json(0x45, params);
}

std::string json_start_a_system_service(std::string name) {
    nlohmann::json params;
    params["service"] = name;
    return create_json(0x46, params);
}

std::string json_get_moonraker_process_stats() {
    return create_json_without_params(0x47);
}

/* File Operations */
std::string json_list_available_files() {
    nlohmann::json params;
    // params["root"] = root_folder;
    // return create_json(0x51, params);
    return create_json_without_params(0x51);
}

std::string json_get_gcode_metadata(std::string filename) {
    nlohmann::json params;
    params["filename"] = filename;
    return create_json(0x52, params);
}

std::string json_get_directory_information(bool extended) {
    nlohmann::json params;
    // params["path"] = path;
    params["extended"] = extended;
    return create_json(0x53, params);
}

std::string json_create_directory(std::string path) {
    nlohmann::json params;
    params["path"] = path;
    return create_json(0x54, params);
}

std::string json_delete_directory(std::string path, bool force) {
    nlohmann::json params;
    params["path"] = path;
    params["force"] = force;
    return create_json(0x55, params);
}

std::string json_move_a_file_or_directory(std::string source, std::string dest) {
    nlohmann::json params;
    params["source"] = source;
    params["dest"] = dest;
    return create_json(0x56, params);
}

std::string json_copy_a_file_or_directory(std::string source, std::string dest) {
    nlohmann::json params;
    params["source"] = source;
    params["dest"] = dest;
    return create_json(0x57, params);
}

// std::string json_file_download() {
//     return "";
// }

// std::string json_file_upload() {
//     return "";
// }

std::string json_file_delete(std::string path) {
    // path: {root}/{filename}
    nlohmann::json params;
    params["path"] = path;
    return create_json(0x5a, params);
}

// std::string json_download_klippy_log() {
//     return "";
// }

// std::string json_download_moonraker_log() {
//     return "";
// }

/* Authorization */
// std::string json_login_user() {
//     return "";
// }

// std::string json_logout_current_user() {
//     return "";
// }

// std::string json_get_current_user() {
//     return "";
// }

// std::string json_create_user() {
//     return "";
// }

// std::string json_delete_user() {
//     return "";
// }

// std::string json_list_available_users() {
//     return "";
// }

// std::string json_reset_user_password() {
//     return "";
// }

// std::string json_generate_a_oneshot_token() {
//     return "";
// }

// std::string json_retrieve_information_about_authorization_endpoints() {
//     return "";
// }

/* Database APIs */
std::string json_list_namespaces() {
    return create_json_without_params(0x71);
}

std::string json_get_database_item(std::string nsp, std::string key) {
    nlohmann::json params;
    params["namespace"] = nsp;
    params["key"] = key;
    return create_json(0x72, params);
}

std::string json_add_database_item(std::string nsp, std::string key, int value) {
    nlohmann::json params;
    params["namespace"] = nsp;
    params["key"] = key;
    params["value"] = value;
    return create_json(0x73, params);
}

std::string json_delete_database_item(std::string nsp, std::string key) {
    nlohmann::json params;
    params["namespace"] = nsp;
    params["key"] = key;
    return create_json(0x74, params);
}

/* Job Queue APIs */
std::string json_retrieve_the_job_queue_status() {
    return create_json_without_params(0x81);
}

std::string json_enqueue_a_job(std::vector<std::string> filenames) {
    nlohmann::json params;
    params["filenames"] = filenames;
    return create_json(0x82, params);
}

std::string json_remove_a_job(std::vector<std::string> job_ids) {
    nlohmann::json params;
    params["job_ids"] = job_ids;
    return create_json(0x83, params);
}

std::string json_pause_the_job_queue() {
    return create_json_without_params(0x84);
}

std::string json_start_the_job_queue() {
    return create_json_without_params(0x85);
}

/* Announcement APIs */
std::string json_list_announcements(bool incluede_dismissed) {
    nlohmann::json params;
    params["include_dismissed"] = incluede_dismissed;
    return create_json(0x86, params);
}

std::string json_update_announcements() {
    return create_json_without_params(0x87);
}

std::string json_dismiss_an_announcement(std::string entry_id, int wake_time) {
    nlohmann::json params;
    params["entry_id"] = entry_id;
    params["wake_time"] = wake_time;
    return create_json(0x88, params);
}

std::string json_list_announcement_feeds() {
    return create_json_without_params(0x89);
}

std::string json_add_an_announcement_feed(std::string name) {
    nlohmann::json params;
    params["name"] = name;
    return create_json(0x8a, params);
}

std::string json_remove_an_announcement_feed(std::string name) {
    nlohmann::json params;
    params["name"] = name;
    return create_json(0x8b, params);
}

/* Webcam APIs */
std::string json_list_webcams() {
    return create_json_without_params(0x101);
}

std::string json_get_webcam_information(std::string name) {
    nlohmann::json params;
    params["name"] = name;
    return create_json(0x102, params);
}

std::string json_add_or_update_a_webcam(std::string name) {
    nlohmann::json params;
    params["name"] = name;
    params["snapshot_url"] = "/webcam?action=snapshot";
    params["stream_url"] = "/webcam?action=stream";
    return create_json(0x103, params);
}

std::string json_delete_a_webcam(std::string name) {
    nlohmann::json params;
    params["name"] = name;
    return create_json(0x104, params);
}

std::string json_test_a_webcam(std::string name) {
    nlohmann::json params;
    params["name"] = name;
    return create_json(0x105, params);
}

/* Update Manager APIs */
std::string json_get_update_status(bool refresh) {
    nlohmann::json params;
    params["refresh"]  = refresh;
    return create_json(0x91, params);
}

/* Perform a full update */
std::string json_perform_a_full_update() {
    return create_json_without_params(0x90);
}

std::string json_update_moonraker() {
    return create_json_without_params(0x92);
}

std::string json_update_klipper() {
    return create_json_without_params(0x93);
}

std::string json_update_client(std::string name) {
    nlohmann::json params;
    params["name"] = name;
    return create_json(0x94, params);
}

std::string json_update_system_packages() {
    return create_json_without_params(0x95);
}

std::string json_recover_a_corrupt_repo(std::string name, bool hard) {
    nlohmann::json params;
    params["name"] = name;
    params["hard"] = hard;
    return create_json(0x96, params);
}

/* Power APIs */
std::string json_get_device_list() {
    return create_json_without_params(0xa1);
}

std::string json_get_device_status(std::string device) {
    nlohmann::json params;
    params["device"] = device;
    return create_json(0xa2, params);
}

std::string json_get_device_state(std::string device, std::string action) {
    nlohmann::json params;
    params["device"] = device;
    params["action"] = action;
    return create_json(0xa3, params);
}
// std::string json_get_batch_device_status(std::string dev_x[]);
// std::string json_batch_power_on_devices(std::string dev_x[]);
// std::string json_batch_power_off_devices(std::string dev_x[]);

/* WLED APIs */
std::string json_get_strips() {
    return create_json_without_params(0xb1);
}

std::string json_get_strip_status() {
    nlohmann::json params;
    params["lights"];
    params["desk"];
    return create_json(0xb2, params);
}

std::string json_turn_strip_on() {
    nlohmann::json params;
    params["lights"];
    params["desk"];
    return create_json(0xb3, params);
}

std::string json_turn_strip_off() {
    nlohmann::json params;
    params["lights"];
    params["desk"];
    return create_json(0xb4, params);
}

std::string json_toggle_strip_on_off_state() {
    nlohmann::json params;
    params["lights"];
    params["desk"];
    return create_json(0xb5, params);
}

std::string json_get_job_totals() {
    return create_json_without_params(0xd2);
}

std::string json_reset_totals() {
    return create_json_without_params(0xd3);
}
