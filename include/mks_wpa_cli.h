#ifndef MKS_WPA_CLI
#define MKS_WPA_CLI

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <wpa_ctrl.h>

struct mks_wifi_signal_poll_result_t {
    char ack[1024];
    int rssi;
    int linkspeed;
    int noise;
    int frequency;
};

struct mks_wifi_status_result_t {
    char ack[1024];
    char bssid[18];
    int freq;
    char ssid[128];
    int id;
    char mode[16];
    char pairwise_cipher[16];
    char group_cipher[16];
    char key_mgmt[16];
    char wpa_state[32];
    char ip_address[18];
    char address[18];
    char uuid[64];
};

struct mks_wifi_status_t {
    char bssid[18];
    int freq;
    char ssid[128];
    int id;
    char mode[16];
    char pairwise_cipher[16];
    char group_cipher[16];
    char key_mgmt[16];
    char wpa_state[32];
    char ip_address[18];
    char address[18];
    char uuid[64];
};

static void wpa_cli_msg_cb(char *msg, size_t len);

static int result_get(char *str, char *key, char *val, int val_len);

int mks_wifi_run_cmd_signal_poll(struct mks_wifi_signal_poll_result_t *result);

int mks_wifi_run_cmd(char const *cmd, char *result, size_t *len);

int mks_wifi_run_cmd_status(struct mks_wifi_status_result_t *result);

// void mks_wifi_set_config(char *ssid, char *psk);

// int mks_wifi_get_config(char *ssid, char *psk);

void test_wifi_run_cmd_signal(void);

void test_wifi_run_cmd_status(void);

int wifi_send_command(const char *cmd, char *reply, size_t *reply_len);

// int mks_wifi_connect_ap_fun(char *ssid, char *psk);

void *mks_wifi_hdlevent_thread(void *arg);

int mks_wpa_scan_scanresults();

// int mks_wifi_connect(char *ssid, char *psk);

int mks_set_ssid(char *ssid);

int mks_set_psk(char *psk);

int mks_disable_network();

int mks_enable_network();

int mks_save_config();

int mks_wpa_cli_open_connection();

int mks_wpa_cli_close_connection();

int mks_wpa_scan();

int mks_wpa_scan_results(char *mks_result);

int mks_wpa_get_status();

int mks_parse_status(struct mks_wifi_status_t *status);

#endif
