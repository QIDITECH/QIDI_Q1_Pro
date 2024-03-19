#ifndef MAKERBASE_WIFI_H
#define MAKERBASE_WIFI_H

#include "./MakerbaseShell.h"

void set_page_wifi_ssid_list(int pages);

void get_ssid_list_pages();

void get_wlan0_status();

bool detected_wlan0();

void split_scan_result(std::string result);
// void set_wpa_supplicant(std::string ssid, std::string psk);
// parse scan result


// std::string get_mac_from_result(std::string result);
// std::string get_frequency_from_result(std::string result);
// std::string get_signal_level_from_result(std::string result);
// std::string get_flags_from_result(std::string result);
// std::string get_ssid_from_result(std::string result);
// 

void get_connected_ssid();
void get_current_wifi();
// void read_wpa_supplicant();
// std::string rescan();
std::string save_wpa_conf();
// void scan_results();
std::string wpa_cli(std::string command);
static std::string lookup(int freq);

/*
void mid_wifi_ssid_convert_utf8(unsigned char *ssid, char *bssid, int size);
unsigned char mid_wifi_ssid_convert_decimal(char ssid);
*/

size_t printf_decode(unsigned char *buf, size_t maxlen, const char *str);

int hex2byte(const char *hex);
static int hex2num(char c);

int parse_scan_results(char* scan_results);

#endif
