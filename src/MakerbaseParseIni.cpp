#include "../include/MakerbaseParseIni.h"

dictionary *mksini = NULL;

dictionary *printer_cfg = NULL;

dictionary *mksversion = NULL;

// std::string get_cfg_serial() {
//     printer_cfg = iniparser_load("/home/mks/klipper_config/MKS_THR.cfg");
//     if (printer_cfg == NULL) {
//         std::cout << "cfg parse failure!" << std::endl;
//         return "";
//     }
//     std::string sk = "mcu MKS_THR:serial";
//     std::cout << "打开配置文件成功" << std::endl;
//     const char *value = iniparser_getstring(mksini, sk.c_str(), "");
//     return (std::string)value;
// }

// CLL 用于获取在线更新信息
int updateini_load() {
    mksini = iniparser_load("/root/auto_update/update_info.ini");
    
    if (mksini == NULL) {
        std::cout << "Ini parse failure" << std::endl;
        return -1;
    }

    return 0;
}

// CLL 用于获取在线更新进度
int progressini_load() {
    mksini = iniparser_load("/root/auto_update/update_progress.ini");
    
    if (mksini == NULL) {
        std::cout << "Ini parse failure" << std::endl;
        return -1;
    }

    return 0;
}

int mksini_load() {
    mksini = iniparser_load(INIPATH);

    if (mksini == NULL) {
        std::cout << "Ini parse failure!" << std::endl;
        return -1;
    }

    return 0;
}

void mksini_free() {
    iniparser_freedict(mksini);
}

std::string mksini_getstring(std::string section, std::string key, std::string def) {
    std::string sk = section + ":" + key;
    const char *value = iniparser_getstring(mksini, sk.c_str(), def.c_str());
    return (std::string)value;
}

int mksini_getint(std::string section, std::string key, int notfound) {
    std::string sk = section + ":" + key;
    int value = iniparser_getint(mksini, sk.c_str(), notfound);
    return value;
}

double mksini_getdouble(std::string section, std::string key, double notfound) {
    std::string sk = section + ":" + key;
    double value = iniparser_getdouble(mksini, sk.c_str(), notfound);
    return value;
}

bool mksini_getboolean(std::string section, std::string key, int notfound) {
    std::string sk = section + ":" + key;
    int value = iniparser_getboolean(mksini, sk.c_str(), notfound);
    return (value == 0) ? false : true;
}

int mksini_set(std::string section, std::string key, std::string value) {
    std::string sk = section + ":" + key;
    int ret;
    ret = iniparser_set(mksini, sk.c_str(), value.c_str());
    return ret;
}

void mksini_unset(std::string section, std::string key) {
    std::string sk = section + ":" + key;
    iniparser_unset(mksini, sk.c_str());
}

// 保存到配置文件
void mksini_save() {
    FILE *ini = fopen(INIPATH, "w");
    if (ini == NULL) {
        printf("[error] open mksini failed");
        return;
    }
    iniparser_dump_ini(mksini, ini);
    fclose(ini);
}

int mksversion_load() {
    mksversion = iniparser_load(VERSION_PATH);

    if (mksversion == NULL) {
        std::cout << "Mks version failure!" << std::endl;
        return -1;
    }

    return 0;
}

void mksversion_free() {
    iniparser_freedict(mksversion);
}

std::string mksversion_mcu(std::string def) {
    std::string version = "version:mcu";
    const char *value = iniparser_getstring(mksversion, version.c_str(), def.c_str());
    return (std::string)value;
}

std::string mksversion_ui(std::string def) {
    std::string version = "version:ui";
    const char *value = iniparser_getstring(mksversion, version.c_str(), def.c_str());
    return (std::string)value;
}

std::string mksversion_soc(std::string def) {
    std::string version = "version:soc";
    const char *value = iniparser_getstring(mksversion, version.c_str(), def.c_str());
    return (std::string)value;
}
