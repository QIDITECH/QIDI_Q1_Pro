#include "nlohmann/json.hpp"

#include "../include/MakerbaseClient.h"
#include "../include/MakerbaseCommand.h"
#include "../include/MoonrakerAPI.h"
#include "../include/mks_log.h"
#include "../include/MakerbaseParseMessage.h"

#include "../include/event.h"
#include "../include/mks_error.h"
#include "../include/mks_printer.h"
#include "../include/mks_file.h"
#include "../include/mks_gcode.h"
// #include "../include/systeminfo.h"
// #include "../include/printer.h"
// #include "../include/file.h"
// #include "../include/job.h"
// #include "../include/tjc_event.h"
#include "../include/ui.h"

extern MakerbaseClient *ep;

bool get_status_flag = false;

std::string message;
bool is_get_message = false;
int response_type_id;
nlohmann::json response;
nlohmann::json res;

extern bool filelist_changed;

extern bool all_level_saving;

//4.4.3 CLL 修改网页打印信息订阅
extern bool jump_to_print;
extern nlohmann::json output_metadata;
extern int current_page_id;

void *json_parse(void *arg) {
    while (1) {
        if (is_get_message == true) {
            try
            {
                response = string2json(message);
                res = response;
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
            if (res["id"] != nlohmann::detail::value_t::null) {
                std::cout << response["id"] << std::endl;
                // std::cout << response << std::endl;
                int id = response["id"];
                switch (id)
                {
                case 3545:
                    std::cout << response << std::endl;
                    parse_file_estimated_time(response);
                    break;

                case 4654:
                    std::cout << response << std::endl;
                    if (response["result"] == "ok") {
                        std::cout << response << std::endl;
                    } else if (response["result"] != nlohmann::detail::value_t::null) {
                        if (response["result"]["status"] != nlohmann::detail::value_t::null) {
                            std::cout << response["result"]["status"] << std::endl;
                            parse_subscribe_objects_status(response["result"]["status"]);
                        }
                    }
                    break;

                case 5445:
                    std::cout << response << std::endl;
                    if (response["result"] != nlohmann::detail::value_t::null) {
                        parse_printer_info(response["result"]);
                    }
                    break;

                // 解析一下总的时间
                case 5656:
                    std::cout << response << std::endl;
                    if (response["result"] != nlohmann::detail::value_t::null) {
                        // parse_server_history_totals(response["result"]);
                        if (response["result"]["job_totals"] != nlohmann::detail::value_t::null) {
                            parse_server_history_totals(response["result"]["job_totals"]);
                        }
                    }
                    break;
                
                default:
                    break;
                }
            }
            if (res["error"] != nlohmann::detail::value_t::null) {
                parse_error(response["error"]);
            } else {
                if (response["method"] != nlohmann::detail::value_t::null) {
                    // std::cout << response << std::endl;
                    std::string method = response["method"];
                    if (response["method"] == "notify_proc_stat_update") {
                        // MKSLOG_BLUE("%s", message.data());
                        /*
                        if (get_status_flag == false) {
                            // get_object_status();									// 主动获取必备的参数
                            sleep(1);
	                        sub_object_status();									// 订阅相关的参数
                            sleep(1);
                            get_status_flag = true;
                        }
                        */
                    } else if (method == "notify_gcode_response") {
                        MKSLOG_RED("%s", message.data());
                        parse_gcode_response(response["params"]);
                        MKSLOG_BLUE("Gcode 响应");
                    } else if (method == "notify_status_update") {
                        //std::cout << response["params"][0] << std::endl;
                        parse_subscribe_objects_status(response["params"][0]);
                        /*
                        if (get_status_flag == false) {
                            get_status_flag = true;
                        }
                        */
                    } else if (method == "notify_klippy_ready") {
                        std::cout << response << std::endl;
                        MKSLOG_BLUE("Klippy 准备好了");
                        // 在这里进行订阅操作
                        get_object_status();
                        sub_object_status();
                    } else if (method == "notify_klippy_shutdown") {
                        MKSLOG_BLUE("Klippy 关机");
                    } else if (method == "notify_klippy_disconnected") {
                        MKSLOG_BLUE("Klippy 已断开连接");
                        get_object_status();
                        sub_object_status();
                    } else if (method == "notify_filelist_changed") {
                        filelist_changed = true;
                        MKSLOG_BLUE("文件列表已更改");
                        
                        if (all_level_saving == false) {
                            all_level_saving = true;
                        }
                        
                    } else if (method == "notify_update_response") {
                        MKSLOG_BLUE("更新管理器响应");
                    } else if (method == "notify_update_refreshed") {
                        MKSLOG_BLUE("更新管理器已刷新");
                    } else if (method == "notify_cpu_throttled") {
                        MKSLOG_BLUE("Moonraker 进程统计更新");
                    } else if (method == "notify_history_changed") {
                        //MKSLOG_RED("%s", message.data());
                        //std::cout << response << std::endl;
                        //4.4.3 CLL 修改网页打印信息订阅
                        switch (current_page_id)
                        {
                        case TJC_PAGE_PRINTING:
                        case TJC_PAGE_PRINT_ZOFFSET:
                        case TJC_PAGE_PRINT_FILAMENT:
                        case TJC_PAGE_PRINTING_2:
                            break;
                        
                        default:
                            parse_file_estimated_time_send(response["params"][0]["job"]["metadata"]);
                            MKSLOG_BLUE("历史改变");
                            break;
                        }
                    } else if (method == "notify_user_created") {
                        MKSLOG_BLUE("授权用户创建");
                    } else if (method == "notify_user_deleted") {
                        MKSLOG_BLUE("已删除授权用户");
                    } else if (method == "notify_service_state_changed") {
                        // std::cout << response << std::endl;
                        // MKSLOG_BLUE("服务状态已更改");
                    } else if (method == "notify_job_queue_changed") {
                        MKSLOG_BLUE("作业队列已更改");
                    } else if (method == "notify_button_event") {
                        MKSLOG_BLUE("按钮事件");
                    } else if (method == "notify_announcement_update") {
                        MKSLOG_BLUE("公告更新事件");
                    } else if (method == "notify_announcement_dismissed") {
                        MKSLOG_BLUE("公告驳回事件");
                    } else if (method == "notify_announcement_wake") {
                        MKSLOG_BLUE("公告唤醒事件");
                    } else if (method == "notify_agent_event") {
                        MKSLOG_BLUE("代理事件");
                    } else if (method == "notify_power_changed") {
                        MKSLOG_BLUE("notify_power_changed");
                        // std::cout << response << std::endl;
                    }
                }
            }
            response.clear();
            is_get_message = false;
        }
        // usleep(300);
        usleep(50);
    }
    // pthread_exit(NULL);
}




