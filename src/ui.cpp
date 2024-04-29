#include "../include/ui.h"
#include "../include/send_msg.h"
#include "../include/mks_log.h"
#include "../include/event.h"
#include "../include/mks_file.h"
#include "../include/mks_preview.h"
#include "../include/mks_update.h"
#include "../include/mks_wpa_cli.h"

#include "../include/MakerbaseNetwork.h"
#include "../include/MakerbaseSerial.h"
#include "../include/MakerbaseWiFi.h"
#include "../include/send_jpg.h"

#include <stack>

// #define DEFAULT_DIR "gcodes/sda1/"
#define DEFAULT_DIR "gcodes/"

int tty_fd = -1;

// 补偿值
extern bool fresh_page_set_zoffset_data;

// update
extern bool detected_soc_data;
extern bool detected_mcu_data;
extern bool detected_ui_data;

// bool start_to_printing = false;
// preview
extern std::vector<std::string> gimage;
extern std::vector<std::string> simage;

extern std::string str_gimage;

// extern std::vector<std::string> gimage_temp;
// extern std::vector<std::string> simage_temp;
// preview

// wifi start
extern struct mks_wifi_status_result_t status_result;
extern int page_wifi_ssid_list_pages;
extern int page_wifi_current_pages;
// wifi end

extern int page_files_pages;
extern int page_files_current_pages;
extern int page_files_folder_layers;

extern std::string page_files_list_name[9];                                     // 文件列表显示文件名称
extern std::string page_files_list_show_name[9];                                // 文件列表名称
extern std::string page_files_list_show_type[9];   

extern std::stack<std::string> page_files_path_stack;          // 路径栈
extern std::string page_files_root_path;                       // Klippy根目录
extern std::string page_files_previous_path;                   // 之前的路径
extern std::string page_files_path;                            // 文件所在路径
extern std::string page_files_print_files_path;                // 要打印的文件路径

int current_page_id;                    // 当前页面的id号
int previous_page_id;                   // 上一页面的id号
int next_page_id;                       // 下一页面的id号

int event_id;
int page_id;
int widget_id;
int type_id;

int level_mode;                         // 调平模式

bool show_preview_complete = false;
bool show_preview_gimage_completed = false;

bool printing_keyboard_enabled = false;
// bool filament_keyboard_enabled = false;
bool auto_level_button_enabled = true;
bool manual_level_button_enabled = true;

bool printing_wifi_keyboard_enabled = false;

extern std::string printer_idle_timeout_state;
extern std::string printer_print_stats_state;

// 打印调平
int level_mode_printing_extruder_target = 0;
int level_mode_printing_heater_bed_target = 0;

bool level_mode_printing_is_printing_level = false;

bool page_wifi_list_ssid_button_enabled[3] = {false};

// page print filament
bool page_print_filament_extrude_restract_button = false;
bool page_filament_extrude_button = false;

// page reset
// bool page_reset_to_about = false;

extern bool mks_file_parse_finished;

//std::string printed_file_path;

extern int printer_extruder_temperature;
extern int printer_extruder_target;

extern int printer_heater_bed_target;

extern std::string printer_webhooks_state;

extern bool printer_ready;

bool page_filament_unload_button = false;

bool printer_bed_leveling = true;

//4.4.1 CLL 耗材确认弹窗新增不再提示按钮
bool preview_pop_1_on = true;
bool preview_pop_2_on = true;

//4.4.1 CLL 修复断料检测与退料冲突bug
extern bool filament_switch_sensor_fila_enabled;
extern bool previous_filament_sensor_state;

//4.4.2 CLL 新增息屏功能
bool previous_caselight_value = false;
extern float printer_caselight_value;

//4.4.2 CLL 使mates与霍尔耗材宽度传感器适配
extern bool filament_detected;

//4.4.2 CLL 在文件列表页面新增本地、U盘按钮
std::string file_mode = "Local";

//4.4.3 CLL 防止预览图界面卡死
extern bool jump_to_print;

int manual_count;

std::string adjust_mode = "Filament"; // CLL 此字符串决定点击调整按钮后跳转至哪个界面
std::string set_mode = "Level_mode"; // CLL 此字符串决定点击设置按钮后跳转至哪个界面

bool move_fan_setting = false; // CLL 根据此变量确定当前滑块是否被按下
int load_target;

extern bool step_1;
extern bool step_2;
extern bool step_3;
extern bool step_4;

extern bool main_picture_detected;
extern bool main_picture_refreshed;

bool load_mode; // CLL 此变量用于判断打印机正处于装载耗材或退出耗材状态

extern int connection_method;

extern int current_server_page;
extern int total_server_count;

extern int mks_ethernet;

bool qr_refreshed = false; // CLL 此变量用于标记二维码刷新，仅在切换wifi、切换本地/互联网连接、修改服务器后需要重新刷新

void parse_cmd_msg_from_tjc_screen(char *cmd) {
    event_id = cmd[0];
    MKSLOG_BLUE("#########################%s", cmd);
    MKSLOG_RED("0x%x", cmd[0]);
    MKSLOG_RED("0x%x", cmd[1]);
    MKSLOG_RED("0x%x", cmd[2]);
    MKSLOG_RED("0x%x", cmd[3]);
    switch (event_id)
    {
    case 0x03:
        // MKSLOG_BLUE("读取到屏幕固件数据发生错误，进入到恢复屏幕的模式");
        break;

    case 0x05:
        get_0x05 = true;
        std::cout << "可以开始发送数据了" << std::endl;
        MKSLOG_RED("0x%x", cmd[0]);
        // set_option(tty_fd, 921600, 8, 'N', 1);
        // set_option(tty_fd, 230400, 8, 'N', 1);
        // download_to_screen();
        // 收到0x05消息，意味着可以开始发送串口屏幕的信息
        break;

    case 0x1a:
        std::cout << "变量名称无效" << std::endl;
        break;

    case 0x24:
        get_0x24 = true;
        break;

    case 0x65:
        page_id = cmd[1];
        widget_id = cmd[2];
        type_id = cmd[3];
        tjc_event_clicked_handler(page_id, widget_id, type_id);
        break;

    case 0x66:
        break;

    case 0x67:
        break;

    case 0x68:
        break;
    
    case 0x70:
        MKSLOG_RED("0x%x", event_id);
        // page_id = cmd[1];
        // widget_id = cmd[2];
        // type_id = cmd[3];
        tjc_event_keyboard(cmd);
        break;

    case 0x71:
        page_id = cmd[1];
        tjc_event_setted_handler(cmd[1], cmd[2], cmd[3], cmd[4]);
        break;

    case 0x86:
    case 0x87:
    case 0x88:
    case 0x89:
        MKSLOG_RED("0x%x", event_id);
        break;

    case 0x91:
        current_page_id = TJC_PAGE_LOGO;
        page_to(TJC_PAGE_UPDATE_SUCCESS);
        break;

    case 0xfd:
        get_0xfd = true;
        MKSLOG_RED("0x%x 0x%x 0x%x 0x%x ", cmd[0], cmd[1], cmd[2], cmd[3]);
        break;
    case 0xfe:
        get_0xfe = true;
        MKSLOG_RED("0x%x 0x%x 0x%x 0x%x ", cmd[0], cmd[1], cmd[2], cmd[3]);
        // set_option(tty_fd, 230400, 8, 'N', 1);
        // download_to_screen();
        break;

    case 0xff:
        MKSLOG_RED("0x%x", event_id);
        break;

    case 0x04:
        get_0x04 = true;
        MKSLOG_RED("0x%x", cmd[0]);
        break;


    case 0x06:
        get_0x06 = true;
        break;
    
    default:
        break;
    }
    cmd = NULL;
}

void page_to(int page_id) {
    previous_page_id = current_page_id;
    current_page_id = page_id;
    send_cmd_page(tty_fd, std::to_string(page_id));
}

void tjc_event_clicked_handler(int page_id, int widget_id, int type_id) {
    std::cout << "+++++++++++++++++++" << page_id << std::endl;
    std::cout << "+++++++++++++++++++" << widget_id << std::endl;
    std::cout << "+++++++++++++++++++" << type_id << std::endl;
    switch (page_id)
    {
    // 开箱第一项
    case TJC_PAGE_OPEN_LANGUAGE:
        switch (widget_id)
        {
        case TJC_PAGE_OPEN_LANGUAGE_NEXT:
            page_to(TJC_PAGE_OPEN_VIDEO_1);
            get_object_status();
            break;
        
        case TJC_PAGE_OPEN_LANGUAGE_SKIP:
            page_to(TJC_PAGE_OPEN_POP);
            break;

        default:
            break;
        }
        break;
    
    case TJC_PAGE_OPEN_POP:
        switch (widget_id)
        {
        case TJC_PAGE_OPEN_POP_YES:
            page_to(TJC_PAGE_MAIN);
            set_mks_oobe_enabled(false);
            break;

        case TJC_PAGE_OPEN_POP_NO:
            page_to(TJC_PAGE_OPEN_LANGUAGE);
            break;
        
        default:
            break;
        }
        break;

    // 开机第二项
    case TJC_PAGE_OPEN_VIDEO_1:
        switch (widget_id)
        {
        case TJC_PAGE_OPEN_VIDEO_1_NEXT:
            page_to(TJC_PAGE_OPEN_VIDEO_2);
            break;
        
        default:
            break;
        }
        break;

    // 开机第三项
    case TJC_PAGE_OPEN_VIDEO_2:
        switch (widget_id)
        {
        case TJC_PAGE_OPEN_VIDEO_2_NEXT:
            page_to(TJC_PAGE_OPEN_WARNING);
            break;

        default:
            break;
        }
        break;

    // 开机第四项
    case TJC_PAGE_OPEN_WARNING:
        switch (widget_id)
        {
        case TJC_PAGE_OPEN_WARNING_NEXT:
            open_heater_bed_up();
            break;

        default:
            break;
        }
        break;

    // 开机第五项
    case TJC_PAGE_OPEN_VIDEO_3:
        switch (widget_id)
        {
        case TJC_PAGE_OPEN_VIDEO_3_NEXT:
            // page_to(TJC_PAGE_OPEN_HEATERBED);
            page_to(TJC_PAGE_OPEN_FILAMENTVIDEO_1); // CLL 修改开机引导取消调平和共振
            break;

        case TJC_PAGE_OPEN_VIDEO_3_UP:
            set_move_dist(10.0);
            move_z_decrease();
            break;

        case TJC_PAGE_OPEN_VIDEO_3_DOWN:
            set_move_dist(10.0);
            move_z_increase();
            break;

        default:
            break;
        }
        break;
    
    case TJC_PAGE_OPEN_HEATERBED:
        switch(widget_id)
        {
        case TJC_PAGE_OPEN_HEATERBED_DOWN:
            set_auto_level_heater_bed_target(false);
            break;
        
        case TJC_PAGE_OPEN_HEATERBED_UP:
            set_auto_level_heater_bed_target(true);
            break;
        
        case TJC_PAGE_OPEN_HEATERBED_ON_OFF:
            filament_heater_bed_target();
            break;
            
        case TJC_PAGE_OPEN_HEATERBED_NEXT:
            open_calibrate_start();
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_OPEN_FILAMENTVIDEO_0:
        switch (widget_id)
        {
        case TJC_PAGE_OPEN_FILAMENTVIDEO_0_NEXT:
            page_to(TJC_PAGE_OPEN_FILAMENTVIDEO_1);
            break;
        
        default:
            break;
        }
    
    case TJC_PAGE_OPEN_FILAMENTVIDEO_1:
        switch (widget_id)
        {
        case TJC_PAGE_OPEN_FILAMENTVIDEO_1_NEXT:
            page_to(TJC_PAGE_OPEN_FILAMENTVIDEO_2);
            break;

        default:
            break;
        }
        break;
    
    case TJC_PAGE_OPEN_FILAMENTVIDEO_2:
        switch (widget_id)
        {
        case TJC_PAGE_OPEN_FILAMENTVIDEO_2_UP:
            set_filament_extruder_target(true);
            break;

        case TJC_PAGE_OPEN_FILAMENTVIDEO_2_DOWN:
            set_filament_extruder_target(false);
            break;

        case TJC_PAGE_OPEN_FILAMENTVIDEO_2_NEXT:
            page_to(TJC_PAGE_OPEN_FILAMENTVIDEO_3);
            break;

        case TJC_PAGE_OPEN_FILAMENTVIDEO_2_ON_OFF:
            open_set_print_filament_target();
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_OPEN_FILAMENTVIDEO_3:
        switch (widget_id)
        {
        case TJC_PAGE_OPEN_FILAMENTVIDEO_3_NEXT:
            set_extruder_target(0);
            page_to(TJC_PAGE_OPEN_FINISH);
            break;

        case TJC_PAGE_OPEN_FILAMENTVIDEO_3_EXTRUDE:
            open_start_extrude();
            break;

        default:
            break;
        }
        break;

    case TJC_PAGE_OPEN_FINISH:
        switch (widget_id)
        {
        case TJC_PAGE_OPEN_FINISH_YES:
            open_more_level_finish();
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_MAIN:
        switch (widget_id)
        {
        case TJC_PAGE_ALL_TO_MAIN:
            break;

        case TJC_PAGE_ALL_TO_FILE_LIST:
            go_to_file_list();
            break;

        case TJC_PAGE_ALL_TO_ADJUST:
            go_to_adjust();
            break;

        case TJC_PAGE_ALL_TO_SETTING:
            go_to_setting();
            break;
        
        case TJC_PAGE_MAIN_CASELIGHT:
            led_on_off();
            break;

        case TJC_PAGE_MAIN_BEEP:
            beep_on_off();
            break;

        case TJC_PAGE_MAIN_STOP:
            motors_off();
            break;

        case TJC_PAGE_MAIN_SET_TEMP:
            adjust_mode = "Filament";
            page_to(TJC_PAGE_FILAMENT);
            break;

        case TJC_PAGE_MAIN_CACHE:
            page_files_pages = 0;
            page_files_current_pages = 0;
            page_files_folder_layers = 0;
            page_files_previous_path = "";
            page_files_root_path = DEFAULT_DIR;
            page_files_path = "";
            refresh_page_files(page_files_current_pages);
            if (page_files_list_show_type[0] == "[c]") {
                clear_cp0_image();
                get_sub_dir_files_list(0);
                file_mode = "Local";
            }
            break;
        

        default:
            break;
        }
        break;

    case TJC_PAGE_FILE_LIST:
        switch (widget_id)
        {
        case TJC_PAGE_ALL_TO_MAIN:
            page_to(TJC_PAGE_MAIN);
            break;

        case TJC_PAGE_ALL_TO_FILE_LIST:
            break;

        case TJC_PAGE_ALL_TO_ADJUST:
            go_to_adjust();
            break;

        case TJC_PAGE_ALL_TO_SETTING:
            go_to_setting();
            break;

        case TJC_PAGE_FILE_LIST_BACK:
            if (page_files_folder_layers == 0 || (page_files_folder_layers == 1 && file_mode != "Local")) {
            } else {
                get_parenet_dir_files_list();
            }
            break;

        case TJC_PAGE_FILE_LIST_BTN_1:
            clear_cp0_image();
            get_sub_dir_files_list(0);
            printer_bed_leveling = true;
            break;

        case TJC_PAGE_FILE_LIST_BTN_2:
            clear_cp0_image();
            get_sub_dir_files_list(1);
            printer_bed_leveling = true;
            break;

        case TJC_PAGE_FILE_LIST_BTN_3:
            clear_cp0_image();
            get_sub_dir_files_list(2);
            printer_bed_leveling = true;
            break;

        case TJC_PAGE_FILE_LIST_BTN_4:
            clear_cp0_image();
            get_sub_dir_files_list(3);
            printer_bed_leveling = true;
            break;

        case TJC_PAGE_FILE_LIST_PREVIOUS:
            if (detect_disk() == -1 && file_mode == "USB") {
                go_to_file_list();
            } else if (page_files_current_pages > 0) {
                page_files_current_pages -= 1;
                refresh_page_files(page_files_current_pages);
                refresh_page_files_list();
            }
            MKSLOG_BLUE("%d", page_files_folder_layers);
            break;
        
        case TJC_PAGE_FILE_LIST_NEXT:
            if (detect_disk() == -1 && file_mode == "USB") {
                go_to_file_list();
            } else if (page_files_current_pages < page_files_pages) {
                page_files_current_pages += 1;
                refresh_page_files(page_files_current_pages);
                refresh_page_files_list();
            }
            MKSLOG_BLUE("%d", page_files_folder_layers);
            break;
        
        //4.4.2 CLL 在文件列表页面新增本地、U盘按钮
        case TJC_PAGE_FILE_LIST_LOCAL:
            if (file_mode != "Local") {
                file_mode = "Local";
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
            }
            break;

        case TJC_PAGE_FILE_LIST_USB:
            if (file_mode != "USB") {
                file_mode = "USB";
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

        default:
            break;
        }
        break;

    case TJC_PAGE_PREVIEW:
        if (current_page_id == TJC_PAGE_PREVIEW) {
            switch (widget_id)
            {
            case TJC_PAGE_ALL_TO_MAIN:
                if (printer_print_stats_state == "printing" || printer_print_stats_state == "paused") {
                    page_to(TJC_PAGE_PRINTING);
                    jump_to_print = false;
                } else
                    page_to(TJC_PAGE_MAIN);
                break;
    
            case TJC_PAGE_ALL_TO_FILE_LIST:
                break;

            case TJC_PAGE_ALL_TO_ADJUST:
                if (printer_print_stats_state == "printing" || printer_print_stats_state == "paused") {
                    page_to(TJC_PAGE_PRINTING);
                    jump_to_print = false;
                } else
                    go_to_adjust();
                break;

            case TJC_PAGE_ALL_TO_SETTING:
                if (printer_print_stats_state == "printing" || printer_print_stats_state == "paused") {
                    page_to(TJC_PAGE_PRINTING);
                    jump_to_print = false;
                } else
                    go_to_setting();
                break;

            case TJC_PAGE_PREVIEW_BACK:
                //4.4.3 CLL 防止预览图界面卡死
                if (printer_print_stats_state == "printing" || printer_print_stats_state == "paused") {
                    page_to(TJC_PAGE_PRINTING);
                    jump_to_print = false;
                } else if (mks_file_parse_finished == false) {
                    get_parenet_dir_files_list();
                    clear_page_preview();
                    show_preview_complete = false;
                    clear_cp0_image();
                } else {
                    if (show_preview_complete == true) {        // 当且仅当预览加载完成才可以按下按钮
                        get_parenet_dir_files_list();
                        clear_page_preview();                   // 返回时清除读取的数据
                        show_preview_complete = false;
                        clear_cp0_image();
                    }
                }
                break;

            case TJC_PAGE_PREVIEW_START:
                if (printer_print_stats_state == "printing" || printer_print_stats_state == "paused") {
                    page_to(TJC_PAGE_PRINTING);
                    jump_to_print = false;
                } else if (show_preview_complete == true) {
                    print_start();
                    sleep(1);
                    if (filament_detected == true) {
                        MKSLOG("未检测到断料");
                        printer_print_stats_state = "printing";
                        check_filament_type();
                        start_printing(page_files_print_files_path);
                        show_preview_complete = false;
                    } else {
                        MKSLOG("检测到断料");
                        page_to(TJC_PAGE_PRINT_NO_FILAMENT);
                    }
                }
                main_picture_detected = false;
                main_picture_refreshed = false;
                break;

            case TJC_PAGE_PREVIEW_BED_LEVELING:
                if (printer_print_stats_state == "printing" || printer_print_stats_state == "paused") {
                    page_to(TJC_PAGE_PRINTING);
                    jump_to_print = false;
                    break;
                }
                if (printer_bed_leveling == true) {
                    printer_bed_leveling = false;
                }else {
                    printer_bed_leveling = true;
                }
                break;

            default:
                break;
            }
        }
        break;

    case TJC_PAGE_PREVIEW_POP_1:
    case TJC_PAGE_PREVIEW_POP_2:
        switch (widget_id)
        {
        case TJC_PAGE_PREVIEW_POP_YES:
            page_to(TJC_PAGE_PRINTING);
            break;
            
        case TJC_PAGE_PREVIEW_POP_NO_POP:
            if (current_page_id == TJC_PAGE_PREVIEW_POP_1) {
                preview_pop_1_on = false;
            } else if (current_page_id == TJC_PAGE_PREVIEW_POP_2) {
                preview_pop_2_on = false;
            }
            page_to(TJC_PAGE_PRINTING);
            break;

        default:
            break;
        }
        break;

    case TJC_PAGE_PRINTING:
        switch (widget_id)
        {
        case TJC_PAGE_PRINTING_EXTRUDER:
        case TJC_PAGE_PRINTING_HEATER_BED:
        case TJC_PAGE_PRINTING_FAN_1:
        case TJC_PAGE_PRINTING_FAN_2:
        case TJC_PAGE_PRINTING_FAN_3:
        case TJC_PAGE_PRINTING_HOT:
            printing_keyboard_enabled = true;
            clear_page_printing_arg();
            break;

        case TJC_PAGE_PRINTING_NEXT:
            page_to(TJC_PAGE_PRINTING_2);
            break;

        case TJC_PAGE_PRINTING_CASE_LIGHT:
            led_on_off();
            break;

        case TJC_PAGE_PRINTING_PAUSE_RESUME:
            printer_ready = false;
            set_print_pause();
            page_to(TJC_PAGE_PRINT_FILAMENT);
            clear_page_printing_arg();
            break;

        case TJC_PAGE_PRINTING_STOP:
            page_to(TJC_PAGE_PRINT_STOP);
            clear_page_printing_arg();
            break;

        default:
            break;
        }
        break;

    case TJC_PAGE_PRINTING_KB:
        switch(widget_id)
        {
        case TJC_PAGE_PRINTING_KB_BACK:
            printing_keyboard_enabled = false;
            MKSLOG_BLUE("已恢复");
            break;

        case TJC_PAGE_PRINTING_KB_PAUSE_RESUME:
            printing_keyboard_enabled = false;
            set_print_pause();
            page_to(TJC_PAGE_PRINT_FILAMENT);
            clear_page_printing_arg();
            break;
        
        case TJC_PAGE_PRINTING_KB_STOP:
            printing_keyboard_enabled = false;
            page_to(TJC_PAGE_PRINT_STOP);
            clear_page_printing_arg();
            break;
        
        default:
            break;
            
        }
        break;

    case TJC_PAGE_PRINT_ZOFFSET:
        switch (widget_id)
        {
        case TJC_PAGE_PRINT_ZOFFSET_BACK:
            printing_keyboard_enabled = false;
            page_to(TJC_PAGE_PRINTING_2);
            break;

        case TJC_PAGE_PRINT_ZOFFSET_SET_001:
            set_intern_zoffset(0.01);
            break;
        
        case TJC_PAGE_PRINT_ZOFFSET_SET_005:
            set_intern_zoffset(0.05);
            break;
        
        case TJC_PAGE_PRINT_ZOFFSET_SET_01:
            set_intern_zoffset(0.1);
            break;
        
        case TJC_PAGE_PRINT_ZOFFSET_SET_05:
            set_intern_zoffset(0.5);
            break;
        
        case TJC_PAGE_PRINT_ZOFFSET_UP:
            set_zoffset(false);
            break;
        
        case TJC_PAGE_PRINT_ZOFFSET_DOWN:
            set_zoffset(true);
            break;
        
        case TJC_PAGE_PRINT_ZOFFSET_PAUSE_RESUME:
            printer_ready = false;
            set_print_pause();
            page_to(TJC_PAGE_PRINT_FILAMENT);
            clear_page_printing_arg();
            break;
        
        case TJC_PAGE_PRINT_ZOFFSET_STOP:
            page_to(TJC_PAGE_PRINT_STOP);
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_PRINT_FILAMENT:
        switch (widget_id)
        {
        case TJC_PAGE_PRINT_FILAMENT_ON_OFF:
            set_print_filament_target();
            break;

        case TJC_PAGE_PRINT_FILAMENT_T_UP:
            set_filament_extruder_target(true);
            break;

        case TJC_PAGE_PRINT_FILAMENT_T_DOWN:
            set_filament_extruder_target(false);
            break;

        case TJC_PAGE_PRINT_FILAMENT_LOAD:
            load_mode = true;
            page_to(TJC_PAGE_PRE_HEAT);
            break;

        case TJC_PAGE_PRINT_FILAMENT_UNLOAD:
            load_mode = false;
            page_to(TJC_PAGE_PRE_HEAT);
            break;

        case TJC_PAGE_PRINT_FILAMENT_PAUSE_RESUME:
            MKSLOG_BLUE("get_filament_detected_enable: %d", get_filament_detected_enable());
            MKSLOG_BLUE("get_filament_detected: %d", get_filament_detected());
            printer_ready = false;
            page_to(TJC_PAGE_PRINTING);
            set_print_resume();
            break;

        case TJC_PAGE_PRINT_FILAMENT_STOP:
            page_to(TJC_PAGE_PRINT_STOP);
            clear_page_printing_arg();
            break;

        case TJC_PAGE_PRINT_FILAMENT_RETRACT:
            // set_print_filament_dist(50);
            // start_retract();
            send_gcode("M603\n");
            break;

        case TJC_PAGE_PRINT_FILAMENT_EXTRUDE:
            set_print_filament_dist(50);
            start_extrude();
            break;

        default:
            break;
        }
        break;

    case TJC_PAGE_PRINTING_2:
        switch (widget_id)
        {
        case TJC_PAGE_PRINTING_2_BACK:
            page_to(TJC_PAGE_PRINTING);
            break;

        case TJC_PAGE_PRINTING_2_SPEED:
        case TJC_PAGE_PRINTING_2_FLOW:
            printing_keyboard_enabled = true;
            clear_page_printing_arg();
            break;

        case TJC_PAGE_PRINTING_2_ZOFFSET:
            page_to(TJC_PAGE_PRINT_ZOFFSET);
            break;

        case TJC_PAGE_PRINTING_2_PAUSE_RESUME:
            printer_ready = false;
            set_print_pause();
            page_to(TJC_PAGE_PRINT_FILAMENT);
            clear_page_printing_arg();
            break;
        
        case TJC_PAGE_PRINTING_2_STOP:
            page_to(TJC_PAGE_PRINT_STOP);
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_PRINT_FINISH:
        switch (widget_id)
        {
        case TJC_PAGE_PRINT_FINISH_YES:
            finish_print();
            // page_to(TJC_PAGE_MAIN);
            break;

        default:
            break;
        }
        break;

    case TJC_PAGE_PRINT_STOP:
        switch (widget_id)
        {
        case TJC_PAGE_PRINT_STOP_YES:
            printer_idle_timeout_state = "Printing";
            page_to(TJC_PAGE_PRINT_STOPPING);
            cancel_print();
            break;

        case TJC_PAGE_PRINT_STOP_NO:
            page_to(previous_page_id);
            break;

        default:
            break;
        }
        break;

    case TJC_PAGE_PRINT_NO_FILAMENT:
        switch (widget_id)
        {
        case TJC_PAGE_PRINT_NO_FILAMENT_YES:
            filament_detected = true;
            if (previous_page_id == TJC_PAGE_PREVIEW) {
                get_object_status();
                page_to(TJC_PAGE_MOVE);
            } else {
                get_object_status();
                page_to(TJC_PAGE_PRINT_FILAMENT);
            }
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_PRINT_NO_FILAMENT_2:
        switch (widget_id)
        {
        case TJC_PAGE_PRINT_NO_FILAMENT_2_YES:
            get_object_status();
            page_to(TJC_PAGE_PRINT_FILAMENT);
            break;
        
        default:
            break;
        }

    case TJC_PAGE_PRINT_LOW_TEMP:
        switch (widget_id)
        {
        case TJC_PAGE_PRINT_LOW_TEMP_YES:
            printer_idle_timeout_state = "Ready";
            page_to(TJC_PAGE_PRINT_FILAMENT);
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_MOVE:
        switch (widget_id)
        {
        case TJC_PAGE_MOVE_SET_01:
            set_move_dist(0.1);
            break;

        case TJC_PAGE_MOVE_SET_1:
            set_move_dist(1.0);
            break;

        case TJC_PAGE_MOVE_SET_10:
            set_move_dist(10.0);
            break;

        case TJC_PAGE_MOVE_Z_UP:
            move_z_decrease();
            break;

        case TJC_PAGE_MOVE_Z_DOWN:
            move_z_increase();
            break;

        case TJC_PAGE_MOVE_MOTOR:
            move_motors_off();
            break;

        case TJC_PAGE_MOVE_X_UP:
            move_x_increase();
            break;

        case TJC_PAGE_MOVE_X_DOWN:
            move_x_decrease();
            break;

        case TJC_PAGE_MOVE_Y_UP:
            move_y_increase();
            break;

        case TJC_PAGE_MOVE_Y_DOWN:
            move_y_decrease();
            break;

        case TJC_PAGE_MOVE_HOME:
            move_home();
            break;
        
        case TJC_PAGE_MOVE_TO_FILAMENT:
            page_to(TJC_PAGE_FILAMENT);
            adjust_mode = "Filament";
            break;

        case TJC_PAGE_ALL_TO_MAIN:
            page_to(TJC_PAGE_MAIN);
            break;

        case TJC_PAGE_ALL_TO_FILE_LIST:
            go_to_file_list();
            break;

        case TJC_PAGE_ALL_TO_ADJUST:
            break;

        case TJC_PAGE_ALL_TO_SETTING:
            go_to_setting();
            break;

        default:
            break;
        }
        break;

    case TJC_PAGE_MOVE_POP_1:
        switch (widget_id)
        {
        case TJC_PAGE_MOVE_POP_1_YES:
            if(previous_page_id == TJC_PAGE_PRINTING || previous_page_id == TJC_PAGE_PRINT_ZOFFSET || previous_page_id == TJC_PAGE_PRINTING_2) {
                cancel_print();
                page_to(TJC_PAGE_PRINT_STOPPING);
            } else {
                page_to(TJC_PAGE_MOVE);
            }
            break;
        
        default:
            break;
        }
        break;
    
    case TJC_PAGE_MOVE_POP_2:
        switch(widget_id)
        {
        case TJC_PAGE_MOVE_POP_2_YES:
            page_to(TJC_PAGE_MOVE);
            move_home();
            break;
        
        case TJC_PAGE_MOVE_POP_2_NO:
            page_to(TJC_PAGE_MOVE);
            break;

        default:
            break;
        }
        break;

    case TJC_PAGE_FILAMENT_SET_FAN:
        switch (widget_id)
        {
        case TJC_PAGE_ALL_TO_MAIN:
            page_to(TJC_PAGE_MAIN);
            break;

        case TJC_PAGE_ALL_TO_FILE_LIST:
            go_to_file_list();
            break;

        case TJC_PAGE_ALL_TO_ADJUST:
            break;

        case TJC_PAGE_ALL_TO_SETTING:
            go_to_setting();
            break;

        case TJC_PAGE_FILAMENT_SET_FAN_BACK:
            page_to(TJC_PAGE_FILAMENT);
            break;
        
        case TJC_PAGE_FILAMENT_SET_FAN_SETTING:
            move_fan_setting = true; // 收到信息后确认滑块被按下
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_FILAMENT_KB:
        switch (widget_id)
        {
        case TJC_PAGE_ALL_TO_MAIN:
            page_to(TJC_PAGE_MAIN);
            break;

        case TJC_PAGE_ALL_TO_FILE_LIST:
            go_to_file_list();
            break;

        case TJC_PAGE_ALL_TO_ADJUST:
            break;

        case TJC_PAGE_ALL_TO_SETTING:
            go_to_setting();
            break;
        
        case TJC_PAGE_FILAMENT_KB_BACK:
            page_to(TJC_PAGE_FILAMENT);
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_FILAMENT:
        switch (widget_id)
        {
        case TJC_PAGE_ALL_TO_MAIN:
            page_to(TJC_PAGE_MAIN);
            break;

        case TJC_PAGE_ALL_TO_FILE_LIST:
            go_to_file_list();
            break;

        case TJC_PAGE_ALL_TO_ADJUST:
            break;

        case TJC_PAGE_ALL_TO_SETTING:
            go_to_setting();
            break;

        case TJC_PAGE_FILAMENT_SET_EXTRUDER:
        case TJC_PAGE_FILAMENT_SET_HEATERBED:
        case TJC_PAGE_FILAMENT_SET_HOT:
            page_to(TJC_PAGE_FILAMENT_KB);
            break;

        case TJC_PAGE_FILAMENT_EXTRUDER_UP:
            printer_idle_timeout_state = "Printing";
            page_filament_extrude_button = true;
            start_retract();
            break;

        case TJC_PAGE_FILAMENT_EXTRUDER_DOWN:
            printer_idle_timeout_state = "Printing";
            page_filament_extrude_button = true;
            start_extrude();
            break;

        case TJC_PAGE_FILAMENT_EXTRUDER_ON_OFF:
            filament_extruder_target();
            break;

        case TJC_PAGE_FILAMENT_HEATERBED_ON_OFF:
            filament_heater_bed_target();
            break;

        case TJC_PAGE_FILAMENT_HOT_ON_OFF:
            filament_hot_target();
            break;

        case TJC_PAGE_FILAMENT_TO_FAN:
            page_to(TJC_PAGE_FILAMENT_SET_FAN);
            break;

        case TJC_PAGE_FILAMENT_LOAD:
            load_mode = true;
            page_to(TJC_PAGE_PRE_HEAT);
            break;

        case TJC_PAGE_FILAMENT_UNLOAD:
            load_mode = false;
            page_to(TJC_PAGE_PRE_HEAT);
            break;

        case TJC_PAGE_FILAMENT_SET_10:
            set_print_filament_dist(10);
            break;

        case TJC_PAGE_FILAMENT_SET_50:
            set_print_filament_dist(50);
            break;

        case TJC_PAGE_FILAMENT_SET_100:
            set_print_filament_dist(100);
            break;

        case TJC_PAGE_FILAMENT_TO_MOVE:
            page_to(TJC_PAGE_MOVE);
            adjust_mode = "Move";
            break;

        default:
            break;
        }
        break;

    case TJC_PAGE_FILAMENT_POP_1:
        switch(widget_id)
        {
        case TJC_PAGE_FILAMENT_POP_1_YES:
            printer_idle_timeout_state = "Ready";
            page_to(TJC_PAGE_FILAMENT);
            break;
        }
        break;

    case TJC_PAGE_FILAMENT_POP_2:
        switch(widget_id)
        {
        case TJC_PAGE_FILAMENT_POP_2_YES:
            send_gcode("SET_HEATER_TEMPERATURE HEATER=extruder TARGET=0\n");
            if (printer_print_stats_state == "paused") {
                page_to(TJC_PAGE_PRINT_FILAMENT);
            } else {
                page_to(TJC_PAGE_FILAMENT);
            }
            break;
            
        case TJC_PAGE_FILAMENT_POP_2_TO_LOAD:
            load_mode = true;
            page_to(TJC_PAGE_PRE_HEAT);
            break;

        case TJC_PAGE_FILAMENT_POP_2_NEXT:
            filament_load();
            // filament_unload();
            break;

        case TJC_PAGE_FILAMENT_POP_2_BACK:
            page_to(TJC_PAGE_UNLOAD_MODE);
            break;
            
        default:
            break;
        }
        break;

    case TJC_PAGE_FILAMENT_POP_3:
        switch(widget_id)
        {
        case TJC_PAGE_FILAMENT_POP_3_NEXT:
            filament_load();
            break;

        case TJC_PAGE_FILAMENT_POP_3_YES:
            send_gcode("SET_HEATER_TEMPERATURE HEATER=extruder TARGET=0\n");
            if (printer_print_stats_state == "paused") {
                page_to(TJC_PAGE_PRINT_FILAMENT);
            } else {
                page_to(TJC_PAGE_FILAMENT);
            }
            break;

        case TJC_PAGE_FILAMENT_POP_3_RETRY:
            load_mode = true;
            page_to(TJC_PAGE_FILAMENT_POP_3);
            filament_load();
            break;

        case TJC_PAGE_FILAMENT_POP_3_BACK:
            page_to(TJC_PAGE_PRE_HEAT);
            break;
        
        default:
            break;
        }
        break;    
    
    case TJC_PAGE_FILAMENT_UNLOAD_FINISH:
        switch (widget_id)
        {
        case TJC_PAGE_FILAMENT_UNLOAD_FINISH_YES:
            page_to(TJC_PAGE_FILAMENT);
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_LEVEL_MODE:
        switch(widget_id)
        {
        case TJC_PAGE_ALL_TO_MAIN:
            page_to(TJC_PAGE_MAIN);
            break;

        case TJC_PAGE_ALL_TO_FILE_LIST:
            go_to_file_list();
            break;

        case TJC_PAGE_ALL_TO_ADJUST:
            go_to_adjust();
            break;

        case TJC_PAGE_ALL_TO_SETTING:
            break;

        case TJC_PAGE_LEVEL_MODE_AUTO_LEVEL:
            get_object_status();
            page_to(TJC_PAGE_AUTO_HEATERBED);
            break;
        
        case TJC_PAGE_LEVEL_MODE_SYNTONY:
            go_to_syntony_move();
            break;

        case TJC_PAGE_LEVEL_MODE_BED_CALIBRATION:
            page_to(TJC_PAGE_CALIBRATE_WARNING);
            break;

        case TJC_PAGE_LEVEL_MODE_TO_COMMON_SETTING:
            page_to(TJC_PAGE_COMMON_SETTING);
            set_mode = "Common_setting";
            break;

        case TJC_PAGE_LEVEL_MODE_ZOFFSET:
            page_to(TJC_PAGE_ZOFFSET);
            break;

        default:
            break;
        }
        break;
    
    case TJC_PAGE_ZOFFSET:
        switch (widget_id)
        {
        case TJC_PAGE_ZOFFSET_BACK:
            page_to(TJC_PAGE_LEVEL_MODE);
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_AUTO_HEATERBED:
        switch (widget_id)
        {
        case TJC_PAGE_AUTO_HEATERBED_DOWN:
            set_auto_level_heater_bed_target(false);
            break;

        case TJC_PAGE_AUTO_HEATERBED_UP:
            set_auto_level_heater_bed_target(true);
            break;
        
        case TJC_PAGE_AUTO_HEATERBED_ON_OFF:
            filament_heater_bed_target();
            break;

        case TJC_PAGE_AUTO_HEATERBED_BACK:
            page_to(TJC_PAGE_LEVEL_MODE);
            break;

        case TJC_PAGE_AUTO_HEATERBED_NEXT:
            if (printer_heater_bed_target < 35) {
                page_to(TJC_PAGE_AUTO_WARNING);
                break;
            }
            auto_level_button_enabled = true;
            printer_idle_timeout_state = "Printing";
            start_auto_level();
            break;

        default:
            break;
        }
        break;

    case TJC_PAGE_AUTO_FINISH:
        switch (widget_id)
        {
        case TJC_PAGE_AUTO_FINISH_YES:
            std::cout << "自动调平已完成" << std::endl;
            page_to(TJC_PAGE_LEVEL_MODE);
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_PRE_BED_CALIBRATION:
        switch (widget_id)
        {
        case TJC_PAGE_PRE_BED_CALIBRATION_SET_001:
            set_auto_level_dist(0.01);
            break;

        case TJC_PAGE_PRE_BED_CALIBRATION_SET_005:
            set_auto_level_dist(0.05);
            break;
        
        case TJC_PAGE_PRE_BED_CALIBRATION_SET_01:
            set_auto_level_dist(0.1);
            break;

        case TJC_PAGE_PRE_BED_CALIBRATION_SET_05:
            set_auto_level_dist(0.5);
            break;
        
        case TJC_PAGE_PRE_BED_CALIBRATION_UP:
            bed_adjust(true);
            break;

        case TJC_PAGE_PRE_BED_CALIBRATION_DOWN:
            bed_adjust(false);
            break;

        case TJC_PAGE_PRE_BED_CALIBRATION_ENTER:
            bed_calibrate();
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_BED_CALIBRATION:
        switch (widget_id)
        {
        case TJC_PAGE_BED_CALIBRATION_NEXT:
            bed_calibrate();
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_BED_FINISH:
        switch (widget_id)
        {
        case TJC_PAGE_BED_FINISH_OK:
            page_to(TJC_PAGE_LEVEL_MODE);
            break;

        case TJC_PAGE_BED_FINISH_SCREW1:
            printer_idle_timeout_state = "Printing";
            send_gcode("G1 Z10 F600\n");
            send_gcode("G1 X10 Y10 F9000\n");
            send_gcode("G1 Z0 F600\n");
            manual_count = -1;
            page_to(TJC_PAGE_BED_MOVING);
            break;

        case TJC_PAGE_BED_FINISH_SCREW2:
            printer_idle_timeout_state = "Printing";
            send_gcode("G1 Z10 F600\n");
            send_gcode("G1 X230 Y10 F9000\n");
            send_gcode("G1 Z0 F600\n");
            manual_count = -1;
            page_to(TJC_PAGE_BED_MOVING);
            break;

        case TJC_PAGE_BED_FINISH_SCREW3:
            printer_idle_timeout_state = "Printing";
            send_gcode("G1 Z10 F600\n");
            send_gcode("G1 X125 Y240 F9000\n");
            send_gcode("G1 Z0 F600\n");
            manual_count = -1;
            page_to(TJC_PAGE_BED_MOVING);
            break;

        case TJC_PAGE_BED_FINISH_Z_TILT:
            printer_idle_timeout_state = "Printing";
            send_gcode("G28\nZ_TILT_ADJUST\n");
            send_gcode("G1 Z10 F600\nG1 X0 Y0 F9000\n");
            manual_count = -2;
            page_to(TJC_PAGE_BED_MOVING);
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_SYNTONY_MOVE:
        switch(widget_id) {
        case TJC_PAGE_SYNTONY_MOVE_JUMP_OUT:
            send_gcode("SAVE_CONFIG\n");
            page_to(TJC_PAGE_SYNTONY_FINISH);
        }
        break;

    case TJC_PAGE_SYNTONY_FINISH:
        switch (widget_id)
        {
        case TJC_PAGE_SYNTONY_FINISH_YES:
            page_to(TJC_PAGE_LEVEL_MODE);
            system("sync");
            init_mks_status();
            sub_object_status();
            get_object_status();
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_INTERNET:
        switch (widget_id)
        {
        case TJC_PAGE_ALL_TO_MAIN:
            if (printer_webhooks_state != "shutdown" && printer_webhooks_state != "error") {
                page_to(TJC_PAGE_MAIN);
            }
            break;

        case TJC_PAGE_ALL_TO_FILE_LIST:
            if (printer_webhooks_state != "shutdown" && printer_webhooks_state != "error") {
                go_to_file_list();
            }
            break;

        case TJC_PAGE_ALL_TO_ADJUST:
            if (printer_webhooks_state != "shutdown" && printer_webhooks_state != "error") {
                go_to_adjust();
            }
            break;

        case TJC_PAGE_ALL_TO_SETTING:
            break;
        
        case TJC_PAGE_INTERNET_REFRESH:
            std::cout << "################## 按下刷新按钮" << std::endl;
            scan_ssid_and_show();
            std::cout << "等待延时测试3s..." << std::endl;
            sleep(3);
            scan_ssid_and_show();
            break;
        
        case TJC_PAGE_INTERNET_TO_WIFI:
            break;
            
        case TJC_PAGE_INTERNET_TO_SETTING:
            page_to(TJC_PAGE_COMMON_SETTING);
            break;

        default:
            break;
        }
        break;

    case TJC_PAGE_WIFI_LIST:
        switch (widget_id)
        {
        case TJC_PAGE_ALL_TO_MAIN:
            if (printer_webhooks_state != "shutdown" && printer_webhooks_state != "error") {
                page_to(TJC_PAGE_MAIN);
            }
            break;

        case TJC_PAGE_ALL_TO_FILE_LIST:
            if (printer_webhooks_state != "shutdown" && printer_webhooks_state != "error") {
                go_to_file_list();
            }
            break;

        case TJC_PAGE_ALL_TO_ADJUST:
            if (printer_webhooks_state != "shutdown" && printer_webhooks_state != "error") {
                go_to_adjust();
            }
            break;

        case TJC_PAGE_ALL_TO_SETTING:
            break;

        case TJC_PAGE_WIFI_LIST_SSID_1:
            if (page_wifi_list_ssid_button_enabled[0] == true) {
                get_wifi_list_ssid(0);
                printing_wifi_keyboard_enabled = true;
                page_to(TJC_PAGE_WIFI_KB);
            }
            break;

        case TJC_PAGE_WIFI_LIST_SSID_2:
            if (true == page_wifi_list_ssid_button_enabled[1]) {
                get_wifi_list_ssid(1);
                printing_wifi_keyboard_enabled = true;
                page_to(TJC_PAGE_WIFI_KB);
            }
            break;

        case TJC_PAGE_WIFI_LIST_SSID_3:
            if (true == page_wifi_list_ssid_button_enabled[2]) {
                get_wifi_list_ssid(2);
                printing_wifi_keyboard_enabled = true;
                page_to(TJC_PAGE_WIFI_KB);
            }
            break;
        
        case TJC_PAGE_WIFI_LIST_SSID_4:
            if (true == page_wifi_list_ssid_button_enabled[3]) {
                get_wifi_list_ssid(3);
                printing_wifi_keyboard_enabled = true;
                page_to(TJC_PAGE_WIFI_KB);
            }
            break;

        case TJC_PAGE_WIFI_LIST_SSID_5:
            if (true == page_wifi_list_ssid_button_enabled[4]) {
                get_wifi_list_ssid(4);
                printing_wifi_keyboard_enabled = true;
                page_to(TJC_PAGE_WIFI_KB);
            }
            break;

        case TJC_PAGE_WIFI_LIST_REFRESH:
            std::cout << "################## 按下刷新按钮" << std::endl;
            scan_ssid_and_show();
            //4.4.1 CLL 修复WiFi刷新
            //std::cout << "等待延时测试3s..." << std::endl;
            //sleep(3);
            //scan_ssid_and_show();
            break;

        case TJC_PAGE_WIFI_LIST_PREVIOUS:
            if (page_wifi_current_pages > 0) {
                std::cout << "page_wifi_current_pages = " << page_wifi_current_pages << std::endl;
                std::cout << "page_wifi_ssid_list_pages = " << page_wifi_ssid_list_pages << std::endl;
                page_wifi_current_pages -= 1;
                set_page_wifi_ssid_list(page_wifi_current_pages);
                refresh_page_wifi_list();
            }
            break;

        case TJC_PAGE_WIFI_LIST_NEXT:
            if (page_wifi_current_pages < page_wifi_ssid_list_pages - 1) {
                std::cout << "page_wifi_current_pages = " << page_wifi_current_pages << std::endl;
                std::cout << "page_wifi_ssid_list_pages = " << page_wifi_ssid_list_pages << std::endl;
                page_wifi_current_pages += 1;
                set_page_wifi_ssid_list(page_wifi_current_pages);
                refresh_page_wifi_list();
            }
            break;

        case TJC_PAGE_WIFI_LIST_TO_WIFI:
            break;
            
        case TJC_PAGE_WIFI_LIST_TO_SETTING:
            // page_to(TJC_PAGE_COMMON_SETTING);
            go_to_showqr();
            break;

        default:
            break;
        }
        break;

    case TJC_PAGE_WIFI_SUCCESS:
        switch (widget_id)
        {
        case TJC_PAGE_WIFI_SUCCESS_YES:
            wifi_save_config();
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_WIFI_FAILED:
        switch (widget_id)
        {
        case TJC_PAGE_WIFI_FAILED_YES:
            page_to(TJC_PAGE_WIFI_LIST);
            break;

        default:
            break;
        }
        break;
    
    case TJC_PAGE_WIFI_KB:
        switch (widget_id)
        {
        case TJC_PAGE_WIFI_KB_BACK:
            page_to(TJC_PAGE_WIFI_LIST);
            printing_wifi_keyboard_enabled = false;
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_COMMON_SETTING:
        switch(widget_id)
        {
        case TJC_PAGE_ALL_TO_MAIN:
            if (printer_webhooks_state != "shutdown" && printer_webhooks_state != "error") {
                page_to(TJC_PAGE_MAIN);
            }
            break;

        case TJC_PAGE_ALL_TO_FILE_LIST:
            if (printer_webhooks_state != "shutdown" && printer_webhooks_state != "error") {
                go_to_file_list();
            }
            break;

        case TJC_PAGE_ALL_TO_ADJUST:
            if (printer_webhooks_state != "shutdown" && printer_webhooks_state != "error") {
                go_to_adjust();
            }
            break;

        case TJC_PAGE_ALL_TO_SETTING:
            break;

        case TJC_PAGE_COMMON_SETTING_LANGUAGE:
            page_to(TJC_PAGE_LANGUAGE);
            break;

        case TJC_PAGE_COMMON_SETTING_WIFI:
            // go_to_network();
            go_to_showqr();
            break;

        case TJC_PAGE_COMMON_SETTING_SYSTEM:
            go_to_reset();
            break;

        case TJC_PAGE_COMMON_SETTING_SERVICE:
            page_to(TJC_PAGE_SERVICE);
            break;

        case TJC_PAGE_COMMON_SETTING_SCREEN_SLEEP:
            page_to(TJC_PAGE_SLEEP_MODE);
            break;

        case TJC_PAGE_COMMON_SETTING_UPDATE:
            go_to_update();
            break;

        case TJC_PAGE_COMMON_SETTING_RESTORE:
            page_to(TJC_PAGE_RESTORE_CONFIG);
            break;

        case TJC_PAGE_COMMON_SETTING_OOBE_OFF:
            set_mks_oobe_enabled(false);
            page_to(TJC_PAGE_COMMON_SETTING);
            break;
        
        case TJC_PAGE_COMMON_SETTING_OOBE_ON:
            set_mks_oobe_enabled(true);
            break;
        
        case TJC_PAGE_COMMON_SETTING_TO_LEVEL_MODE:
            page_to(TJC_PAGE_LEVEL_MODE);
            set_mode = "Level_mode";
            break;

        }
        break;

    case TJC_PAGE_LANGUAGE:
    case TJC_PAGE_SERVICE:
    case TJC_PAGE_SYS_OK:
    case TJC_PAGE_RESET:
    case TJC_PAGE_SLEEP_MODE:
        switch (widget_id)
        {
        case TJC_PAGE_ALL_TO_MAIN:
            if (printer_webhooks_state != "shutdown" && printer_webhooks_state != "error") {
                page_to(TJC_PAGE_MAIN);
            }
            break;

        case TJC_PAGE_ALL_TO_FILE_LIST:
            if (printer_webhooks_state != "shutdown" && printer_webhooks_state != "error") {
                go_to_file_list();
            }
            break;

        case TJC_PAGE_ALL_TO_ADJUST:
            if (printer_webhooks_state != "shutdown" && printer_webhooks_state != "error") {
                go_to_adjust();
            }
            break;

        case TJC_PAGE_ALL_TO_SETTING:
            break;
        
        case TJC_PAGE_BACK_TO_COMMON_SETTING:
            page_to(TJC_PAGE_COMMON_SETTING);
            break;

        case TJC_PAGE_RESET_PRINT_LOG:
            print_log();
            break;            
        
        case TJC_PAGE_RESET_RESTART_KLIPPER:
            reset_klipper();
            break;

        case TJC_PAGE_RESET_RESTART_FIRMWARE:
            reset_firmware();
            break;

        default:
            break;
        }
        break;

    case TJC_PAGE_UPDATE_SUCCESS:
        switch (widget_id)
        {
        case TJC_PAGE_UPDATE_SUCCESS_YES:
            finish_tjc_update();
            page_to(TJC_PAGE_MAIN);
            break;

        default:
            break;
        }
        break;

    case TJC_PAGE_PRINT_LOG_F:
    case TJC_PAGE_PRINT_LOG_S:
        switch(widget_id)
        {
        case TJC_PAGE_PRINT_LOG_YES:
            go_to_reset();
            break;
        
        default:
            break;
        }
        break;
        
    case TJC_PAGE_DETECT_ERROR:
        switch(widget_id)
        {
        case TJC_PAGE_DETECT_ERROR_YES:
            if (previous_page_id == TJC_PAGE_AUTO_MOVING || previous_page_id == TJC_PAGE_OPEN_CALIBRATE) {
                reset_klipper();
            }
            page_to(TJC_PAGE_MAIN);
            clear_previous_data();
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_GCODE_ERROR:
        switch(widget_id)
        {
        case TJC_PAGE_GCODE_ERROR_YES:
            page_to(TJC_PAGE_MAIN);
            break;
        
        default:
            break;
        }
        break;

    //4.4.2 CLL 新增息屏功能
    case TJC_PAGE_SCREEN_SLEEP:
        switch (widget_id)
        {
        case TJC_PAGE_SCREEN_SLEEP_ENTER:
            page_to(TJC_PAGE_SCREEN_SLEEP);
            if (printer_caselight_value == 1) {
                led_on_off();
                previous_caselight_value = true;
            } else {
                previous_caselight_value =false;
            }
            break;

        case TJC_PAGE_SCREEN_SLEEP_EXIT:
            if (previous_caselight_value == true) {
                led_on_off();
                previous_caselight_value = false;
            }
            if (previous_page_id == TJC_PAGE_FILE_LIST) {
                go_to_file_list();
            } else {
                page_to(previous_page_id);
                get_object_status();
            }
            break;
        
        default:
            break;
        }
        break;

    //4.4.3 CLL 修改更新按钮始终显示
    case TJC_PAGE_UPDATE_FOUND:
        switch(widget_id)
        {
        case TJC_PAGE_UPDATE_FOUND_YES:
            page_to(TJC_PAGE_UPDATING);
            disable_page_about_successed();
            start_update();
            break;
        
        case TJC_PAGE_UPDATE_FOUND_NO:
            go_to_update();
            break;

        default:
            break;
        }
        break;
        
    case TJC_PAGE_UPDATE_NOT_FOUND:
        switch(widget_id)
        {
        case TJC_PAGE_UPDATE_NOT_FOUND_YES:
            go_to_update();
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_RESTORE_CONFIG:
        switch(widget_id)
        {
        case TJC_PAGE_RESTORE_CONFIG_YES:
            restore_config();
            break;

        case TJC_PAGE_RESTORE_CONFIG_NO:
            page_to(TJC_PAGE_COMMON_SETTING);
            break;

        default:
            break;   
        }
        break;

    case TJC_PAGE_LEVEL_ERROR: // CLL 增加调平出错后特定弹窗
        switch (widget_id)
        {
        case TJC_PAGE_LEVEL_ERROR_YES:
            page_to(TJC_PAGE_MAIN);
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_MEMORY_WARNING:
        switch (widget_id)
        {
        case TJC_PAGE_MEMORY_WARNING_YES:
            page_to(TJC_PAGE_MAIN);
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_PRE_HEAT:
        switch (widget_id)
        {
        case TJC_PAGE_PRE_HEAT_SET_220:
            load_target = 220;
            if (load_mode == true) {
                page_to(TJC_PAGE_FILAMENT_POP_3);
            } else {
                page_to(TJC_PAGE_UNLOAD_MODE);
            }
            break;

        case TJC_PAGE_PRE_HEAT_SET_250:
            load_target = 250;
            if (load_mode == true) {
                page_to(TJC_PAGE_FILAMENT_POP_3);
            } else {
                page_to(TJC_PAGE_UNLOAD_MODE);
            }
            break;

        case TJC_PAGE_PRE_HEAT_SET_300:
            load_target = 300;
            if (load_mode == true) {
                page_to(TJC_PAGE_FILAMENT_POP_3);
            } else {
                page_to(TJC_PAGE_UNLOAD_MODE);
            }
            break;

        case TJC_PAGE_PRE_HEAT_BACK:
            if (printer_print_stats_state == "paused") {
                page_to(TJC_PAGE_PRINT_FILAMENT);
            } else {
                page_to(TJC_PAGE_FILAMENT);
            }
            break;
            
        default:
            break;
        }
        break;

    case TJC_PAGE_RESUME_PRINT:
        switch(widget_id) {
            case TJC_PAGE_RESUME_PRINT_YES:
                page_to(TJC_PAGE_MAIN);
                send_gcode("RESUME_INTERRUPTED\n");
                break;

            case TJC_PAGE_RESUME_PRINT_NO:
                page_to(TJC_PAGE_MAIN);
                send_gcode("CLEAR_LAST_FILE");
                break;

            default:
                break;
        }
        break;

    case TJC_PAGE_SHOW_QR:
        switch (widget_id)
        {
        case TJC_PAGE_ALL_TO_MAIN:
            if (printer_webhooks_state != "shutdown" && printer_webhooks_state != "error") {
                page_to(TJC_PAGE_MAIN);
            }
            break;

        case TJC_PAGE_ALL_TO_FILE_LIST:
            if (printer_webhooks_state != "shutdown" && printer_webhooks_state != "error") {
                go_to_file_list();
            }
            break;

        case TJC_PAGE_ALL_TO_ADJUST:
            if (printer_webhooks_state != "shutdown" && printer_webhooks_state != "error") {
                go_to_adjust();
            }
            break;
        
        case TJC_PAGE_SHOW_QR_BACK:
            page_to(TJC_PAGE_COMMON_SETTING);
            break;

        case TJC_PAGE_SHOW_QR_SET:
            go_to_server_set(0);
            break;

        case TJC_PAGE_SHOW_QR_REFRESH:
            std::cout << "################## 按下刷新按钮" << std::endl;
            qr_refreshed = false;
            go_to_showqr();
            break;

        case TJC_PAGE_SHOW_QR_WIFI:
            go_to_network();
            break;

        case TJC_PAGE_SHOW_QR_ETHERNET:
            if (mks_ethernet == 1)
                set_mks_ethernet(0);
            else 
                set_mks_ethernet(1);
            qr_refreshed = false;
            go_to_showqr();
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_SERVER_SET:
        switch (widget_id)
        {
        case TJC_PAGE_ALL_TO_MAIN:
            if (printer_webhooks_state != "shutdown" && printer_webhooks_state != "error") {
                page_to(TJC_PAGE_MAIN);
            }
            break;

        case TJC_PAGE_ALL_TO_FILE_LIST:
            if (printer_webhooks_state != "shutdown" && printer_webhooks_state != "error") {
                go_to_file_list();
            }
            break;

        case TJC_PAGE_ALL_TO_ADJUST:
            if (printer_webhooks_state != "shutdown" && printer_webhooks_state != "error") {
                go_to_adjust();
            }
            break;

        case TJC_PAGE_SERVER_SET_REFRESH:
            go_to_server_set(0);
            break;

        case TJC_PAGE_SERVER_SET_LOCAL:
            if (strcmp(status_result.wpa_state, "COMPLETED") != 0)
                break;
            if (connection_method == 1) {
                set_mks_connection_method(0);
                connection_method = 0;
                go_to_server_set(0);
                system("systemctl stop frpc.service\n");
                system("systemctl disable frpc.service\n");
            } else {
                set_mks_connection_method(1);
                connection_method = 1;
                go_to_server_set(0);
                system("systemctl enable frpc.service\n");
                system("systemctl start frpc.service\n");
            }
            break;
        
        case TJC_PAGE_SERVER_SET_BACK:
            go_to_showqr();
            break;

        case TJC_PAGE_SERVER_SET_PREVIOUS:
            if (current_server_page != 0) {
                current_server_page--;
                go_to_server_set(current_server_page);
            }
            break;

        case TJC_PAGE_SERVER_SET_NEXT:
            if (current_server_page * 4 + 4 < total_server_count) {
                current_server_page++;
                go_to_server_set(current_server_page);
            }
            break;

        case TJC_PAGE_SERVER_SET_1:
            if (current_server_page * 4 + 1 <= total_server_count) {
                update_server(current_server_page * 4 + 1);
            }
            break;

        case TJC_PAGE_SERVER_SET_2:
            if (current_server_page * 4 + 2 <= total_server_count) {
                update_server(current_server_page * 4 + 2);
            }
            break;

        case TJC_PAGE_SERVER_SET_3:
            if (current_server_page * 4 + 3 <= total_server_count) {
                update_server(current_server_page * 4 + 3);
            }
            break;
        
        case TJC_PAGE_SERVER_SET_4:
            if (current_server_page * 4 + 4 <= total_server_count) {
                update_server(current_server_page * 4 + 4);
            }
            break;
        
        default:
            break;
        }
        break;
    
    case TJC_PAGE_UPDATE_MODE:
        switch (widget_id)
        {
        case TJC_PAGE_ALL_TO_MAIN:
            if (printer_webhooks_state != "shutdown" && printer_webhooks_state != "error") {
                page_to(TJC_PAGE_MAIN);
            }
            break;

        case TJC_PAGE_ALL_TO_FILE_LIST:
            if (printer_webhooks_state != "shutdown" && printer_webhooks_state != "error") {
                go_to_file_list();
            }
            break;

        case TJC_PAGE_ALL_TO_ADJUST:
            if (printer_webhooks_state != "shutdown" && printer_webhooks_state != "error") {
                go_to_adjust();
            }
            break;

        case TJC_PAGE_UPDATE_MODE_BACK:
            page_to(TJC_PAGE_COMMON_SETTING);
            break;

        case TJC_PAGE_UPDATE_MODE_LOCAL:
            local_update();
            break;
        
        case TJC_PAGE_UPDATE_MODE_ONLINE:
            check_online_version();
            break;

        default:
            break;
        }
        break;

    case TJC_PAGE_ONLINE_UPDATE:
        switch (widget_id)
        {
        case TJC_PAGE_ALL_TO_MAIN:
            if (printer_webhooks_state != "shutdown" && printer_webhooks_state != "error") {
                page_to(TJC_PAGE_MAIN);
            }
            break;

        case TJC_PAGE_ALL_TO_FILE_LIST:
            if (printer_webhooks_state != "shutdown" && printer_webhooks_state != "error") {
                go_to_file_list();
            }
            break;

        case TJC_PAGE_ALL_TO_ADJUST:
            if (printer_webhooks_state != "shutdown" && printer_webhooks_state != "error") {
                go_to_adjust();
            }
            break;

        case TJC_PAGE_ONLINE_UPDATE_BACK:
            page_to(TJC_PAGE_COMMON_SETTING);
            break;

        case TJC_PAGE_ONLINE_UPDATE_YES:
            online_update();
            break;

        case TJC_PAGE_ONLINE_UPDATE_NO:
            go_to_update();
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_UNLOAD_MODE:
        switch (widget_id)
        {
        case TJC_PAGE_UNLOAD_MODE_MANUAL:
            page_to(TJC_PAGE_FILAMENT_POP_2);
            break;

        case TJC_PAGE_UNLOAD_MODE_AUTO:
            page_to(TJC_PAGE_AUTO_UNLOAD);
            filament_unload();
            break;

        case TJC_PAGE_UNLOAD_MODE_BACK:
            page_to(TJC_PAGE_PRE_HEAT);
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_AUTO_UNLOAD:
        switch (widget_id)
        {
        case TJC_PAGE_AUTO_UNLOAD_YES:
            send_gcode("SET_HEATER_TEMPERATURE HEATER=extruder TARGET=0\n");
            if (printer_print_stats_state == "paused") {
                page_to(TJC_PAGE_PRINT_FILAMENT);
            } else {
                page_to(TJC_PAGE_FILAMENT);
            }
            break;
            
        case TJC_PAGE_AUTO_UNLOAD_TO_LOAD:
            load_mode = true;
            page_to(TJC_PAGE_PRE_HEAT);
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_AUTO_WARNING:
        switch (widget_id)
        {
        case TJC_PAGE_AUTO_WARNING_YES:
            page_to(TJC_PAGE_AUTO_HEATERBED);
            break;
        
        default:
            break;
        }
        break;
    
    case TJC_PAGE_CALIBRATE_WARNING:
        switch (widget_id)
        {
        case TJC_PAGE_CALIBRATE_WARNING_NEXT:
            manual_count = 4;
            bed_calibrate();
            break;

        case TJC_PAGE_CALIBRATE_WARNING_BACK:
            page_to(TJC_PAGE_LEVEL_MODE);
            break;
        
        default:
            break;
        }
        break;

    default:
        break;
    }
}

void tjc_event_setted_handler(int page_id, int widget_id, unsigned char first, unsigned char second) {
    std::cout << "!!!" << page_id << std::endl;
    std::cout << "!!!" << widget_id << std::endl;
    std::cout << "!!!" << first << std::endl;
    std::cout << "!!!" << second << std::endl;
    int number = (int)((second << 8) + first);
    switch (page_id)
    {
    case TJC_PAGE_PRINTING:
        switch (widget_id)
        {
        
        case TJC_PAGE_PRINTING_EXTRUDER:
            if (number > 350) {
                number =350;
            }
            printing_keyboard_enabled = false;
            set_extruder_target(number);
            send_cmd_val(tty_fd, "n0", std::to_string(number));
            set_mks_extruder_target(number);
            break;

        case TJC_PAGE_PRINTING_HEATER_BED:
            if (number >120) {
                number =120;
            }
            printing_keyboard_enabled = false;
            set_heater_bed_target(number);
            send_cmd_val(tty_fd, "n1", std::to_string(number));
            set_mks_heater_bed_target(number);
            break;
        
        case TJC_PAGE_PRINTING_FAN_1:
            if (number > 100) {
                number = 100;
            }
            printing_keyboard_enabled = false;
            set_fan0(number);
            break;
        
        //4.4.2 CLL 新增fan2
        case TJC_PAGE_PRINTING_FAN_2:
            if (number > 100) {
                number = 100;
            }
            printing_keyboard_enabled = false;
            set_fan2(number);
            break;
        
        case TJC_PAGE_PRINTING_FAN_3:
            if (number > 100) {
                number = 100;
            }
            printing_keyboard_enabled = false;
            set_fan3(number);
            break;

        case TJC_PAGE_PRINTING_2_SPEED:
            if (number > 150) {
                number = 150;
            }
            printing_keyboard_enabled = false;
            set_printer_speed(number);
            send_cmd_val(tty_fd, "n5", std::to_string(number));
            break;

        case TJC_PAGE_PRINTING_2_FLOW:
            if (number > 150) {
                number =150;
            }
            printing_keyboard_enabled = false;
            set_printer_flow(number);
            send_cmd_val(tty_fd, "n6", std::to_string(number));
            break;
        
        case TJC_PAGE_PRINTING_HOT:
            if (number > 60) {
                number = 60;
            }
            printing_keyboard_enabled = false;
            set_hot_target(number);
            set_mks_hot_target(number);
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_FILAMENT:
        switch (widget_id)
        {
        case TJC_PAGE_FILAMENT_SET_EXTRUDER:
            if (number > 350) {
                number = 350;
            }
            set_extruder_target(number);
            set_mks_extruder_target(number);
            page_to(TJC_PAGE_FILAMENT);
            break;

        case TJC_PAGE_FILAMENT_SET_HEATERBED:
            if (number > 120) {
                number = 120;
            }
            set_heater_bed_target(number);
            set_mks_heater_bed_target(number);
            page_to(TJC_PAGE_FILAMENT);
            break;

        case TJC_PAGE_FILAMENT_SET_FAN_1:
            if (number > 100) {
                number = 100;
            }
            set_fan0(number);
            move_fan_setting = false; // CLL 收到信息后确认滑块不再被按下
            break;

        case TJC_PAGE_FILAMENT_SET_FAN_2:
            if (number > 100) {
                number = 100;
            }
            set_fan2(number);
            move_fan_setting = false;
            break;
        
        case TJC_PAGE_FILAMENT_SET_FAN_3:
            if (number > 100) {
                number = 100;
            }
            set_fan3(number);
            move_fan_setting = false;
            break;     

        case TJC_PAGE_FILAMENT_SET_HOT:
            if (number > 60) {
                number = 60;
            }
            set_hot_target(number);
            set_mks_hot_target(number);
            page_to(TJC_PAGE_FILAMENT);
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }
}

void tjc_event_keyboard(char *cmd) {
    MKSLOG("收到的键盘返回的值为 %s\n", cmd);
    MKSLOG("cmd 1  %d\n", cmd[1]);
    MKSLOG("cmd 2  %d\n", cmd[2]);
    char *psk = &cmd[3];
    switch (cmd[1]) {
    case TJC_PAGE_WIFI_LIST:
        qr_refreshed = false;
        switch (cmd[2])
        {
        case TJC_PAGE_WIFI_LIST_SSID_1:
            MKSLOG_RED("收到密码, %d, 密码是%s", strlen(psk), psk);
            printing_wifi_keyboard_enabled = false;
            page_to(TJC_PAGE_WIFI_CONNECT);
            print_ssid_psk(psk);
            break;

        case TJC_PAGE_WIFI_LIST_SSID_2:
            MKSLOG_RED("收到密码, %d, 密码是%s", strlen(psk), psk);
            printing_wifi_keyboard_enabled = false;
            page_to(TJC_PAGE_WIFI_CONNECT);
            print_ssid_psk(psk);
            break;

        case TJC_PAGE_WIFI_LIST_SSID_3:
            MKSLOG_RED("收到密码, %d, 密码是%s", strlen(psk), psk);
            printing_wifi_keyboard_enabled = false;
            page_to(TJC_PAGE_WIFI_CONNECT);
            print_ssid_psk(psk);
            break;

        case TJC_PAGE_WIFI_LIST_SSID_4:
            MKSLOG_RED("收到密码, %d, 密码是%s", strlen(psk), psk);
            printing_wifi_keyboard_enabled = false;
            page_to(TJC_PAGE_WIFI_CONNECT);
            print_ssid_psk(psk);
            break;

        case TJC_PAGE_WIFI_LIST_SSID_5:
            MKSLOG_RED("收到密码, %d, 密码是%s", strlen(psk), psk);
            printing_wifi_keyboard_enabled = false;
            page_to(TJC_PAGE_WIFI_CONNECT);
            print_ssid_psk(psk);
            break;

        default:
            break;
        }
        break;
    }
}
