#ifndef MAKERBASEPARSEINI_H
#define MAKERBASEPARSEINI_H

#define XINDI_PLUS 1
#define XINDI_MAX 0
#define XINDI_MINI 0

#include <iostream>
#include "./dictionary.h"
#include "./iniparser.h"

//#define INIPATH "/root/config.mksini"
#define INIPATH "/home/mks/klipper_config/config.mksini"

#ifdef XINDI_PLUS
#define VERSION_PATH "/root/xindi/version"
#elif XINDI_MAX
#define VERSION_PATH "/root/xindi/version-max"
#elif XINDI_MINI
#define VERSION_PATH "/root/xindi/version-4-3"
#endif

// std::string get_cfg_serial();
int mksini_load();
void mksini_free();
std::string mksini_getstring(std::string section, std::string key, std::string def);
int mksini_getint(std::string section, std::string key, int notfound);
double mksini_getdouble(std::string section, std::string key, double notfound);
bool mksini_getboolean(std::string section, std::string key, int notfound);
int mksini_set(std::string section, std::string key, std::string value);
void mksini_unset(std::string section, std::string key);
void mksini_save();

int mksversion_load();
void mksversion_free();
std::string mksversion_mcu(std::string def);
std::string mksversion_ui(std::string def);
std::string mksversion_soc(std::string def);

int updateini_load();
int progressini_load();

#endif
