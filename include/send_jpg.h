#ifndef SEND_JPG_H
#define SEND_JPG_H

#include <stdio.h>
#include <stdint.h>
#include <stdint.h>
#include "mks_log.h"
#include "send_msg.h"

// #define BLOCK_SIZE 3072
#define BLOCK_SIZE 3800
#define HEADER_SIZE 12

extern bool get_0xfe;
extern bool get_0x06;
extern bool get_0x05;
extern bool get_0xfd;
extern bool get_0x04;
extern bool get_0x24;


extern bool have_64_jpg[6];
extern std::string have_64_png_path[6];
extern bool begin_show_64_jpg;
extern bool begin_show_160_jpg;
extern bool begin_show_192_jpg;
extern bool show_192_jpg_complete;



#define LOG_RED             "\033[31;1m"
#define LOG_YELLOW          "\033[0;33m"
#define LOG_GREEN           "\033[0;32m"
#define LOG_BLUE            "\033[0;34m"
#define LOG_PURPLE          "\033[0;35m"
#define LOG_SKYBLUE         "\033[0;36m"
#define LOG_HIGHLIGHT       "\033[7m\033[5m"
#define LOG_END             "\033[0m"



void printHex(const uint8_t *data, size_t size);
// bool mks_send_jpg(char *path, int fd, int index);
void *sent_jpg_thread_handle(void *arg);
void delet_pic(std::string ram_path);

// void *sent_jpg_thread_handle(void *arg);
void png_to_64_jpg(std::string dir_path, std::string file_name, uint8_t i);
void png_to_160_jpg(std::string dir_path, std::string file_name);
void png_to_192_jpg(std::string dir_path, std::string file_name);
void delet_pic(std::string ram_path);
void delete_small_jpg();
bool sent_jpg_to_tjc(std::string ram_path, std::string pic_path);

#endif 
