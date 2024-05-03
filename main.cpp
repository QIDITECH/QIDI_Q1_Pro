#include <fstream>


#include <wpa_ctrl.h>

#include "include/MakerbaseClient.h"
#include "include/MoonrakerAPI.h"
#include "include/MakerbaseIPC.h"
#include "include/MakerbaseSerial.h"
#include "include/MakerbaseParseMessage.h"
#include "include/MakerbasePanel.h"
#include "include/MakerbaseParseIni.h"
#include "include/MakerbaseWiFi.h"
#include "include/MakerbaseNetwork.h"

#include "include/KlippyGcodes.h"

#include "include/mks_log.h"
#include "include/mks_preview.h"
#include "include/mks_init.h"
#include "include/mks_gpio.h"
#include "include/mks_update.h"
#include "include/mks_wpa_cli.h"
#include "include/mks_test.h"
#include "include/send_jpg.h"

// #include <gtk/gtk.h>

#include "include/ui.h"
#include "include/send_msg.h"
// #include "include/receive_msg.h"
// #include "include/printer.h"
// #include "include/systeminfo.h"
#include "include/KlippyRest.h"
#include "include/event.h"

extern int tty_fd;
extern int current_page_id;
extern int previous_page_id;

MakerbaseClient *ep;

extern std::string serial_by_id;
extern std::string str_gimage;

bool is_download_to_screen = false;

bool find_screen_tft_file = false;

// extern struct wpa_ctrl *mon_conn;

int main(int argc, char** argv) {


	DIR *dir;
	struct dirent *entry;

	dir = opendir("/dev");

	if (dir == NULL) {
		perror("无法打开目录 /dev");
	}

	while ((entry = readdir(dir))) {
		if (strstr(entry->d_name, "/dev/sd") == entry->d_name) {
			if (strlen(entry->d_name) >= 8) {
				char *partition_suffix = entry->d_name + 7;
				if (partition_suffix[1] == '1') {
					char command[256];
                    snprintf(command, sizeof(command), "/usr/bin/systemctl --no-block restart makerbase-automount@%s.service", partition_suffix);
					system(command);
				}
			}
		}
	}

	// getIPAddress();
	/*
	if (access("/dev/sda", F_OK) == 0) {
		if (access("/dev/sda1", F_OK) == 0) {
			if (access("/home/mks/gcode_files/sda1", F_OK) != 0) {
				system("/usr/bin/systemctl --no-block restart makerbase-automount@sda1.service");
			}
		}
    }
	*/

    if (access("/home/mks/gcode_files/sda1/mksscreen.recovery", F_OK) == 0) {
        system("cp /home/mks/gcode_files/sda1/mksscreen.recovery /root/800_480.tft; sync");
    }

    if (access("/home/mks/gcode_files/sda1/mksclient.recovery", F_OK) == 0) {
        system("dpkg -i /home/mks/gcode_files/sda1/mksclient.recovery; sync");
    }

	if (access("/root/800_480.tft", F_OK) == 0) {
		find_screen_tft_file = true;
		MKSLOG_BLUE("找到tft升级文件");
	} else {
		find_screen_tft_file = false;
		MKSLOG_BLUE("没有找到tft升级文件");
	}
	/*
	if (set_GPIO1_C5_high() == 0) {
		MKSLOG("GPIO1_C5拉高成功");
	} else {
		MKSLOG("GPIO1_C5拉高失败");
	}

	if (init_GPIO1_B2() == 0) {
		MKSLOG("初始化GPIO1_B2成功");
	} else {
		MKSLOG("初始化GPIO1_B2失败");
	}

	if (init_GPIO1_C3() == 0) {
		MKSLOG("GPIO1_C3初始化成功");
	} else {
		MKSLOG("GPIO1_C3初始化失败");
	}
	*/
	if (find_screen_tft_file == true) {
		MKSLOG("执行移动程序");
		system("/root/uart; mv /root/800_480.tft /root/800_480.tft.bak");
	}

    if (access("/home/mks/gcode_files/sda1/QD_factory_mode.txt", F_OK) == 0) {
		system("dmesg > /home/mks/gcode_files/sda1/mks-dmesg.log; sync; ");
		if (access("/home/mks/gcode_files/sda1/mks-super.sh", F_OK) == 0) {
			system("bash /home/mks/gcode_files/sda1/mks-super.sh");
		}
	}

	//4.4.15 CCW 添加开机检测c_helper.so文件
    const char *sourceFile = "/home/mks/klipper/klippy/chelper/c_helper.so";
    const char *destFile = "/root/etc/c_helper.so";

    std::ifstream file(sourceFile, std::ifstream::binary | std::ifstream::ate);
    long size = 0; 

    if (!file) {
        std::printf("File does not exist or could not open file: %s\n", sourceFile);
        size = -1; 
    } else {
        size = file.tellg(); 
    }
    file.close();

    // 检查c_helper.so文件是否正常
    if (size < 10240) {
        std::printf("File %s does not exist or is less than 10KB\n", sourceFile);
        
        std::ifstream src(destFile, std::ios::binary);
        std::ofstream dst(sourceFile, std::ios::binary);
        
        if (!src || !dst) {
            std::printf("Failed to open source or destination file\n");
        } else {
            dst << src.rdbuf(); // Copy file content
            std::printf("File %s has been replaced with %s\n", sourceFile, destFile);
        }
    } else {
        std::printf("File %s exists and is not less than 10KB, no action taken\n", sourceFile);
    }

	// pthread_t monitor_thread;
	// pthread_t monitor_C3_thread;
	pthread_t wpa_recv_thread;

	// pthread_t test_thread;

	// pthread_create(&monitor_thread, NULL, monitor_GPIO1_B2, NULL);
	// pthread_create(&monitor_C3_thread, NULL, monitor_GPIO1_C3, NULL);
	pthread_create(&wpa_recv_thread, NULL, mks_wifi_hdlevent_thread, NULL);


	bool done = false;


	pthread_t tid_msg_parse;
    pthread_t sent_jpg_thread;
	std::string host = "localhost";
	std::string url = "ws://localhost:7125/websocket?";
	MKSLOG("%s", url.data());

	if (argc == 2) {
		host = argv[1];
		url = "ws://" + host + ":7125/websocket?";
	}

	ep = new MakerbaseClient(host, "7125");
	std::cout << ep->GetURL() << std::endl;
	std::cout << ep->GetStatus() << std::endl;
	std::cout << ep->GetIsConnected() << std::endl;

	int fd;			// 串口文件描述符
	char buff[4096];
	int count;

	bool start_connect = false;

	int connected_count = 0;
	
	while (!ep->GetIsConnected()) {
		std::cout << connected_count << "状态未连接: " << ep->GetIsConnected() << std::endl;
		ep->Close();
		ep->Connect(url);
		connected_count++;
		sleep(1);
		ep->GetStatus();
		sleep(1);
	}
	
	pthread_create(&tid_msg_parse, NULL, json_parse, NULL);
    pthread_create(&sent_jpg_thread, NULL, sent_jpg_thread_handle, NULL);   // 创建预览图线程

	if ((fd = open("/dev/ttyS1", O_RDWR | O_NDELAY | O_NOCTTY)) < 0) {
		printf("Open tty failed\n");
	} else {
		MKSLOG_BLUE("%d", tty_fd);
		tty_fd = fd;
		printf("Open tty success\n");
		set_option(fd, 115200, 8, 'N', 1);
		try
		{
			fcntl(fd, F_SETFL, FNDELAY);

			get_total_time();
			sleep(2);
			sub_object_status();									// 订阅相关的参数

			sleep(2);

			get_object_status();									// 主动获取必备的参数
			sleep(2);
			init_mks_status();
			get_wlan0_status();
            mks_wpa_scan_scanresults();
            get_ssid_list_pages();
			mks_get_version();
			sleep(3);

            send_cmd_val(tty_fd, "logo.version", "19"); // CLL 检测UI与SOC版本是否匹配4.4.19版本输出标记数字为19
            if (find_screen_tft_file == false) {
				previous_page_id = TJC_PAGE_LOGO;
				if (get_mks_oobe_enabled() == true) {
					current_page_id = TJC_PAGE_OPEN_LANGUAGE;
				} else {
					current_page_id = TJC_PAGE_MAIN;
				}
			} else {
				current_page_id = TJC_PAGE_UPDATE_SUCCESS;
			}
			page_to(current_page_id);

		}
		catch(const std::exception& e)
		{
			std::cerr << "Page main error, " << e.what() << '\n';
		}
	}

	while(1) {
		refresh_page_show(); // CLL 修复刷新函数冲突问题,修改后xindi在同一时段内只能处理一条屏幕返回信息，若同时多条信息输入则会造成丢失现象(尤其是刷新函数错误使屏幕返回变量名称无效信息，将会导致大量无效输入)
		if ((count = read(fd, buff, sizeof(buff))) > 0) {
			char *cmd = buff;
			parse_cmd_msg_from_tjc_screen(cmd);
			memset(buff, 0, sizeof(buff));
		}
		usleep(1000);
	}
	close(fd);
	return 0;
}
