#ifndef KLIPPY_REST_H
#define KLIPPY_REST_H

#include <iostream>

#include "./HTTPRequest.hpp"


std::string get_server_files_list(std::string ip, std::string port);
std::string get_server_files_directory(std::string ip, std::string port);

std::string server_files_metadata(std::string ip, std::string port, std::string filename);
/* 打印机对象参数获取 */
std::string printer_objects_query(std::string ip, std::string port, std::string parameter);
/* 打印管理 */
// 打印文件
std::string printer_print_start(std::string ip, std::string port, std::string filename);
// 暂停打印
std::string printer_print_pause(std::string ip, std::string port);
// 继续打印
std::string printer_print_resume(std::string ip, std::string port);
// 取消打印
std::string printer_print_cancel(std::string ip, std::string port);
/* 打印机状态 */


std::string get_server_info(std::string ip, std::string port);
std::string get_oneshot_token(std::string ip, std::string port);
std::string get_printer_info(std::string ip, std::string port);

/* File Operations */
std::string delete_file_delete(std::string ip, std::string port, std::string filepath);

std::string get_thumbnail_stream(std::string ip, std::string port, std::string thumbnail);
std::string send_request(std::string ip, std::string port, std::string method, std::string request_type);



// std::string send_post_request(std::string ip, std::string port, std::string method);

#endif
