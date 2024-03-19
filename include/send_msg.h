#ifndef SEND_MSG_H
#define SEND_MSG_H

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void send_cmd_page(int fd, std::string pageid);
void send_cmd_ref(int fd, std::string obj);
void send_cmd_click(int fd, std::string obj, std::string event);
void send_cmd_get(int fd, std::string att);
void send_cmd_prints(int fd, std::string att, int lenth);
void send_cmd_printh(int fd, std::string hex);
void send_cmd_vis(int fd, std::string obj, std::string state);
void send_cmd_tsw(int fd, std::string obj, std::string state);
void send_cmd_randset(int fd, std::string minval, std::string maxval);
void send_cmd_add(int fd, std::string objid, std::string ch, std::string val);
void send_cmd_cle(int fd, std::string objid, std::string ch);
void send_cmd_addt(int fd, std::string objid, std::string ch, std::string qyt);
void send_cmd_doevents(int fd);
void send_cmd_sendme(int fd);
void send_cmd_covx(int fd, std::string att1, std::string att2, std::string lenth);
void send_cmd_strlen(int fd, std::string att0, std::string att1);
void send_cmd_btlen(int fd, std::string att0, std::string att1);
void send_cmd_substr(int fd, std::string att0, std::string att1, std::string star, std::string lenth);
void send_cmd_spstr(int fd, std::string src, std::string dec, std::string key, std::string indec);
void send_cmd_touch_j(int fd);
void send_cmd_ref_stop(int fd);
void send_cmd_ref_star(int fd);
void send_cmd_com_stop(int fd);
void send_cmd_com_star(int fd);
void send_cmd_code_c(int fd);
void send_cmd_rest(int fd);
void send_cmd_wepo(int fd, std::string att, std::string add);
void send_cmd_repo(int fd, std::string att, std::string add);
void send_cmd_wept(int fd, std::string add, std::string lenth);
void send_cmd_rept(int fd, std::string add, std::string lenth);
void send_cmd_cfgpio(int fd, std::string id, std::string state, std::string obj);
void send_cmd_crcrest(int fd, std::string crctype, std::string initval);
void send_cmd_crcputs(int fd, std::string att, std::string length);
void send_cmd_crcputh(int fd, std::string hex);
void send_cmd_crcputu(int fd, std::string star, std::string length);
void send_cmd_setlayer(int fd, std::string obj0, std::string obj1);
void send_cmd_move(int fd, std::string obj, std::string startx, std::string starty, std::string endx, std::string endy, std::string first, std::string time);
void send_cmd_play(int fd, std::string ch, std::string audio, std::string loop);
void send_cmd_twfile(int fd, std::string filepath, std::string filesize);
void send_cmd_delfile(int fd, std::string filepath);
void send_cmd_refile(int fd, std::string srcfilepath, std::string decfilepath);
void send_cmd_findfile(int fd, std::string filepath, std::string att);
void send_cmd_rdfile(int fd, std::string filepath, std::string addr, std::string size, std::string crc);
void send_cmd_newfile(int fd, std::string filepath, std::string size);
void send_cmd_newdir(int fd, std::string dir);
void send_cmd_deldir(int fd, std::string dir);
void send_cmd_redir(int fd, std::string srcdir, std::string decdir);
void send_cmd_finddir(int fd, std::string dir, std::string att);
void send_cmd_beep(int fd, std::string time);

void send_cmd_txt(int fd, std::string obj, std::string txt);
void send_cmd_pic(int fd, std::string obj, std::string pic);
void send_cmd_picc(int fd, std::string obj, std::string picc);
void send_cmd_picc2(int fd, std::string obj, std::string picc);
void send_cmd_val(int fd, std::string obj, std::string val);
void send_cmd_val(int fd, std::string obj, std::string val);
void send_cmd_pco(int fd, std::string obj, std::string poc);
void send_cmd_pco2(int fd, std::string obj, std::string poc2);
void send_cmd_bpic(int fd, std::string obj, std::string bpic);
void send_cmd_vid(int fd, std::string obj, std::string vid);

void send_cmd_cp(int fd, std::string obj);
void send_cmd_cp_write(int fd, std::string obj, std::string data);
void send_cmd_cp_end(int fd);
void send_cmd_cp_close(int fd, std::string obj);
void send_cmd_byte_data(int fd, char data);
void send_cmd_write(int fd, std::string obj);
void send_cmd_write_end(int fd);

void send_cmd_cp_image(int fd, std::string obj, std::string image);

void send_cmd_txt_start(int fd, std::string obj);
void send_cmd_txt_data(int fd, std::string txt);
void send_cmd_txt_end(int fd);

void send_cmd_txt_plus(int fd, std::string obj1, std::string obj2, std::string obj3);

void send_cmd_download(int fd, int filesize);

void send_cmd_download_data(int fd, std::string data);

void send_cmd_vid_en(int fd, std::string obj, int value);

void send_cmd_bauds(int fd, int bauds);

void send_cmd_baud(int fd, int baud);

void send_var_value(int fd, std::string var, int value);

#endif 
