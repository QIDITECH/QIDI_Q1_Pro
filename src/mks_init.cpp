#include "../include/mks_init.h"
#include "../include/MakerbaseShell.h"

std::string serial_by_id;

bool get_by_id() {
    struct dirent *dir;
    DIR *dirp;
    dirp = opendir("/dev/serial/by-id");
    if (NULL == dirp) {
        printf("读取串口失败!!!!!!!!!!!!!!!!\n");
        return false;
    } else {
        serial_by_id = generate_by_id();
        serial_by_id = serial_by_id.substr(0, 58);
        std::cout << serial_by_id.length() << " 成功读取到ID号  " + serial_by_id << std::endl;
        closedir(dirp);
        return true;
    }
}

std::string generate_by_id() {
    char result[MAX_FILE_LEN] = {0};
    std::string cmd = "ls /dev/serial/by-id/*";
    execute_cmd(cmd.data(), result);
    printf("%s", result);
    return result;
}

bool FileStringReplace(std::ifstream &instream, std::ofstream &outstream)
{
	std::string str;
	size_t pos = 0;
	while (getline(instream, str)) // 按行读取
	{
		pos = str.find("serial:"); // 查找每一行中的"Tom"
		if (pos != std::string::npos)
		{
			str = str.replace(pos, str.length(), "Jerry"); // 将Tom替换为Jerry
			outstream << str << std::endl;
			continue;
		}
		outstream << str << std::endl;
	}
	return true;
}

std::string get_cfg_by_id() {
    std::string path = "/home/mks/klipper_config/MKS_THR.cfg";
    std::ifstream data(path.c_str());
    std::string strline;
    std::string ret;
    while (getline(data, strline)) {
        bool exists = strline.find("serial: ") == std::string::npos;
        if (!exists) {
            std::cout << strline.length() << " CFG文件里面的serial的值 " + strline.substr(8, 62)  << std::endl;
            ret = strline.substr(8, 63);
            break;
        }
    }
    data.close();
    return ret;
}
