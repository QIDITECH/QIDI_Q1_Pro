#include <iostream>

#include "../include/ui.h"
#include "../include/MakerbaseWiFi.h"
#include "../include/mks_wpa_cli.h"
#include "../include/mks_log.h"
#include "../include/event.h"

extern int current_page_id;                    // 当前页面的id号

extern struct mks_wifi_status_t wpa_status;

std::string str_scan_results;
char mks_wifi_scan_results[4096];
char mks_wifi_status[512];

static struct wpa_ctrl *ctrl_conn;      // 控制连接wifi的wpa_ctrl
static struct wpa_ctrl *mon_conn;       // 用于监听wifi的wpa_ctrl

static const char *client_socket_dir = NULL;

bool mks_wpa_cli_connected = false;

std::string wlan_state_str = "";

extern std::string wifi_ip_address;

extern bool page_wifi_list_ssid_button_enabled[4];
extern int page_wifi_current_pages;
extern int page_wifi_ssid_list_pages;

static void wpa_cli_msg_cb(char *msg, size_t len)
{
	printf("%s\n", msg);
}

static int result_get(char *str, char *key, char *val, int val_len) {
    char *s;

    if (!(s = strstr(str, key))) {
        return -1;
    }

    if (!(s = strchr(s, '='))) {
        return -1;
    }

    s++;

    while (*s != '\n' && *s != '\0' && val_len > 1) {
        *val++ = *s++;
        val_len--;
    }
    *val = '\0';
    return 0;
}

/**
 * 查询已连接ap的信号状态
 * @result: 查询结果
 * @ret: 0: 成功, -1: 失败
 * 
 */
int mks_wifi_run_cmd_signal_poll(struct mks_wifi_signal_poll_result_t *result) {
    int ret;
    char val[512];
    char *ack = result->ack;
    size_t len = sizeof(result->ack);
    char const *cmd = "SIGNAL_POLL";

    char item[16];

    memset(result, 0x00, sizeof(struct mks_wifi_signal_poll_result_t));

    ret = mks_wifi_run_cmd(cmd, ack, &len);

    if (ret < 0 || len == 0) {
        ret = -1;
        printf("Err: %s\n", cmd);
        return -1;
    }


    if (strncmp(ack, "FAIL", 4) == 0) {
        printf("Err: %s (FAIL)\n", cmd);
    }

    memset(item, 0x00, sizeof(item));
    sprintf(item, "RSSI");
    if (result_get(ack, item, val, sizeof(val)) == 0) {
        result->rssi = strtol(val, NULL, 10);
    }

    memset(item, 0x00, sizeof(item));
    sprintf(item, "LINKSPEED");
    if (result_get(ack, item, val, sizeof(val)) == 0) {
        result->linkspeed = strtol(val, NULL, 10);
    }

    memset(item, 0x00, sizeof(item));
    sprintf(item, "FREQUENCY");
    if (result_get(ack, item, val, sizeof(val)) == 0) {
        result->frequency = strtol(val, NULL, 10);
    }

    memset(item, 0x00, sizeof(item));
    sprintf(item, "NOISE");
    if (result_get(ack, item, val, sizeof(val)) == 0) {
        result->noise = strtol(val, NULL, 10);
    }

    return ret;
}

int mks_wifi_run_cmd(char const *cmd, char *result, size_t *len) {
    struct wpa_ctrl *wpa_ctrl = wpa_ctrl_open("/var/run/wpa_supplicant/wlan0");

    if (!wpa_ctrl) {
        printf("Err: wpa_ctrl_open()\n");
        return -1;
    }

    int ret = wpa_ctrl_request(wpa_ctrl, cmd, strlen(cmd), result, len, 0);
    result[*len] = 0;

    wpa_ctrl_close(wpa_ctrl);

    return ret;
}

int mks_wifi_run_cmd_status(struct mks_wifi_status_result_t *result) {
    int ret;
    char val[512];
    char *ack = result->ack;
    size_t len = sizeof(result->ack);
    char const *cmd = "STATUS";

    char item[16];

    memset(result, 0x00, sizeof(struct mks_wifi_status_result_t));

    ret = mks_wifi_run_cmd(cmd, ack, &len);

    if (ret < 0 || len == 0) {
        printf("Err: %s\n", cmd);
        return ret;
    }

    if (strncmp(ack, "FAIL", 4) == 0) {
        printf("Err: %s (FAIL)\n", cmd);
    }

    memset(item, 0x00, sizeof(item));
    sprintf(item, "bssid");
    result_get(ack, item, result->bssid, sizeof(result->bssid));
	
    memset(item, 0x00, sizeof(item));
    sprintf(item, "freq");
	if (result_get(ack, item, val, sizeof(val)) == 0) {
		result->freq = strtol(val, NULL, 10);
	}

    memset(item, 0x00, sizeof(item));
    sprintf(item, "ssid");
    char *nack = ack;
    nack=nack+5;
    result_get(nack, item, result->ssid, sizeof(result->ssid));

    memset(item, 0x00, sizeof(item));
    sprintf(item, "id");
    if (result_get(ack, item, val, sizeof(val)) == 0) {
        result->id = strtol(val, NULL, 10);
    }

    memset(item, 0x00, sizeof(item));
    sprintf(item, "mode");
    result_get(ack, item, result->mode, sizeof(result->mode));

    memset(item, 0x00, sizeof(item));
    sprintf(item, "pairwise_cipher");
    result_get(ack, item, result->pairwise_cipher, sizeof(result->pairwise_cipher));

    memset(item, 0x00, sizeof(item));
    sprintf(item, "group_cipher");
    result_get(ack, item, result->group_cipher, sizeof(result->group_cipher));

    memset(item, 0x00, sizeof(item));
    sprintf(item, "key_mgmt");
    result_get(ack, item, result->key_mgmt, sizeof(result->key_mgmt));

    memset(item, 0x00, sizeof(item));
    sprintf(item, "wpa_state");
    result_get(ack, item, result->wpa_state, sizeof(result->wpa_state));

    memset(item, 0x00, sizeof(item));
    sprintf(item, "ip_address");
    result_get(ack, item, result->ip_address, sizeof(result->ip_address));
    wifi_ip_address = result->ip_address;

    memset(item, 0x00, sizeof(item));
    sprintf(item, "address");
    result_get(ack, item, result->address, sizeof(result->address));

    memset(item, 0x00, sizeof(item));
    sprintf(item, "uuid");
    result_get(ack, item, result->uuid, sizeof(result->uuid));

    return ret;
}

/**
 * 修改wifi配置
 * 
 */
/*
void mks_wifi_set_config(char *ssid, char *psk) {
    char buf[1024];
    int len;
    char *path = "/etc/wpa_supplicant/wpa_supplicant-wlan0.conf";

    int fd = open(path, O_CREAT | O_TRUNC | O_WRONLY);

    len = sprintf(buf, "ctrl_interface=/var/run/wpa_supplicant\nnetwork={\nssid=\"%s\"\npsk=\"%s\"\n}\n", ssid, psk);

    write(fd, buf, len);

    fsync(fd);
    close(fd);
}
*/

/**
 * 获取wifi配置
 * 
 */
/*
int mks_wifi_get_config(char *ssid, char *psk) {
    char buf[1024];
    // int len;
    char *path = "/etc/wpa_supplicant/wpa_supplicant-wlan0.conf";

    memset(buf, 0x00, sizeof(buf));

    int fd = open(path, O_RDONLY);

    char item[16];

    if (fd < 0) {
        return -1;
    }

    read(fd, buf, 1024);
    close(fd);

    char val[512];

    memset(item, 0x00, sizeof(item));
    sprintf(item, "ssid");
    if (result_get(buf, item, val, sizeof(val)) < 0) {
        return -1;
    }

    strcpy(ssid, val + 1);
    ssid[strlen(ssid) - 1] = '\0';

    memset(item, 0x00, sizeof(item));
    sprintf(item, "psk");
    if (result_get(buf, item, val, sizeof(val)) < 0) {
        return -1;
    }

    strcpy(psk, val + 1);
    psk[strlen(psk) - 1] = '\0';

    return 0;
}
*/

void test_wifi_run_cmd_signal(void) {
    struct mks_wifi_signal_poll_result_t result;
    mks_wifi_run_cmd_signal_poll(&result);

    printf("%s: ack: \n%s", __FUNCTION__, result.ack);
    printf("%s: rssi: %d\n", __FUNCTION__, result.rssi);
    printf("%s: linkspeed: %d\n", __FUNCTION__, result.linkspeed);
    printf("%s frequency: %d\n", __FUNCTION__, result.frequency);
    printf("%s noise: %d\n", __FUNCTION__, result.noise);
}

void test_wifi_run_cmd_status(void) {
    // std::cout << "TEST STATUS" << std::endl;
    struct mks_wifi_status_result_t result;
    mks_wifi_run_cmd_status(&result);

    printf("%s: ack:\n%s", __FUNCTION__, result.ack);
	printf("%s: bssid:%s\n", __FUNCTION__, result.bssid);
	printf("%s: freq:%d\n", __FUNCTION__, result.freq);
	printf("%s: ssid:%s\n", __FUNCTION__, result.ssid);
	printf("%s: id:%d\n", __FUNCTION__, result.id);
	printf("%s: mode:%s\n", __FUNCTION__, result.mode);
	printf("%s: pairwise_cipher:%s\n", __FUNCTION__, result.pairwise_cipher);
	printf("%s: group_cipher:%s\n", __FUNCTION__, result.group_cipher);
	printf("%s: key_mgmt:%s\n", __FUNCTION__, result.key_mgmt);
	printf("%s: wpa_state:%s\n", __FUNCTION__, result.wpa_state);
	printf("%s: ip_address:%s\n", __FUNCTION__, result.ip_address);
	printf("%s: address:%s\n", __FUNCTION__, result.address);
	printf("%s: uuid:%s\n", __FUNCTION__, result.uuid);
}


// int mks_wifi_connect_ap_fun(char *ssid, char *psk) {
//     char au8ReplyBuf[2048] = {"\0"};
//     size_t reply_len;
//     int ret;
//     int i;
//     int s32NetId = -1;
//     char au8Path[128] = {"\0"};

//     /* wpa_ctrl_open */
//     sprintf(au8Path, "/var/run/wpa_supplicant/wlan0");
//     g_pstWpaCtrl = wpa_ctrl_open(au8Path);
//     if (g_pstWpaCtrl == NULL) {
//         printf("wpa_ctrl_open failed: %s!\n");
//         return -1;
//     }

//     /* wpa_ctrl_request ADD_NETWORK */
//     /*
//     memset(au8ReplyBuf, '\0', sizeof(au8ReplyBuf));
//     reply_len = sizeof(au8ReplyBuf) - 1;
//     ret = wpa_ctrl_request(g_pstWpaCtrl, "ADD_NETWORK", strlen("ADD_NETWORK"), au8ReplyBuf, &reply_len, wpa_cli_msg_cb);
//     if (ret == 0) {
//         au8ReplyBuf[reply_len] = '\0';
//     }

//     s32NetId = atoi(au8ReplyBuf);
//     */

//     /* wpa_ctrl_request SET_NETWORK */
//     char au8SsidBuf[128] = {"\0"};
//     // snprintf(au8SsidBuf, sizeof(au8SsidBuf) - 1, "SET_NETWORK %d ssid \"%s\"", s32NetId, ssid);
//     snprintf(au8SsidBuf, sizeof(au8SsidBuf) - 1, "SET_NETWORK 0 ssid \"%s\"", ssid);
//     memset(au8ReplyBuf, 0x00, sizeof(au8ReplyBuf));
//     reply_len = sizeof(au8ReplyBuf) - 1;
//     ret = wpa_ctrl_request(g_pstWpaCtrl, au8SsidBuf, strlen(au8SsidBuf), au8ReplyBuf, &reply_len, wpa_cli_msg_cb);
//     if (ret == 0) {
//         au8ReplyBuf[reply_len] = '\0';
//         printf("Reply_len: %s", au8ReplyBuf);
//     } else {
//         return -1;
//     }

//     /* wpa_ctrl_request SET_NETWORK */
//     char au8PskBuf[128] = {"\0"};
//     // snprintf(au8PskBuf, sizeof(au8PskBuf) - 1, "SET_NETWORK %d psk \"%s\"", s32NetId, psk);
//     snprintf(au8PskBuf, sizeof(au8PskBuf) - 1, "SET_NETWORK 0 psk \"%s\"", psk);
//     memset(au8PskBuf, 0x00, sizeof(au8ReplyBuf));
//     reply_len = sizeof(au8ReplyBuf) - 1;
//     ret = wpa_ctrl_request(g_pstWpaCtrl, au8PskBuf, strlen(au8PskBuf), au8ReplyBuf, &reply_len, wpa_cli_msg_cb);
//     if (ret == 0) {
//         au8ReplyBuf[reply_len] = '\0';
//         printf("au8ReplyBuf: %s\n", au8ReplyBuf);
//     } else {
//         return -1;
//     }

//     /* wpa_ctrl_request ENABLE_NETWORK */
//     char au8EnableBuf[64] = {"\0"};
//     // snprintf(au8EnableBuf, sizeof(au8EnableBuf) - 1, "ENABLE_NETWORK %d", s32NetId);
//     snprintf(au8EnableBuf, sizeof(au8EnableBuf) - 1, "ENABLE_NETWORK 0");
//     ret = wpa_ctrl_request(g_pstWpaCtrl, au8EnableBuf, strlen(au8EnableBuf), au8ReplyBuf, &reply_len, wpa_cli_msg_cb);
//     if (ret == 0) {
//         au8ReplyBuf[reply_len] = '\0';
//         printf("au8ReplyBuf: %s", au8ReplyBuf);
//     } else {
//         return -1;
//     }

//     wpa_ctrl_close(g_pstWpaCtrl);

//     return ret;
// }

void *mks_wifi_hdlevent_thread(void *arg) {
    //4.4.3 修复WiFi
	int flag = 0;
	int t = 0;
    char path[64] = {"\0"};
    sprintf(path, "/var/run/wpa_supplicant/wlan0");

    while (1)
    {
        if (access(path, F_OK) == 0) {
            break;
        }
        usleep(100000);
    }

    mon_conn = wpa_ctrl_open(path);
    
    
    if (!mon_conn) {
        MKSLOG_YELLOW("监听wpa线程失败");
        pthread_exit(NULL);
    } else {
        MKSLOG_YELLOW("开启mon_conn成功");
        wpa_ctrl_attach(mon_conn);
    }
    

    while(1) {
        if (wpa_ctrl_pending(mon_conn) > 0) {
            char buf[4096];
            size_t len = sizeof(buf) - 1;
            if (wpa_ctrl_recv(mon_conn, buf, &len) == 0) {
                buf[len] = '\0';
                MKSLOG_YELLOW("收到wpa回调信息: \n%s", buf);
                if (strstr(buf, "CTRL-EVENT-SCAN-RESULTS") != NULL) {
                    MKSLOG_BLUE("已获取到了扫描的结果");
                } else if (strstr(buf, "CTRL-EVENT-DISCONNECTED") != NULL) {
                    MKSLOG_BLUE("已经断开wifi链接");
                    MKSLOG_BLUE("尝试连接新的wifi");
                    wlan_state_str = "disconnected";
                    mks_enable_network();
					flag = 0;
                    t = 0;
                } else if (strstr(buf, "CTRL-EVENT-CONNECTED") != NULL) {
                    MKSLOG_BLUE("已经成功连接上wifi");
                    //4.4.1 CLL 修复WiFi刷新
                    mks_enable_network();
                    usleep(10000);
                    wlan_state_str = "connected";
                    //system("dhcpcd wlan0");
                    if (current_page_id == TJC_PAGE_WIFI_CONNECT) {
                        page_to(TJC_PAGE_WIFI_SUCCESS);
                    }

                } else if (strstr(buf, WPS_EVENT_AP_AVAILABLE) != NULL) {
                    MKSLOG("Available WPS AP found in scan results.");
					//4.4.3 CLL 修复WiFi
                    if (current_page_id == TJC_PAGE_WIFI_CONNECT&&flag==0){     //pwtest:在flag为0且在连接界面时，执行网络选择，且将flag置为2特殊状态值
                        system("wpa_cli select_network 0");
                        flag=2;
                    }
                } else if (strstr(buf, "pre-shared key may be incorrect") != NULL) {
                    if (current_page_id == TJC_PAGE_WIFI_CONNECT) {
                        page_to(TJC_PAGE_WIFI_FAILED);
                    }
                //4.4.3 CLL 修复WiFi刷新
                } else if (strstr(buf, "CONN_FAILED") != NULL || strstr(buf,"timed out") != NULL || strstr(buf,"WRONG_KEY") != NULL) {
                    if (current_page_id == TJC_PAGE_WIFI_CONNECT) {
                        page_to(TJC_PAGE_WIFI_FAILED);
						flag = 0;
						t = 0;
                    }
                } else if (strstr(buf, "Associated with") != NULL) {
                    MKSLOG_RED("握手握手握手");
			     }else if (strstr(buf,"NETWORK-NOT-FOUND") && current_page_id == TJC_PAGE_WIFI_CONNECT && flag==2) {   //4.4.3 CLL 修复WiFi
                    if (t<3) {
                        t++;
                    } else if (t==3) {
                        page_to(TJC_PAGE_WIFI_FAILED);
                        system("wpa_cli disable_network 0");
                        flag=0;
                        t=0;
                    }
                }
            }
        } else {
            usleep(10000);
        }
    }
    // wpa_ctrl_close(mon_conn);
    // pthread_exit(NULL);
}

int mks_wpa_scan_scanresults() {
    //4.4.1 CLL 修复WiFi刷新
    char path[64] = {"\0"};

    sprintf(path, "/var/run/wpa_supplicant/wlan0");
    ctrl_conn = wpa_ctrl_open(path);

    if (!ctrl_conn) {
        printf("Open wpa control interfaces failed!\n");
        return -3;
    } else {
        printf("Successful!\n");
    }

    char replyBuff[4096] = {"\0"};
    size_t reply_len;

    int ret;
    reply_len = sizeof(replyBuff) - 1;
    ret = wpa_ctrl_request(ctrl_conn, "SCAN", strlen("SCAN"), replyBuff, &reply_len, NULL);
    if (ret == -2) {
        printf("Command timed out.\n");
        // wpa_ctrl_close(ctrl_conn);
        return ret;
    } else if (ret < 0) {
        printf("Command failed.\n");
        // wpa_ctrl_close(ctrl_conn);
        return ret;
    } else if (ret == 0) {
        replyBuff[reply_len] = '\0';
        printf("回收到的消息: \n%s\n", replyBuff);
        if (strncmp(replyBuff, "OK", 2) == 0) {
            MKSLOG_YELLOW("扫描结果: %s", replyBuff);
            sleep(2);
        }
    }

    memset(replyBuff, 0x00, sizeof(replyBuff));
    reply_len = sizeof(replyBuff) - 1;
    ret = wpa_ctrl_request(ctrl_conn, "SCAN_RESULTS", strlen("SCAN_RESULTS"), replyBuff, &reply_len, NULL);
    

    if (ret == -2) {
        MKSLOG_RED("Command timed out.");
        // wpa_ctrl_close(ctrl_conn);
        return ret;
    } else if (ret < 0) {
        MKSLOG_RED("Command failed.");
        // wpa_ctrl_close(ctrl_conn);
        return ret;
    } else if (ret == 0) {
        replyBuff[reply_len] = '\0';
        // MKSLOG_YELLOW("扫描结果: %s", replyBuff);
        str_scan_results = replyBuff;
        // split_scan_result(str_scan_results);
        // std::cout << str_scan_results << std::endl;
        MKSLOG("%s", str_scan_results.c_str());
        // char result[4096] = {0};
        // execute_cmd(convertStr.c_str(), result);
        // MKSLOG("%s", result.c_str());
        // str_scan_results = result;
        // split_scan_result(str_scan_results);
        parse_scan_results(replyBuff);
    }

    // wpa_ctrl_close(ctrl_conn);
    return ret;
}

/*
int mks_wifi_connect(char *ssid, char *psk) {
    // char path[64] = {"\0"};

    char cmd[64];
    // char ssidBuff[64];
    // char pskBuff[64];

    char replyBuff[4096] = {"\0"};
    size_t reply_len;
    int ret;

    // sprintf(path, "/var/run/wpa_supplicant/wlan0");
    // ctrl_conn = wpa_ctrl_open(path);

    if (!ctrl_conn) {
        printf("Open wpa control interfaces failed!\n");
        return -3;
    } else {
        printf("Successful!\n");
    }

    memset(cmd, 0x00, sizeof(cmd));
    snprintf(cmd, sizeof(cmd) - 1, "SET_NETWORK 0 ssid \"%s\"", ssid);
    // snprintf(cmd, sizeof(cmd) - 1, "SET_NETWORK 0 ssid \"MENSON-WIFI\"");
    memset(replyBuff, 0x00, sizeof(replyBuff));
    reply_len = sizeof(replyBuff) - 1;
    ret = wpa_ctrl_request(ctrl_conn, cmd, strlen(cmd), replyBuff, &reply_len, wpa_cli_msg_cb);
    if (ret == -2) {
        MKSLOG_RED("Command timed out.");
        // wpa_ctrl_close(ctrl_conn);
        return ret;
    } else if (ret < 0) {
        MKSLOG_RED("Command failed.");
        // wpa_ctrl_close(ctrl_conn);
        return ret;
    } else if (ret == 0) {
        replyBuff[reply_len] = '\0';
        MKSLOG_YELLOW("返回的消息: %s", replyBuff);
    }

    memset(cmd, 0x00, sizeof(cmd));
    snprintf(cmd, sizeof(cmd) - 1, "SET_NETWORK 0 psk \"%s\"", psk);
    // snprintf(cmd, sizeof(cmd) - 1, "SET_NETWORK 0 psk \"makerbase318\"");
    memset(replyBuff, 0x00, sizeof(replyBuff));
    reply_len = sizeof(replyBuff) - 1;
    ret = wpa_ctrl_request(ctrl_conn, cmd, strlen(cmd), replyBuff, &reply_len, wpa_cli_msg_cb);
    if (ret == -2) {
        MKSLOG_RED("Command timed out.");
        // wpa_ctrl_close(ctrl_conn);
        return ret;
    } else if (ret < 0) {
        MKSLOG_RED("Command failed.");
        return ret;
    } else if (ret == 0) {
        replyBuff[reply_len] = '\0';
        MKSLOG_YELLOW("返回的消息: %s", replyBuff);
    }

    memset(cmd, 0x00, sizeof(cmd));
    snprintf(cmd, sizeof(cmd) - 1, "DISABLE_NETWORK 0");
    memset(replyBuff, 0x00, sizeof(replyBuff));
    reply_len = sizeof(replyBuff) - 1;
    ret = wpa_ctrl_request(ctrl_conn, cmd, strlen(cmd), replyBuff, &reply_len, wpa_cli_msg_cb);
    if (ret == -2) {
        MKSLOG_RED("Command timed out.");
        // wpa_ctrl_close(ctrl_conn);
        return ret;
    } else if (ret < 0) {
        MKSLOG_RED("Command failed.");
        // wpa_ctrl_close(ctrl_conn);
    } else if (ret == 0) {
        replyBuff[reply_len] = '\0';
        MKSLOG_YELLOW("返回的消息: %s", replyBuff);
    }

    memset(cmd, 0x00, sizeof(cmd));
    snprintf(cmd, sizeof(cmd) - 1, "ENABLE_NETWORK 0");
    memset(replyBuff, 0x00, sizeof(replyBuff));
    reply_len = sizeof(replyBuff) - 1;
    ret = wpa_ctrl_request(ctrl_conn, cmd, strlen(cmd), replyBuff, &reply_len, wpa_cli_msg_cb);
    if (ret == -2) {
        MKSLOG_RED("Command timed out.");
        // wpa_ctrl_close(ctrl_conn);
        return ret;
    } else if (ret < 0) {
        MKSLOG_RED("Command failed.");
        // wpa_ctrl_close(ctrl_conn);
        return ret;
    } else if (ret == 0) {
        replyBuff[reply_len] = '\0';
        MKSLOG_YELLOW("返回的消息: %s", replyBuff);
        if (strstr(replyBuff, "OK") == 0) {
            MKSLOG("ENABLE_NETWORK 成功");
            mks_save_config();
        }
    }

    return ret;
}
*/

int mks_set_ssid(char *ssid) {
    //4.4.1 CLL 修复WiFi刷新
    char path[64] = {"\0"};
    char cmd[64];
    char replyBuff[2048] = {"\0"};
    size_t reply_len;
    int ret;

    sprintf(path, "/var/run/wpa_supplicant/wlan0");
    ctrl_conn = wpa_ctrl_open(path);

    if (!ctrl_conn) {
        printf("Open wpa control interfaces failed!\n");
        return -3;
    } else {
        printf("Successful!\n");
    }

    /* wpa_ctrl_request SET_NETWORK ssid */
    memset(cmd, 0x00, sizeof(cmd));
    snprintf(cmd, sizeof(cmd) - 1, "SET_NETWORK 0 ssid \"%s\"", ssid);
    MKSLOG_RED("发送cmd命令为: %s", cmd);
    memset(replyBuff, 0x00, sizeof(replyBuff));
    reply_len = sizeof(replyBuff) - 1;
    ret = wpa_ctrl_request(ctrl_conn, cmd, strlen(cmd), replyBuff, &reply_len, NULL);
    if (ret == -2) {
        MKSLOG_RED("Command timed out.");
        // wpa_ctrl_close(ctrl_conn);
        return ret;
    } else if (ret < 0) {
        MKSLOG_RED("Command failed.");
        return ret;
    } else if (ret == 0) {
        replyBuff[reply_len] = '\0';
        MKSLOG_YELLOW("返回的消息: %s", replyBuff);
        if (strstr(replyBuff, "OK") != NULL) {
            return ret;
        }
        /*
        } else if (strstr(replyBuff, "FAIL") != NULL) {
            mks_add_set_network(ssid);
        }
        */
    }
    // wpa_ctrl_close(ctrl_conn);
    return ret;
}

int mks_set_psk(char *psk) {
    //4.4.1 CLL 修复WiFi刷新
    //char path[64] = {"\0"};
    char cmd[64];
    char replyBuff[2048] = {"\0"};
    size_t reply_len;
    int ret;
    char path[64] = {"\0"};
    sprintf(path, "/var/run/wpa_supplicant/wlan0");
    ctrl_conn = wpa_ctrl_open(path);

    if (!ctrl_conn) {
        printf("Open wpa control interfaces failed!\n");
        return -3;
    } else {
        printf("Successful!\n");
    }

    /* wpa_ctrl_request SET_NETWORK psk */
    // memset(cmd, 0x00, sizeof(cmd));
    // snprintf(cmd, sizeof(cmd) - 1, "SET_NETWORK 0 psk \"makerbase318\"");
    memset(cmd, 0x00, sizeof(cmd));
    sprintf(cmd, "SET_NETWORK 0 psk \"%s\"", psk);
    MKSLOG_RED("发送cmd命令为: %s", cmd);
    // memset(replyBuff, 0x00, sizeof(replyBuff));
    reply_len = sizeof(replyBuff) - 1;
    // ret = wpa_ctrl_request(ctrl_conn, "SET_NETWORK 0 psk \"makerbase318\"", strlen("SET_NETWORK 0 psk \"makerbase318\""), replyBuff, &reply_len, wpa_cli_msg_cb);
    ret = wpa_ctrl_request(ctrl_conn, cmd, strlen(cmd), replyBuff, &reply_len, NULL);
    if (ret == 0) {
        replyBuff[reply_len] = '\0';
        MKSLOG_YELLOW("返回的消息: %s", replyBuff);
        if (strstr(replyBuff, "OK") != NULL) {
            // wpa_ctrl_close(ctrl_conn);
            // MKSLOG_RED("断开网络连接");
            // mks_disable_network();
            system("ifconfig wlan0 down");
            system("ifconfig wlan0 up");
			//4.4.3 CLL 修复WiFi
            system("wpa_cli reassociate");
            if ( mks_enable_network()==-2 && current_page_id == TJC_PAGE_WIFI_CONNECT) {
                page_to(TJC_PAGE_WIFI_FAILED);
            } else {
                MKSLOG_RED("判断成功");
            }
            // sleep(2);
            // mks_enable_network();
            return ret;
        }
    } else if (ret = -2) {
        MKSLOG_RED("Command timed out. %s", replyBuff);
        // wpa_ctrl_close(ctrl_conn);
        // return ret;
    } else if (ret < 0) {
        MKSLOG_RED("Command failed.");
        // wpa_ctrl_close(ctrl_conn);
        // return ret;
    }

    // wpa_ctrl_close(ctrl_conn);
    return ret;
}

int mks_disable_network() {
    // char path[64] = {"\0"};
    char cmd[64];

    char replyBuff[4096] = {"\0"};
    size_t reply_len;
    int ret;

    // sprintf(path, "/var/run/wpa_supplicant/wlan0");
    // ctrl_conn = wpa_ctrl_open(path);

    if (!ctrl_conn) {
        MKSLOG_RED("Open wpa control interfaces failed!");
        return -3;
    } else {
        MKSLOG_RED("Successful!");
    }

    /* wpa_ctrl_request, disable_network 0 */
    memset(cmd, 0x00, sizeof(cmd));
    snprintf(cmd, sizeof(cmd) - 1, "DISABLE_NETWORK 0");
    memset(replyBuff, 0x00, sizeof(replyBuff));
    reply_len = sizeof(replyBuff) - 1;
    ret = wpa_ctrl_request(ctrl_conn, cmd, strlen(cmd), replyBuff, &reply_len, wpa_cli_msg_cb);
    if (ret == -2) {
        MKSLOG_RED("Command timed out.");
        // wpa_ctrl_close(ctrl_conn);
        return ret;
    } else if (ret < 0) {
        MKSLOG_RED("Command failed.");
        // wpa_ctrl_close(ctrl_conn);
    } else if (ret == 0) {
        replyBuff[reply_len] = '\0';
        MKSLOG_YELLOW("返回的消息: %s", replyBuff);
        if (strstr(replyBuff, "OK") != NULL) {
            MKSLOG_RED("使能网络连接");
            // system("ifconfig wlan0 down");
            // system("ifconfig wlan0 up");
            // sleep(2);
            // mks_enable_network();
        }
    }

    // wpa_ctrl_close(ctrl_conn);
    return ret;
}

int mks_enable_network() {
    //4.4.1 CLL 修复WiFi刷新
    char path[64] = {"\0"};
    char cmd[64];

    char replyBuff[4096] = {"\0"};
    size_t reply_len;
    int ret;

    sprintf(path, "/var/run/wpa_supplicant/wlan0");
    ctrl_conn = wpa_ctrl_open(path);

    if (!ctrl_conn) {
        MKSLOG_RED("Open wpa control interfaces failed!");
        return -3;
    } else {
        MKSLOG_RED("Successful!");
    }

    /* wpa_ctrl_request, enable_network 0 */
    memset(cmd, 0x00, sizeof(cmd));
    snprintf(cmd, sizeof(cmd) - 1, "ENABLE_NETWORK 0");
    memset(replyBuff, 0x00, sizeof(replyBuff));
    reply_len = sizeof(replyBuff) - 1;
    ret = wpa_ctrl_request(ctrl_conn, cmd, strlen(cmd), replyBuff, &reply_len, NULL);
    if (ret == -2) {
        MKSLOG_RED("Command timed out.");
        // wpa_ctrl_close(ctrl_conn);
        return ret;
    } else if (ret < 0) {
        MKSLOG_RED("Command failed.");
        // wpa_ctrl_close(ctrl_conn);
		return ret;
    } else if (ret == 0) {
        replyBuff[reply_len] = '\0';
        MKSLOG_YELLOW("返回的消息: %s", replyBuff);
    }

    // wpa_ctrl_close(ctrl_conn);
    return ret;
}

int mks_save_config() {
    char cmd[64] {"\0"};
    char replyBuff[2048] = {"\0"};
    size_t reply_len;
    int ret;

    if (!ctrl_conn) {
        MKSLOG_RED("Open wpa control interfaces failed!");
        return -3;
    } else {
        MKSLOG_RED("Successful!");
    }

    /* wpa_ctrl_request, save_config */
    reply_len = sizeof(replyBuff) - 1;
    sprintf(cmd, "SAVE_CONFIG");
    memset(replyBuff, 0x00, sizeof(replyBuff));
    reply_len = sizeof(replyBuff) - 1;
    ret = wpa_ctrl_request(ctrl_conn, cmd, strlen(cmd), replyBuff, &reply_len, NULL);
    if (ret == -2) {
        MKSLOG_RED("Command timed out.");
        // wpa_ctrl_close(ctrl_conn);
        return ret;
    } else if (ret < 0) {
        MKSLOG_RED("Command failed.");
        // wpa_ctrl_close(ctrl_conn);
    } else if (ret == 0) {
        replyBuff[reply_len] = '\0';
        MKSLOG_YELLOW("返回的消息: %s", replyBuff);
        if (strstr(replyBuff, "OK") != NULL) {
            MKSLOG_YELLOW("保存wifi配置成功");
            mks_wpa_get_status();       // 在这里获取一下 status
            system("sync");
            if (TJC_PAGE_WIFI_SAVING == current_page_id) {
                sleep(3);
                page_wifi_list_ssid_button_enabled[0] = false;
                page_wifi_list_ssid_button_enabled[1] = false;
                page_wifi_list_ssid_button_enabled[2] = false;
                page_wifi_list_ssid_button_enabled[3] = false;
                page_wifi_ssid_list_pages = 0;
                page_wifi_current_pages = 0;
                page_to(TJC_PAGE_WIFI_LIST);
            }
        }
    }

    return ret;
}

int mks_wpa_cli_open_connection() {
    char path[64] = {"\0"};
    sprintf(path, "/var/run/wpa_supplicant/wlan0");

    ctrl_conn = wpa_ctrl_open(path);

    //4.4.1 CLL 修复WiFi刷新
    if (ctrl_conn) {
        mks_wpa_cli_connected = true;
        MKSLOG_RED("成功连接wpa connection");
        return 0;
    } else {
        mks_wpa_cli_connected = false;
        return -1;
    }
}

int mks_wpa_cli_close_connection() {
    wpa_ctrl_close(ctrl_conn);
    ctrl_conn = NULL;
    mks_wpa_cli_connected = false;
    MKSLOG_RED("断开wpa connection");
    return 0;
}

int mks_wpa_scan() {
    if (!ctrl_conn) {
        MKSLOG_RED("打开wpa控制接口失败!\n");
        return -3;
    } else {
        MKSLOG_BLUE("Successful");
    }

    char replyBuff[4096] = {"\n"};
    size_t reply_len;

    int ret;
    reply_len = sizeof(replyBuff) - 1;
    ret = wpa_ctrl_request(ctrl_conn, "SCAN", strlen("SCAN"), replyBuff, &reply_len, wpa_cli_msg_cb);
    if (ret == -2) {
        MKSLOG_RED("Command timed out.");
        // return ret;
    } else if (ret < 0) {
        MKSLOG_RED("Command failed.");
        // return ret;
    } else if (ret == 0) {
        replyBuff[reply_len] = '\0';
        MKSLOG_RED("回收到的消息: \n%s\n", replyBuff);
        if (strncmp(replyBuff, "OK", 2) == 0) {
            MKSLOG_YELLOW("结果: %s", replyBuff);
        }
    }
    return ret;
}

int mks_wpa_scan_results(char *mks_result) {
    if (!ctrl_conn) {
        MKSLOG_RED("打开wpa控制接口失败!\n");
        return -3;
    } else {
        MKSLOG_BLUE("Successful");
    }

    char replyBuff[4096] = {"\n"};
    size_t reply_len;

    int ret;
    reply_len = sizeof(replyBuff) - 1;
    ret = wpa_ctrl_request(ctrl_conn, "SCAN_RESULTS", strlen("SCAN_RESULTS"), replyBuff, &reply_len, wpa_cli_msg_cb);

    if (ret == -2) {
        MKSLOG_RED("Command timed out.");
    } else if (ret < 0) {
        MKSLOG_RED("Command failed.");
    } else if (ret == 0) {
        replyBuff[reply_len] = '\0';
        MKSLOG_RED("收到:\n%s", replyBuff);
        mks_result = replyBuff;
    }

    return ret;
}

int mks_wpa_get_status() {
    if (!ctrl_conn) {
        printf("Open wpa control interfaces failed!\n");
        return -3;
    } else {
        printf("Successful!\n");
    }

    char replyBuff[512] = {"\0"};
    size_t reply_len;

    int ret;
    reply_len = sizeof(replyBuff) - 1;
    ret = wpa_ctrl_request(ctrl_conn, "STATUS", strlen("STATUS"), replyBuff, &reply_len, NULL);

    if (ret == -2) {
        printf("Command timed out.\n");
        return ret;
    } else if (ret < 0) {
        printf("Command failed.\n");
        return ret;
    } else if (ret == 0) {
        replyBuff[reply_len] = '\0';
        MKSLOG_YELLOW("返回的消息:\n%s", replyBuff);
    }
    return ret;
}

int mks_parse_status(struct mks_wifi_status_t *status) {
    memset(status, 0x00, sizeof(mks_wifi_status_t));
    char item[16];

    memset(item, 0x00, sizeof(item));
    sprintf(item, "bssid");
    result_get(mks_wifi_status, item, status->bssid, sizeof(status->bssid));

    memset(item, 0x00, sizeof(item));
    sprintf(item, "wpa_state");
    result_get(mks_wifi_status, item, status->wpa_state, sizeof(status->wpa_state));
    
    memset(item, 0x00, sizeof(item));
    sprintf(item, "ip_address");
    result_get(mks_wifi_status, item, status->ip_address, sizeof(status->ip_address));

    memset(item, 0x00, sizeof(item));
    sprintf(item, "address");
    result_get(mks_wifi_status, item, status->address, sizeof(status->address));

    return 0;
}
