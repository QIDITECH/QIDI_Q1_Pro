#ifndef MKS_FILE_H
#define MKS_FILE_H

#include "./mks_log.h"
#include "nlohmann/json.hpp"

void parse_file_estimated_time(nlohmann::json response);
void parse_server_files_list(nlohmann::json result);
void parse_server_files_get_directory(nlohmann::json result);
void parse_server_files_metadata(nlohmann::json result);
void parse_create_directory(nlohmann::json result);
void parse_delete_directory(nlohmann::json result);
void parse_move_a_file_or_directory(nlohmann::json result);
void parse_copy_a_file_or_directory(nlohmann::json result);
void parse_file_delete(nlohmann::json result);
void get_page_files_filelist(std::string current_dir);
void set_page_files_show_list(int pages);
void get_sub_dir_files_list(int button);
void get_parenet_dir_files_list();
// void get_file_estimated_time(std::string filename, int *estimated_time);
void parse_file_estimated_time_send(nlohmann::json response);


std::string getParentDirectory(const std::string& path);
int output_imgdata(std::string thumbpath, int size);

int output_jpg(std::string thumbpath, int size);

std::string extractFileName(const std::string& filePath);

#endif
