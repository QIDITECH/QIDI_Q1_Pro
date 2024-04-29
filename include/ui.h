#ifndef UI_H
#define UI_H

//所有页面的下方跳转按钮的发送比特均一致
#define TJC_PAGE_ALL_TO_MAIN                0x1e
#define TJC_PAGE_ALL_TO_ADJUST              0x1f
#define TJC_PAGE_ALL_TO_FILE_LIST           0x20
#define TJC_PAGE_ALL_TO_SETTING             0x21

#define TJC_PAGE_LOGO                       0

#define TJC_PAGE_RESTART                    1

#define TJC_PAGE_SHUTDOWN                   2

#define TJC_PAGE_OPEN_LANGUAGE              3
#define TJC_PAGE_OPEN_LANGUAGE_NEXT         0x00
#define TJC_PAGE_OPEN_LANGUAGE_SKIP         0x01

#define TJC_PAGE_OPEN_POP                   4
#define TJC_PAGE_OPEN_POP_YES               0x00
#define TJC_PAGE_OPEN_POP_NO                0x01

#define TJC_PAGE_OPEN_VIDEO_1               5
#define TJC_PAGE_OPEN_VIDEO_1_NEXT          0x00

#define TJC_PAGE_OPEN_VIDEO_2               6
#define TJC_PAGE_OPEN_VIDEO_2_NEXT          0x00

#define TJC_PAGE_OPEN_WARNING               7
#define TJC_PAGE_OPEN_WARNING_NEXT          0x00

#define TJC_PAGE_OPEN_VIDEO_3               8
#define TJC_PAGE_OPEN_VIDEO_3_NEXT          0x00
#define TJC_PAGE_OPEN_VIDEO_3_UP            0x01
#define TJC_PAGE_OPEN_VIDEO_3_DOWN          0x02

#define TJC_PAGE_OPEN_HEATERBED             9
#define TJC_PAGE_OPEN_HEATERBED_ON_OFF      0x00
#define TJC_PAGE_OPEN_HEATERBED_UP          0x01
#define TJC_PAGE_OPEN_HEATERBED_DOWN        0x02
#define TJC_PAGE_OPEN_HEATERBED_NEXT        0x03

#define TJC_PAGE_OPEN_CALIBRATE             10

#define TJC_PAGE_OPEN_FILAMENTVIDEO_0       72
#define TJC_PAGE_OPEN_FILAMENTVIDEO_0_NEXT  0x00

#define TJC_PAGE_OPEN_FILAMENTVIDEO_1       11
#define TJC_PAGE_OPEN_FILAMENTVIDEO_1_NEXT  0x00

#define TJC_PAGE_OPEN_FILAMENTVIDEO_2       12
#define TJC_PAGE_OPEN_FILAMENTVIDEO_2_DOWN  0x00
#define TJC_PAGE_OPEN_FILAMENTVIDEO_2_UP    0x01
#define TJC_PAGE_OPEN_FILAMENTVIDEO_2_NEXT  0x02
#define TJC_PAGE_OPEN_FILAMENTVIDEO_2_ON_OFF 0x03

#define TJC_PAGE_OPEN_FILAMENTVIDEO_3       13
#define TJC_PAGE_OPEN_FILAMENTVIDEO_3_NEXT  0x00
#define TJC_PAGE_OPEN_FILAMENTVIDEO_3_EXTRUDE  0x01

#define TJC_PAGE_OPEN_FINISH                14
#define TJC_PAGE_OPEN_FINISH_YES            0x00

#define TJC_PAGE_MAIN                       15
#define TJC_PAGE_MAIN_CASELIGHT             0x00
#define TJC_PAGE_MAIN_BEEP                  0x01
#define TJC_PAGE_MAIN_STOP                  0x02
#define TJC_PAGE_MAIN_SET_TEMP              0x03
#define TJC_PAGE_MAIN_CACHE                 0x06

#define TJC_PAGE_FILE_LIST                  16
#define TJC_PAGE_FILE_LIST_BACK             0x00
#define TJC_PAGE_FILE_LIST_BTN_1            0x01
#define TJC_PAGE_FILE_LIST_BTN_2            0x02
#define TJC_PAGE_FILE_LIST_BTN_3            0x03
#define TJC_PAGE_FILE_LIST_BTN_4            0x04
#define TJC_PAGE_FILE_LIST_BTN_5            0x05
#define TJC_PAGE_FILE_LIST_PREVIOUS         0x0a
#define TJC_PAGE_FILE_LIST_NEXT             0x0b
#define TJC_PAGE_FILE_LIST_LOCAL            0x0c
#define TJC_PAGE_FILE_LIST_USB              0x0d

#define TJC_PAGE_PREVIEW                    17
#define TJC_PAGE_PREVIEW_BACK               0x00
#define TJC_PAGE_PREVIEW_START              0x01
#define TJC_PAGE_PREVIEW_BED_LEVELING       0x02

#define TJC_PAGE_PREVIEW_POP_1              18
#define TJC_PAGE_PREVIEW_POP_2              19
#define TJC_PAGE_PREVIEW_POP_YES            0x00
//4.4.1 CLL 耗材确认弹窗新增不再提示按钮
#define TJC_PAGE_PREVIEW_POP_NO_POP         0x01

#define TJC_PAGE_PRINTING                   20
#define TJC_PAGE_PRINTING_EXTRUDER          0x00
#define TJC_PAGE_PRINTING_HEATER_BED        0x01
#define TJC_PAGE_PRINTING_NEXT              0x02
#define TJC_PAGE_PRINTING_CASE_LIGHT        0x03
#define TJC_PAGE_PRINTING_FAN_1             0x04
#define TJC_PAGE_PRINTING_FAN_2             0x05
#define TJC_PAGE_PRINTING_FAN_3             0x06
#define TJC_PAGE_PRINTING_HOT               0x07
#define TJC_PAGE_PRINTING_PAUSE_RESUME      0x0a
#define TJC_PAGE_PRINTING_STOP              0x0b

#define TJC_PAGE_PRINTING_KB                21
#define TJC_PAGE_PRINTING_KB_BACK           0x00
#define TJC_PAGE_PRINTING_KB_PAUSE_RESUME   0x0a
#define TJC_PAGE_PRINTING_KB_STOP           0x0b

#define TJC_PAGE_PRINT_ZOFFSET              22
#define TJC_PAGE_PRINT_ZOFFSET_BACK         0x00
#define TJC_PAGE_PRINT_ZOFFSET_SET_001      0x01
#define TJC_PAGE_PRINT_ZOFFSET_SET_005      0x02
#define TJC_PAGE_PRINT_ZOFFSET_SET_01       0x03
#define TJC_PAGE_PRINT_ZOFFSET_SET_05       0x04
#define TJC_PAGE_PRINT_ZOFFSET_UP           0x05
#define TJC_PAGE_PRINT_ZOFFSET_DOWN         0x06
#define TJC_PAGE_PRINT_ZOFFSET_PAUSE_RESUME 0x0a
#define TJC_PAGE_PRINT_ZOFFSET_STOP         0x0b

#define TJC_PAGE_PRINT_FILAMENT             23
#define TJC_PAGE_PRINT_FILAMENT_ON_OFF      0x00
#define TJC_PAGE_PRINT_FILAMENT_T_UP        0x01
#define TJC_PAGE_PRINT_FILAMENT_T_DOWN      0x02
#define TJC_PAGE_PRINT_FILAMENT_LOAD        0x03
#define TJC_PAGE_PRINT_FILAMENT_UNLOAD      0x04
#define TJC_PAGE_PRINT_FILAMENT_RETRACT     0x05
#define TJC_PAGE_PRINT_FILAMENT_EXTRUDE     0x06
#define TJC_PAGE_PRINT_FILAMENT_PAUSE_RESUME 0x0a
#define TJC_PAGE_PRINT_FILAMENT_STOP        0x0b

#define TJC_PAGE_PRINTING_2              24
#define TJC_PAGE_PRINTING_2_BACK         0x00
#define TJC_PAGE_PRINTING_2_ZOFFSET      0x01
#define TJC_PAGE_PRINTING_2_SPEED        0x02
#define TJC_PAGE_PRINTING_2_FLOW         0x03
#define TJC_PAGE_PRINTING_2_PAUSE_RESUME 0x0a
#define TJC_PAGE_PRINTING_2_STOP         0x0b

#define TJC_PAGE_PRINT_FINISH               25
#define TJC_PAGE_PRINT_FINISH_YES           0x00

#define TJC_PAGE_PRINT_STOP                 26
#define TJC_PAGE_PRINT_STOP_YES             0x00
#define TJC_PAGE_PRINT_STOP_NO              0x01

#define TJC_PAGE_PRINT_STOPPING             27

#define TJC_PAGE_PRINT_NO_FILAMENT          28
#define TJC_PAGE_PRINT_NO_FILAMENT_YES      0x00

#define TJC_PAGE_PRINT_LOW_TEMP             29
#define TJC_PAGE_PRINT_LOW_TEMP_YES         0x00

#define TJC_PAGE_MOVE                       30
#define TJC_PAGE_MOVE_SET_01                0x00
#define TJC_PAGE_MOVE_SET_1                 0x01
#define TJC_PAGE_MOVE_SET_10                0x02
#define TJC_PAGE_MOVE_Z_UP                  0x03
#define TJC_PAGE_MOVE_MOTOR                 0x04
#define TJC_PAGE_MOVE_Z_DOWN                0x05
#define TJC_PAGE_MOVE_Y_UP                  0x06
#define TJC_PAGE_MOVE_Y_DOWN                0x07
#define TJC_PAGE_MOVE_X_DOWN                0x08
#define TJC_PAGE_MOVE_X_UP                  0x09
#define TJC_PAGE_MOVE_HOME                  0x0a
#define TJC_PAGE_MOVE_TO_FILAMENT           0x16

#define TJC_PAGE_MOVE_POP_1                 31
#define TJC_PAGE_MOVE_POP_1_YES             0x00

#define TJC_PAGE_MOVE_POP_2                 32
#define TJC_PAGE_MOVE_POP_2_YES             0x00
#define TJC_PAGE_MOVE_POP_2_NO              0x01

#define TJC_PAGE_FILAMENT_SET_FAN               33
#define TJC_PAGE_FILAMENT_SET_FAN_BACK          0x00
#define TJC_PAGE_FILAMENT_SET_FAN_SETTING       0x01

#define TJC_PAGE_FILAMENT_KB                    34
#define TJC_PAGE_FILAMENT_KB_BACK               0x00

#define TJC_PAGE_FILAMENT_POP_1             35
#define TJC_PAGE_FILAMENT_POP_1_YES         0x00

#define TJC_PAGE_FILAMENT_POP_2             36
#define TJC_PAGE_FILAMENT_POP_2_YES         0x00
#define TJC_PAGE_FILAMENT_POP_2_TO_LOAD     0x01
#define TJC_PAGE_FILAMENT_POP_2_NEXT        0x02
#define TJC_PAGE_FILAMENT_POP_2_BACK        0x03

#define TJC_PAGE_FILAMENT_POP_3             37
#define TJC_PAGE_FILAMENT_POP_3_YES         0x00
#define TJC_PAGE_FILAMENT_POP_3_RETRY       0x01
#define TJC_PAGE_FILAMENT_POP_3_NEXT        0x02
#define TJC_PAGE_FILAMENT_POP_3_BACK        0x03

#define TJC_PAGE_FILAMENT_UNLOAD_FINISH     38
#define TJC_PAGE_FILAMENT_UNLOAD_FINISH_YES 0x00

#define TJC_PAGE_LEVEL_MODE                 39
#define TJC_PAGE_LEVEL_MODE_AUTO_LEVEL      0x00
#define TJC_PAGE_LEVEL_MODE_SYNTONY         0x01
#define TJC_PAGE_LEVEL_MODE_BED_CALIBRATION 0x02
#define TJC_PAGE_LEVEL_MODE_TO_COMMON_SETTING 0x17
#define TJC_PAGE_LEVEL_MODE_ZOFFSET         0x18

#define TJC_PAGE_ZOFFSET                    40
#define TJC_PAGE_ZOFFSET_BACK               0x00

#define TJC_PAGE_AUTO_HEATERBED             41
#define TJC_PAGE_AUTO_HEATERBED_DOWN        0x00
#define TJC_PAGE_AUTO_HEATERBED_UP          0x01
#define TJC_PAGE_AUTO_HEATERBED_ON_OFF      0x02
#define TJC_PAGE_AUTO_HEATERBED_BACK        0x03
#define TJC_PAGE_AUTO_HEATERBED_NEXT        0x04

#define TJC_PAGE_AUTO_MOVING                42

#define TJC_PAGE_AUTO_FINISH                43
#define TJC_PAGE_AUTO_FINISH_YES            0x00

#define TJC_PAGE_PRE_BED_CALIBRATION                 44
#define TJC_PAGE_PRE_BED_CALIBRATION_SET_001         0x00
#define TJC_PAGE_PRE_BED_CALIBRATION_SET_005         0x01
#define TJC_PAGE_PRE_BED_CALIBRATION_SET_01          0x02
#define TJC_PAGE_PRE_BED_CALIBRATION_SET_05          0x03
#define TJC_PAGE_PRE_BED_CALIBRATION_UP              0x04
#define TJC_PAGE_PRE_BED_CALIBRATION_DOWN            0x05
#define TJC_PAGE_PRE_BED_CALIBRATION_ENTER           0x06

#define TJC_PAGE_BED_MOVING                 45

#define TJC_PAGE_BED_CALIBRATION            46
#define TJC_PAGE_BED_CALIBRATION_NEXT       0x00

#define TJC_PAGE_BED_FINISH                 47
#define TJC_PAGE_BED_FINISH_OK              0x00
#define TJC_PAGE_BED_FINISH_SCREW1          0x01
#define TJC_PAGE_BED_FINISH_SCREW2          0x02
#define TJC_PAGE_BED_FINISH_SCREW3          0x03
#define TJC_PAGE_BED_FINISH_Z_TILT          0x04

#define TJC_PAGE_SYNTONY_MOVE               48
#define TJC_PAGE_SYNTONY_MOVE_JUMP_OUT      0x00

#define TJC_PAGE_SYNTONY_FINISH             49
#define TJC_PAGE_SYNTONY_FINISH_YES         0x00

#define TJC_PAGE_INTERNET                   50
#define TJC_PAGE_INTERNET_REFRESH           0x00
#define TJC_PAGE_INTERNET_TO_WIFI           0x16
#define TJC_PAGE_INTERNET_TO_SETTING        0x17

#define TJC_PAGE_WIFI_LIST                  51
#define TJC_PAGE_WIFI_LIST_SSID_1           0x00
#define TJC_PAGE_WIFI_LIST_SSID_2           0x01
#define TJC_PAGE_WIFI_LIST_SSID_3           0x02
#define TJC_PAGE_WIFI_LIST_SSID_4           0x03
#define TJC_PAGE_WIFI_LIST_SSID_5           0x04
#define TJC_PAGE_WIFI_LIST_REFRESH          0x07
#define TJC_PAGE_WIFI_LIST_PREVIOUS         0x05
#define TJC_PAGE_WIFI_LIST_NEXT             0x06
#define TJC_PAGE_WIFI_LIST_TO_WIFI          0x16
#define TJC_PAGE_WIFI_LIST_TO_SETTING       0x17

#define TJC_PAGE_WIFI_CONNECT               52

#define TJC_PAGE_WIFI_SAVING                53

#define TJC_PAGE_WIFI_SUCCESS               54
#define TJC_PAGE_WIFI_SUCCESS_YES           0x00

#define TJC_PAGE_WIFI_FAILED                55
#define TJC_PAGE_WIFI_FAILED_YES            0x00

#define TJC_PAGE_WIFI_KB                    56
#define TJC_PAGE_WIFI_KB_BACK               0x00

#define TJC_PAGE_COMMON_SETTING             57
#define TJC_PAGE_COMMON_SETTING_LANGUAGE    0x00
#define TJC_PAGE_COMMON_SETTING_WIFI        0x01
#define TJC_PAGE_COMMON_SETTING_SYSTEM      0x02
#define TJC_PAGE_COMMON_SETTING_SERVICE     0x03
#define TJC_PAGE_COMMON_SETTING_SCREEN_SLEEP 0x04
#define TJC_PAGE_COMMON_SETTING_UPDATE      0x05
#define TJC_PAGE_COMMON_SETTING_RESTORE     0x06
#define TJC_PAGE_COMMON_SETTING_OOBE_OFF    0x07
#define TJC_PAGE_COMMON_SETTING_TO_LEVEL_MODE 0x16
#define TJC_PAGE_COMMON_SETTING_OOBE_ON     0x17

// CLL 以下几个页面需要xindi执行的功能相似，所以统一执行响应函数，其他功能主要由UI内置程序实现
#define TJC_PAGE_LANGUAGE                   58
#define TJC_PAGE_SYS_OK                     59
#define TJC_PAGE_RESET                      60
#define TJC_PAGE_SERVICE                    61
#define TJC_PAGE_SLEEP_MODE                 62
#define TJC_PAGE_BACK_TO_COMMON_SETTING     0x00
#define TJC_PAGE_RESET_PRINT_LOG            0x01
#define TJC_PAGE_RESET_RESTART_KLIPPER      0x02
#define TJC_PAGE_RESET_RESTART_FIRMWARE     0x03

#define TJC_PAGE_UPDATE_FOUND               63
#define TJC_PAGE_UPDATE_FOUND_YES           0x00
#define TJC_PAGE_UPDATE_FOUND_NO            0x01

#define TJC_PAGE_UPDATE_NOT_FOUND           64
#define TJC_PAGE_UPDATE_NOT_FOUND_YES       0x00

#define TJC_PAGE_UPDATE_FINISH              65

#define TJC_PAGE_UPDATE_SUCCESS             66
#define TJC_PAGE_UPDATE_SUCCESS_YES         0x00

#define TJC_PAGE_RESTORE_CONFIG             67
#define TJC_PAGE_RESTORE_CONFIG_YES         0x00
#define TJC_PAGE_RESTORE_CONFIG_NO          0x01

#define TJC_PAGE_PRINT_LOG_S                68
#define TJC_PAGE_PRINT_LOG_F                69
#define TJC_PAGE_PRINT_LOG_YES              0x00

#define TJC_PAGE_DETECT_ERROR               70
#define TJC_PAGE_DETECT_ERROR_YES           0x00

#define TJC_PAGE_GCODE_ERROR                71
#define TJC_PAGE_GCODE_ERROR_YES            0x00

//4.4.2 CLL 新增息屏功能
#define TJC_PAGE_SCREEN_SLEEP               73
#define TJC_PAGE_SCREEN_SLEEP_ENTER         0x01
#define TJC_PAGE_SCREEN_SLEEP_EXIT          0x00

#define TJC_PAGE_LEVEL_ERROR                74
#define TJC_PAGE_LEVEL_ERROR_YES            0x00

#define TJC_PAGE_FILAMENT                   75
#define TJC_PAGE_FILAMENT_SET_EXTRUDER      0x00
#define TJC_PAGE_FILAMENT_SET_HEATERBED     0x01
#define TJC_PAGE_FILAMENT_EXTRUDER_ON_OFF   0x02
#define TJC_PAGE_FILAMENT_HEATERBED_ON_OFF  0x03
#define TJC_PAGE_FILAMENT_TO_FAN            0x04
#define TJC_PAGE_FILAMENT_LOAD              0x05
#define TJC_PAGE_FILAMENT_UNLOAD            0x06
#define TJC_PAGE_FILAMENT_EXTRUDER_UP       0x07
#define TJC_PAGE_FILAMENT_EXTRUDER_DOWN     0x08
#define TJC_PAGE_FILAMENT_SET_10            0x09
#define TJC_PAGE_FILAMENT_SET_50            0x0a
#define TJC_PAGE_FILAMENT_SET_100           0x0b
#define TJC_PAGE_FILAMENT_SET_FAN_1         0x0c
#define TJC_PAGE_FILAMENT_SET_FAN_2         0x0d
#define TJC_PAGE_FILAMENT_SET_FAN_3         0x0e
#define TJC_PAGE_FILAMENT_HOT_ON_OFF        0x0f
#define TJC_PAGE_FILAMENT_SET_HOT           0x10
#define TJC_PAGE_FILAMENT_TO_FILAMENT       0x16
#define TJC_PAGE_FILAMENT_TO_MOVE           0x17

#define TJC_PAGE_PRINT_NO_FILAMENT_2        76
#define TJC_PAGE_PRINT_NO_FILAMENT_2_YES    0x00

#define TJC_PAGE_MEMORY_WARNING             77
#define TJC_PAGE_MEMORY_WARNING_YES         0x00

#define TJC_PAGE_UPDATING                   78

#define TJC_PAGE_PRE_HEAT                   79
#define TJC_PAGE_PRE_HEAT_SET_220           0x00
#define TJC_PAGE_PRE_HEAT_SET_250           0x01
#define TJC_PAGE_PRE_HEAT_SET_300           0x02
#define TJC_PAGE_PRE_HEAT_BACK              0x04

#define TJC_PAGE_RESUME_PRINT               80
#define TJC_PAGE_RESUME_PRINT_YES           0x00
#define TJC_PAGE_RESUME_PRINT_NO            0x01

#define TJC_PAGE_SHOW_QR                    81
#define TJC_PAGE_SHOW_QR_REFRESH            0x00
#define TJC_PAGE_SHOW_QR_BACK               0x01
#define TJC_PAGE_SHOW_QR_WIFI               0x02
#define TJC_PAGE_SHOW_QR_SET                0x03
#define TJC_PAGE_SHOW_QR_ETHERNET           0x04

#define TJC_PAGE_SERVER_SET                 82
#define TJC_PAGE_SERVER_SET_REFRESH         0x00
#define TJC_PAGE_SERVER_SET_BACK            0x01
#define TJC_PAGE_SERVER_SET_LOCAL           0x02
#define TJC_PAGE_SERVER_SET_PREVIOUS        0x03
#define TJC_PAGE_SERVER_SET_NEXT            0x04
#define TJC_PAGE_SERVER_SET_1               0x05
#define TJC_PAGE_SERVER_SET_2               0x06
#define TJC_PAGE_SERVER_SET_3               0x07
#define TJC_PAGE_SERVER_SET_4               0x08

#define TJC_PAGE_UPDATE_MODE                83
#define TJC_PAGE_UPDATE_MODE_BACK           0x00
#define TJC_PAGE_UPDATE_MODE_LOCAL          0x01
#define TJC_PAGE_UPDATE_MODE_ONLINE         0x02

#define TJC_PAGE_ONLINE_UPDATE              84
#define TJC_PAGE_ONLINE_UPDATE_BACK         0x00
#define TJC_PAGE_ONLINE_UPDATE_YES          0x01
#define TJC_PAGE_ONLINE_UPDATE_NO           0x02

#define TJC_PAGE_SEARCH_SERVER              85

#define TJC_PAGE_UNLOAD_MODE                86
#define TJC_PAGE_UNLOAD_MODE_MANUAL         0x00
#define TJC_PAGE_UNLOAD_MODE_AUTO           0x01
#define TJC_PAGE_UNLOAD_MODE_BACK           0x02

#define TJC_PAGE_AUTO_UNLOAD                87
#define TJC_PAGE_AUTO_UNLOAD_TO_LOAD        0x00
#define TJC_PAGE_AUTO_UNLOAD_YES            0x01

#define TJC_PAGE_OPEN_LANGUAGE2             88

#define TJC_PAGE_LANGUAGE2                  89

#define TJC_PAGE_INSTALLING                 90

#define TJC_PAGE_AUTO_WARNING               91
#define TJC_PAGE_AUTO_WARNING_YES           0x00

#define TJC_PAGE_CALIBRATE_WARNING          92
#define TJC_PAGE_CALIBRATE_WARNING_NEXT     0x00
#define TJC_PAGE_CALIBRATE_WARNING_BACK     0x01

void parse_cmd_msg_from_tjc_screen(char *cmd);
void page_to(int page_id);
void tjc_event_clicked_handler(int page_id, int widget_id, int type_id);
void tjc_event_setted_handler(int page_id, int widget_id, unsigned char first, unsigned char second);
void tjc_event_keyboard(char *cmd);

#endif
