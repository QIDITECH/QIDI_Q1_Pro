#include <iostream>
#include <cstring>
#include <vector>
#include <set>
#include <regex>

#include <unistd.h>

#include "../include/MakerbaseWiFi.h"
#include "../include/MakerbaseShell.h"
#include "../include/mks_log.h"
#include "../include/mks_wpa_cli.h"
#include "../include/common.h"

struct mks_wifi_status_result_t status_result;
struct mks_wifi_status_t wpa_status;
std::vector<std::string> result_list;
std::vector<std::string> ssid_list;
std::vector<int> level_list;
std::string page_wifi_ssid_list[5] = {"", "", "", "", ""};
int page_wifi_ssid_list_pages = 0;
int page_wifi_current_pages;
std::string get_wifi_name = "";
extern std::string current_connected_ssid_name;

void set_page_wifi_ssid_list(int pages) {
    page_wifi_ssid_list[0] = "";
    page_wifi_ssid_list[1] = "";
    page_wifi_ssid_list[2] = "";
    page_wifi_ssid_list[3] = "";
    page_wifi_ssid_list[4] = "";

    auto it = ssid_list.begin();

    for (int i = 0; i < pages * 5; i++) {
        it++;
    }

    if (it != ssid_list.end()) {
        page_wifi_ssid_list[0] = *it;
        it++;
    }

    if (it != ssid_list.end()) {
        page_wifi_ssid_list[1] = *it;
        it++;
    }

    if (it != ssid_list.end()) {
        page_wifi_ssid_list[2] = *it;
        it++;
    }

    if (it != ssid_list.end()) {
        page_wifi_ssid_list[3] = *it;
        it++;
    }

    if (it != ssid_list.end()) {
        page_wifi_ssid_list[4] = *it;
    }

    for (int j = 0; j < 5; j++) {
        std::cout << page_wifi_ssid_list[j] << std::endl;
    }
}

//4.4.1 CLL 修复WiFi刷新
void get_ssid_list_pages() {
    if (ssid_list.size() % 5 == 0) {
        page_wifi_ssid_list_pages = ssid_list.size() / 5;
    } else {
        page_wifi_ssid_list_pages = ssid_list.size() / 5 + 1;
    }

    if (page_wifi_ssid_list_pages > 5) {
        page_wifi_ssid_list_pages = 5;
    }
    
}

void get_wlan0_status() {
    mks_wifi_run_cmd_status(&status_result);
}

bool detected_wlan0() {
    if (access("/var/run/wpa_supplicant/wlan0", F_OK) == 0) {
        return true;
    } else {
        return false;
    }
}

// 
void split_scan_result(std::string result) {
    result_list.clear();
    level_list.clear();
    ssid_list.clear();
    char *sub_result;

    std::string token;
    std::string delimiter = "\n";
    size_t pos = 0;
    int n_level = -100;
    int index = 0;

    while ((pos = result.find(delimiter)) != std::string::npos) {
        token = result.substr(0, pos);
        result_list.push_back(token);
        result.erase(0, pos + delimiter.length());
    }

    for (int i = 1; i < result_list.size(); i++) {
        char bssid[64];
	    char freq[64];
	    char level[64];
	    char flag[64];
	    char ssid[64];
        unsigned char re[100];
        sub_result = const_cast<char*>(result_list[i].data());
	    if (5 == sscanf(sub_result, "%s \t %s \t %s \t %s \t %s", &bssid, &freq, &level, &flag, &ssid)) {
            printf_decode(re, 64, ssid);
            level_list.push_back(atoi(level));
            if (re[0] == '\x00') {
                MKSLOG_YELLOW("扫描出来的ssid为\\0");
            } else {
                ssid_list.push_back((char *)re);
                // ssid_list.insert((char *)re);
            }
        }
        memset(sub_result, 0x00, sizeof(sub_result));
        memset(ssid, 0x00, sizeof(ssid));
    }

    for (int j = 0; j < ssid_list.size(); j++) {
        if (current_connected_ssid_name == ssid_list[j]) {
            std::string temp = ssid_list[j];
            ssid_list[j] = ssid_list[0];
            ssid_list[0] = temp;
        }
    }
    for (int k = 0; k < ssid_list.size(); k++) {
        MKSLOG_RED("%s", ssid_list[k].c_str());
    }

}

std::string rescan() {
    MKSLOG("wpa_cli SCAN");
    return wpa_cli("SCAN");
}

std::string save_wpa_conf() {
    MKSLOG("Saving WPA config");
    return wpa_cli("SAVE_CONFIG");
}

std::string wpa_cli(std::string command) {
    char result[MAX_FILE_LEN];
    std::string cmd = "wpa_cli " + command;
    execute_cmd(cmd.data(), result);
    return result;
}

// WifiChannels:
static std::string lookup(int freq) {
    switch (freq)
    {
    case 2412:
        return "2.4GHz 1";
        break;

    case 2417:
        return "2.4GHz 2";
        break;

    case 2422:
        return "2.4GHz 3";
        break;

    case 2427:
        return "2.4GHz 4";
        break;

    case 2432:
        return "2.4GHz 5";
        break;

    case 2437:
        return "2.4GHz 6";
        break;

    case 2442:
        return "2.4GHz 7";
        break;

    case 2447:
        return "2.4GHz 8";
        break;

    case 2452:
        return "2.4GHz 9";
        break;

    case 2457:
        return "2.4GHz 10";
        break;

    case 2462:
        return "2.4GHz 11";
        break;

    case 2467:
        return "2.4GHz 12";
        break;

    case 2472:
        return "2.4GHz 13";
        break;

    case 2484:
        return "2.4GHz 14";
        break;

    case 5035:
        return "5GHz 7";
        break;

    case 5040:
        return "5GHz 8";
        break;

    case 5045:
        return "5GHz 9";
        break;

    case 5055:
        return "5GHz 11";
        break;

    case 5060:
        return "5GHz 12";
        break;

    case 5080:
        return "5GHz 16";
        break;

    case 5170:
        return "5GHz 34";
        break;

    case 5180:
        return "5GHz 36";
        break;

    case 5190:
        return "5GHz 38";
        break;

    case 5200:
        return "5GHz 40";
        break;

    case 5210:
        return "5GHz 42";
        break;

    case 5220:
        return "5GHz 44";
        break;

    case 5230:
        return "5GHz 46";
        break;

    case 5240:
        return "5GHz 48";
        break;

    case 5260:
        return "5GHz 52";
        break;

    case 5280:
        return "5GHz 56";
        break;

    case 5300:
        return "5GHz 60";
        break;

    case 5320:
        return "5GHz 64";
        break;

    case 5500:
        return "5GHz 100";
        break;

    case 5560:
        return "5GHz 112";
        break;

    case 5580:
        return "5GHz 116";
        break;

    case 5600:
        return "5GHz 120";
        break;

    case 5620:
        return "5GHz 124";
        break;

    case 5640:
        return "5GHz 128";
        break;

    case 5660:
        return "5GHz 132";
        break;

    case 5680:
        return "5GHz 136";
        break;

    case 5700:
        return "5GHz 140";
        break;

    case 5720:
        return "5GHz 144";
        break;

    case 5745:
        return "5GHz 149";
        break;

    case 5765:
        return "5GHz 153";
        break;

    case 5785:
        return "5GHz 157";
        break;

    case 5805:
        return "5GHz 161";
        break;

    case 5825:
        return "5GHz 165";
        break;

    case 4915:
        return "5GHz 183";
        break;

    case 4920:
        return "5GHz 184";
        break;

    case 4925:
        return "5GHz 185";
        break;

    case 4935:
        return "5GHz 187";
        break;

    case 4940:
        return "5GHz 188";
        break;

    case 4945:
        return "5GHz 189";
        break;

    case 4960:
        return "5GHz 192";
        break;

    case 4980:
        return "5GHz 196";
        break;

    default:
        return "";
        break;
    }
}

size_t printf_decode(unsigned char *buf, size_t maxlen, const char *str)
{
	const char *pos = str;
	size_t len = 0;
	int val;

	while (*pos) {
		if (len + 1 >= maxlen)
			break;
		switch (*pos) {
		case '\\':
			pos++;
			switch (*pos) {
			case '\\':
				buf[len++] = '\\';
				pos++;
				break;
			case '"':
				buf[len++] = '"';
				pos++;
				break;
			case 'n':
				buf[len++] = '\n';
				pos++;
				break;
			case 'r':
				buf[len++] = '\r';
				pos++;
				break;
			case 't':
				buf[len++] = '\t';
				pos++;
				break;
			case 'e':
				buf[len++] = '\033';
				pos++;
				break;
			case 'x':
				pos++;
				val = hex2byte(pos);
				if (val < 0) {
					val = hex2num(*pos);
					if (val < 0)
						break;
					buf[len++] = val;
					pos++;
				} else {
					buf[len++] = val;
					pos += 2;
				}
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
				val = *pos++ - '0';
				if (*pos >= '0' && *pos <= '7')
					val = val * 8 + (*pos++ - '0');
				if (*pos >= '0' && *pos <= '7')
					val = val * 8 + (*pos++ - '0');
				buf[len++] = val;
				break;
			default:
				break;
			}
			break;
		default:
			buf[len++] = *pos++;
			break;
		}
	}
	if (maxlen > len)
		buf[len] = '\0';

	return len;
}

int hex2byte(const char *hex)
{
	int a, b;
	a = hex2num(*hex++);
	if (a < 0)
		return -1;
	b = hex2num(*hex++);
	if (b < 0)
		return -1;
	return (a << 4) | b;
}

static int hex2num(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	return -1;
}

int parse_scan_results(char* scan_results) {
    result_list.clear();
    level_list.clear();
    ssid_list.clear();

    char buffer[4096];
    strcpy(buffer, scan_results);
    char* lines[128] = {0};
    int num_lines = 0;
    char* line = strtok(buffer, "\n");
    while (line != NULL) {
        lines[num_lines++] = line;
        line = strtok(NULL, "\n");
    }

    for (int i = 1; i < num_lines; ++i) {
        char* fields[5] = {0};
        int num_fields = 0;

        char ssid_line[256] = {0};
        memset(ssid_line, 0x00, sizeof(ssid_line));
        strcpy(ssid_line, lines[i]);
        int ssid_line_index = 0;

        char* field = strtok(lines[i], " \t");
        
        while (field != NULL) {
            if (4 == num_fields) {
                ssid_line_index = field - lines[i];
            }
            
            fields[num_fields++] = field;
            field = strtok(NULL, " \t");
        }

        if (num_fields < 5) {
            printf("Invalid scan result: %s\n", lines[i]);
            continue;
        } else {
            unsigned char ssid_name[192];
            printf_decode(ssid_name, 192, ssid_line + ssid_line_index);
            if (ssid_name[0] == '\x00') {

            } else {
                ssid_list.push_back((char *)ssid_name);
            }
        }
        
        if (strstr(fields[3], "ESS") && strstr(fields[3], "privacy") && strstr(fields[3], "hidden-ssid")) {
        } else {
        }
    }

    for (int j = 0; j < ssid_list.size(); j++) {
        if (current_connected_ssid_name == ssid_list[j]) {
            std::string temp = ssid_list[j];
            ssid_list[j] = ssid_list[0];
            ssid_list[0] = temp;
        }
    }

    for (int k = 0; k < ssid_list.size(); k++) {
        MKSLOG_RED("%s", ssid_list[k].c_str());
    }

    return 0;
}
