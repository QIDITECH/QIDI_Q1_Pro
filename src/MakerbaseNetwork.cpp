#include "../include/MakerbaseNetwork.h"

std::string get_wlan0_ip() {
    char result[MAX_FILE_LEN] = {0};
    std::string cmd = "ifconfig wlan0 | awk 'NR==2{print $2}' | tr -d '\n\r'";
    execute_cmd(cmd.data(), result);
    printf("%s", result);
    return result;
}
