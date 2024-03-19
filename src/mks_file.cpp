#include <iostream>
#include <stack>
#include <set>

#include <fstream>
#include <string>
#include <sstream>

#include "../include/send_msg.h"
#include "../include/mks_file.h"
#include "../include/KlippyRest.h"
#include "../include/event.h"
#include "../include/ui.h"
#include "../include/mks_preview.h"

// List available files

// Get gcode metadata
std::string file_filename;                      // 文件名
std::string file_current_filename;              // 当前文件名
std::string file_previous_filename;             // 先前文件名

float file_filament_total;                      // 用料
float file_estimated_time;                      // 文件预估时间

// Directory
std::stack<std::string> file_path_stack;        // 路径栈
std::string file_root_path;                     // Gcodes根目录
std::string file_previous_path;                 // 之前的路径
std::string file_current_path;                  // 文件当前所在的路径
std::string file_print_path;                    // 要打印的文件路径

// 文件管理
std::set<std::string> server_files_list;                // gcodes文件列表
std::set<std::string> server_files_get_directroy;       // 路径下面的目录列表

std::set<std::string> newfiles;
std::set<std::string> deletedfiles;
std::set<std::string> updatedfiles;

// 解析文件内容相关

std::set<std::string> page_files_dirname_list;
std::set<std::string> page_files_filename_list;
std::set<std::string> page_files_dirname_filename_list;

std::string page_files_last_printing_files_dir;

bool page_files_is_last_print_files_dir;

// 文件列表相关处理变量
bool filelist_changed = false;

int page_files_pages;
int page_files_current_pages;
int page_files_folder_layers;

std::string page_files_list_name[9] = {"", "", "", "", "", "", "", "", ""};                                     // 文件列表显示文件名称
std::string page_files_list_show_name[9] = {"", "", "", "", "", "", "", "", ""};                                // 文件列表名称
std::string page_files_list_show_type[9] = {"[n]", "[n]", "[n]", "[n]", "[n]", "[n]", "[n]", "[n]", "[n]"};        // 文件类型: [f]或者[d]，或者[n]

bool page_files_refresh_status = false;

std::stack<std::string> page_files_path_stack;          // 路径栈
std::string page_files_root_path;                       // Klippy根目录
std::string page_files_previous_path;                   // 之前的路径
std::string page_files_path;                            // 文件所在路径
std::string page_files_print_files_path;                // 要打印的文件路径

int file_metadata_estimated_time = 0;
std::string file_metadata_filename;
float file_metadata_filament_total = 0.0;
int file_metadata_object_height = 0;
std::string file_metadata_filament_name;
std::string file_metadata_filament_type;

float file_metadata_filament_weight_total = 0.0;
std::string file_metadata_gimage;
std::string file_metadata_simage;
bool mks_file_parse_finished = false;
extern bool show_preview_complete;

extern int current_page_id;

//4.4.2 CLL 文件列表页面新增本地、U盘按钮
extern std::string file_mode;

extern bool jump_to_print;

std::string thumbnail_relative_path;
std::string thumbnail_path;

bool cache_clicked = false;

void parse_file_estimated_time(nlohmann::json response) {
    if (response["result"]["estimated_time"] != nlohmann::detail::value_t::null) {
        file_metadata_estimated_time = (float)response["result"]["estimated_time"];
    }
    if (response["result"]["filename"] != nlohmann::detail::value_t::null) {
        file_metadata_filename = response["result"]["filename"];
    }
    if (response["result"]["filament_total"] != nlohmann::detail::value_t::null) {
        file_metadata_filament_total = (int)response["result"]["filament_total"];
        MKSLOG_RED("用料长度%f", file_metadata_filament_total);
    }
    if (response["result"]["object_height"] != nlohmann::detail::value_t::null) {
        file_metadata_object_height = (int)response["result"]["object_height"];
    }
    if (response["result"]["filament_name"] != nlohmann::detail::value_t::null) {
        file_metadata_filament_name = response["result"]["filament_name"];
    }
    if (response["result"]["filament_type"] != nlohmann::detail::value_t::null) {
        file_metadata_filament_type = response["result"]["filament_type"];
    }
    if (response["result"]["filament_weight_total"] != nlohmann::detail::value_t::null) {
        file_metadata_filament_weight_total = response["result"]["filament_weight_total"];
    }
    if (response["result"]["gimage"] != nlohmann::detail::value_t::null) {
        file_metadata_gimage = response["result"]["gimage"];
    }
    if (response["result"]["simage"] != nlohmann::detail::value_t::null) {
        file_metadata_simage = response["result"]["simage"];
    }
    if (response["result"]["thumbnails"] != nlohmann::detail::value_t::null) {
        for (int i = 0; response["result"]["thumbnails"][i] != nlohmann::detail::value_t::null; i++) {
            if (response["result"]["thumbnails"][i]["width"] == 168|| response["result"]["thumbnails"][i]["width"] == 300) {
                thumbnail_relative_path = response["result"]["thumbnails"][i]["relative_path"];
                if (getParentDirectory(file_metadata_filename) == "") {
                    thumbnail_path = thumbnail_relative_path;
                } else {
                    thumbnail_path = getParentDirectory(file_metadata_filename) + "/" + thumbnail_relative_path;
                }
                MKSLOG_RED("图片路径%s", thumbnail_path.c_str());
                break;
            }
            if (response["result"]["thumbnails"][i] == nlohmann::detail::value_t::null) {
                thumbnail_relative_path.clear();
                thumbnail_path.clear();
            }
        }
    }
    mks_file_parse_finished = true;
}

void parse_server_files_list(nlohmann::json result) {
    server_files_list.clear();
    std::string path_temp = "";
    for (int i = 0; i < result.size(); i++) {
        path_temp = result[i]["path"];
        server_files_list.insert(path_temp);
    }
}

void parse_server_files_get_directory(nlohmann::json result) {
    server_files_get_directroy.clear();
    server_files_list.clear();

    std::string dirname_temp = "";
    std::string filename_temp = "";

    for (int i = 0; i < result["dirs"].size(); i++) {
        std::cout << result["dirs"][i]["dirname"] << std::endl;
        dirname_temp = result["dirs"][i]["dirname"];
        server_files_list.insert(filename_temp);
    }
    for (int j = 0; j < result["files"].size(); j++) {
        std::cout << result["files"][j]["filename"] << std::endl;
        filename_temp = result["files"][j]["filename"];
        server_files_list.insert(filename_temp);
    }
}

void parse_server_files_metadata(nlohmann::json result) {

}

void parse_create_directory(nlohmann::json result) {
    std::cout << "path: " << result["item"]["path"] << std::endl;
    std::cout << "root: " << result["item"]["root"] << std::endl;
    std::cout << "action: " << result["action"] << std::endl;
}

void parse_delete_directory(nlohmann::json result) {
    std::cout << "path: " << result["item"]["path"] << std::endl;
    std::cout << "root: " << result["item"]["root"] << std::endl;
}

void parse_move_a_file_or_directory(nlohmann::json result) {
    std::cout << "item: " << result["item"] << std::endl;
    std::cout << "source_item" << result["source_item"] << std::endl;
    std::cout << "action: " << result["action"] << std::endl;
}

void parse_copy_a_file_or_directory(nlohmann::json result) {
    std::cout << "root: " << result["item"]["root"] << std::endl;
    std::cout << "path: " << result["item"]["path"] << std::endl;
    std::cout << "action: " << result["action"] << std::endl;
}

void parse_file_delete(nlohmann::json result) {
    std::cout << "path: " << result["item"]["path"] << std::endl;
    std::cout << "root: " << result["item"]["root"] << std::endl;
    std::cout << "action: " << result["action"] << std::endl;
}

// 获取文件列表信息
void get_page_files_filelist(std::string current_dir) {
    if (page_files_last_printing_files_dir == current_dir) {
        page_files_is_last_print_files_dir = true;
    } else {
        page_files_is_last_print_files_dir = false;
    }
    page_files_dirname_list.clear();
    page_files_filename_list.clear();
    page_files_dirname_filename_list.clear();
    nlohmann::json json_temp;
    nlohmann::json result;
    //4.4.2 CLL 新增在文件首页第一个显示打印过文件
    std::string temp_dirname = "";
    std::string temp_filename = "";
    std::string json_files_directory = send_request("localhost", "7125", "server/files/directory?path=gcodes//.cache", "GET");
    if (json_files_directory != "" && page_files_folder_layers == 0) {
        json_temp = nlohmann::json::parse(json_files_directory);
        result = json_temp["result"];
        if (0 < result["files"].size()) {
            std::cout << result["files"][0]["filename"] << std::endl;
            temp_filename = result["files"][0]["filename"];
            if (temp_filename.find(".gcode") != -1) {
                if (temp_filename.find(".") != 0) {
                    page_files_filename_list.insert(temp_filename);
                    auto it = page_files_filename_list.begin();
                    page_files_dirname_filename_list.insert("[c] " + *it);
                    page_files_filename_list.clear();
                }
            }
        }
    }
    json_files_directory = send_request("localhost", "7125", "server/files/directory?path=" + current_dir, "GET");
    if (json_files_directory != "") {
        json_temp = nlohmann::json::parse(json_files_directory);
        result = json_temp["result"];
    }

    for (int i = 0; i < result["dirs"].size(); i++) {
        temp_dirname = result["dirs"][i]["dirname"];
        if (temp_dirname != "System Volume Information" && temp_dirname.find(".") != 0 && temp_dirname != "sda1") {
            page_files_dirname_list.insert(temp_dirname);
        }
    }

    for (int j = 0; j < result["files"].size(); j++) {
        std::cout << result["files"][j]["filename"] << std::endl;
        temp_filename = result["files"][j]["filename"];
        if (temp_filename.find(".gcode") != -1) {
            if (temp_filename.find(".") != 0) {
                page_files_filename_list.insert(temp_filename);
            }
        }
    }

    for (auto it = page_files_dirname_list.begin(); it != page_files_dirname_list.end(); it++) {
        page_files_dirname_filename_list.insert("[d] " + *it);
    }

    for (auto it = page_files_filename_list.begin(); it != page_files_filename_list.end(); it++) {
        page_files_dirname_filename_list.insert("[f] " + *it);
    }

    if (0 == page_files_dirname_filename_list.size() % 4) { // CLL 此函数后的除数为一页所包含的文件数
        page_files_pages = page_files_dirname_filename_list.size() / 4 - 1;
        if (page_files_pages <= 0) {
            page_files_pages = 0;
        }
    } else {
        page_files_pages = page_files_dirname_filename_list.size() / 4;
    }
}

void set_page_files_show_list(int pages) {
    page_files_list_name[0] = "";
    page_files_list_name[1] = "";
    page_files_list_name[2] = "";
    page_files_list_name[3] = "";

    auto it = page_files_dirname_filename_list.begin();

    for (int i = 0; i < pages * 4; i++) {
        it++;
        std::cout << *it << std::endl;
    }
    for (int i = 0; i < 4; i++) {
        if (it != page_files_dirname_filename_list.end()) {
            page_files_list_name[i] = *it;
            it++;
        }
    }

    // 打印测试
    for (int j = 0; j < 4; j++) {
        if (page_files_list_name[j] != "") {
            page_files_list_show_type[j] = page_files_list_name[j].substr(0, 3);
            page_files_list_show_name[j] = page_files_list_name[j].substr(4);
        } else {
            page_files_list_show_type[j] = "[n]";
            page_files_list_show_name[j] = "";
        }
    }
}

void get_sub_dir_files_list(int button) {
    //4.4.2 CLL 在文件列表界面第一页显示打印过文件
    if ("[c]" == page_files_list_show_type[button]) {
        jump_to_print = false;
        show_preview_complete = false;
        cache_clicked = true;
        page_files_path_stack.push(page_files_path);
        page_files_print_files_path = page_files_path + "/.cache/" + page_files_list_show_name[button];
        page_files_folder_layers++;
        get_file_estimated_time(page_files_print_files_path.substr(1));
        page_to(TJC_PAGE_PREVIEW);
    } else if ("[d]" == page_files_list_show_type[button]) {
        page_files_path_stack.push(page_files_path);
        page_files_path = page_files_path + "/" + page_files_list_show_name[button];
        page_files_folder_layers++;
        page_to(TJC_PAGE_FILE_LIST);
        page_files_current_pages = 0;
        refresh_page_files(page_files_current_pages);
        refresh_page_files_list();
    } else if ("[f]" == page_files_list_show_type[button]) {
        jump_to_print = false;
        show_preview_complete = false;
        cache_clicked = false;
        page_files_path_stack.push(page_files_path);
        page_files_print_files_path = page_files_path + "/" + page_files_list_show_name[button];
        page_files_folder_layers++;
        MKSLOG("%s", page_files_print_files_path.substr(1).c_str());
        get_file_estimated_time(page_files_print_files_path.substr(1));
        page_to(TJC_PAGE_PREVIEW);
    }
}

void get_parenet_dir_files_list() {
    page_files_previous_path = page_files_path_stack.top();
    page_files_path = page_files_previous_path;
    if (current_page_id != TJC_PAGE_PREVIEW) {
        page_files_current_pages = 0;
    }
    //4.4.2 CLL 文件列表页面新增本地、U盘切换按钮
    if (detect_disk() == -1 && file_mode == "USB") {
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
    } else if (page_files_folder_layers > 0) {
        page_to(TJC_PAGE_FILE_LIST);
        page_files_folder_layers--;
        if (page_files_folder_layers == 0) {
            refresh_page_files(page_files_current_pages);
            refresh_page_files_list();
            page_files_previous_path = "";
            page_files_path = "";
        } else {
            refresh_page_files(page_files_current_pages);
            refresh_page_files_list();
            page_files_path_stack.pop();
        }
    } 
}

void parse_file_estimated_time_send(nlohmann::json response) {
    if (response["estimated_time"] != nlohmann::detail::value_t::null) {
        file_metadata_estimated_time = (float)response["estimated_time"];
    }
    if (response["filename"] != nlohmann::detail::value_t::null) {
        file_metadata_filename = response["filename"];
    }
    if (response["filament_total"] != nlohmann::detail::value_t::null) {
        file_metadata_filament_total = (int)response["filament_total"];
        MKSLOG_RED("用料长度%f", file_metadata_filament_total);
    }
    if (response["object_height"] != nlohmann::detail::value_t::null) {
        file_metadata_object_height = (int)response["object_height"];
    }
    if (response["filament_name"] != nlohmann::detail::value_t::null) {
        file_metadata_filament_name = response["filament_name"];
    }
    if (response["filament_type"] != nlohmann::detail::value_t::null) {
        file_metadata_filament_type = response["filament_type"];
    }

    if (response["filament_weight_total"] != nlohmann::detail::value_t::null) {
        file_metadata_filament_weight_total = response["filament_weight_total"];
    }
    if (response["gimage"] != nlohmann::detail::value_t::null) {
        file_metadata_gimage = response["gimage"];
		MKSLOG_RED("gimage");
    }
    if (response["simage"] != nlohmann::detail::value_t::null) {
        file_metadata_simage = response["simage"];
		MKSLOG_RED("simage");
    }
    if (response["result"]["thumbnails"] != nlohmann::detail::value_t::null) {
        if (response["result"]["thumbnails"].back() != nlohmann::detail::value_t::null) {
            thumbnail_relative_path = response["result"]["thumbnails"].back()["relative_path"];
            if (getParentDirectory(file_metadata_filename) == "") {
                thumbnail_path = thumbnail_relative_path;
            } else {
                thumbnail_path = getParentDirectory(file_metadata_filename) + "/" + thumbnail_relative_path;
            }
            MKSLOG_RED("图片路径%s", thumbnail_path.c_str());

        }
    } else {
        thumbnail_relative_path.clear();
        thumbnail_path.clear();
    }
    mks_file_parse_finished = true;
}

std::string getParentDirectory(const std::string& path) {
    size_t found = path.find_last_of("/\\");
    if (found != std::string::npos) {
        return path.substr(0, found);
    } else {
        return ""; // 如果没有找到分隔符，说明是根目录，返回空字符串或根据需求返回当前目录 "."
    }
}

int output_imgdata(std::string thumbpath, int size) {
    std::string path;
    if (size != 176) {
        path = "/home/mks/gcode_files/" + thumbpath;
    } else {
        path = thumbpath;
    }
    // std::string path = "/home/mks/printer_data/gcodes/" + thumbpath;
    std::string temp= "python3 /home/mks/gene4.py \"" + path + "\" /home/mks/tjc " + std::to_string(size);
    std::cout << temp << std::endl;
    const char* command = temp.c_str();
    system(command);
    return 0;
}

int output_jpg(std::string thumbpath, int size) {
    std::string path = "/home/mks/gcode_files/" + thumbpath;
    std::string path2 = path + ".jpg";
    // std::string temp= "python3 /home/mks/gene5.py \"" + path + "\" /home/mks/tjc.jpg " + std::to_string(size);
    std::string temp= "python3 /home/mks/gene5.py \"" + path + "\" \"" + path2 +"\" " + std::to_string(size);
    std::cout << temp << std::endl;
    const char* command = temp.c_str();
    if (access(path2.c_str(), F_OK) == -1) {
        system(command);
    }
    return 0;
}

// 自定义函数来提取文件名
std::string extractFileName(const std::string& filePath) {
    size_t lastSlash = filePath.find_last_of("/\\"); // 找到最后一个路径分隔符的位置
    if (lastSlash != std::string::npos) {
        return filePath.substr(lastSlash + 1); // 提取文件名部分
    }
    return filePath; // 如果没有路径分隔符，则返回整个路径
}

