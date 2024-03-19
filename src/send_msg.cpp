#include "../include/mks_log.h"
#include "../include/send_msg.h"
#include "../include/MakerbaseSerial.h"

extern int copy_fd;

// 刷新页面
void send_cmd_page(int fd, std::string pageid) {
    std::string cmd  = "page " + pageid + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 重绘控件
void send_cmd_ref(int fd, std::string obj) {
    std::string cmd = "ref " + obj + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 	激活控件按下/弹起事件
void send_cmd_click(int fd, std::string obj, std::string event) {
    std::string cmd = "click " + obj + "," + event + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), sizeof(cmd));
}

// 带格式获取变量值/常量值
void send_cmd_get(int fd, std::string att) {
    std::string cmd = "get " + att + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 从串口打印一个变量/常量
void send_cmd_prints(int fd, std::string att, int lenth = 0) {
    std::string cmd = "prints " + att + "," + std::to_string(lenth) + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 隐藏/显示控件
void send_cmd_vis(int fd, std::string obj, std::string state) {
    std::string cmd = "vis " + obj + "," + state + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 控件触摸使能
void send_cmd_tsw(int fd, std::string obj, std::string state) {
    std::string cmd = "tsw " + obj + "," + state + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 随机数范围设置
void send_cmd_randset(int fd, std::string minval, std::string maxval) {
    std::string cmd = "randset " + minval + "," + maxval + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 往曲线控件添加数据
void send_cmd_add(int fd, std::string objid, std::string ch, std::string val) {
    std::string cmd = "add " + objid + "," + ch + "," + val + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 清除曲线控件中的数据
void send_cmd_cle(int fd, std::string objid, std::string ch) {
    std::string cmd = "cle " + objid + "," + ch + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 曲线数据透传指令
void send_cmd_addt(int fd, std::string objid, std::string ch, std::string qyt) {
    std::string cmd = "addt " + objid + "," + ch + "," + qyt + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 转让系统控制权给屏幕刷新
void send_cmd_doevents(int fd) {
    std::string cmd = "doevents\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 发送当前页面ID号到串口
void send_cmd_sendme(int fd) {
    std::string cmd = "sendme\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 变量类型转换
void send_cmd_covx(int fd, std::string att1, std::string att2, std::string lenth) {
    std::string cmd = "covx " + att1 + "," + att2 + "," + lenth + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 字符串变量字符长度测试
void send_cmd_strlen(int fd, std::string att0, std::string att1) {
    std::string cmd = "strlen " + att0 + "," + att1 + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 字符串变量字节长度测试
void send_cmd_btlen(int fd, std::string att0, std::string att1) {
    std::string cmd = "btlen " + att0 + "," + att1 + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 字符串截取
void send_cmd_substr(int fd, std::string att0, std::string att1, std::string star, std::string lenth) {
    std::string cmd = "substr " + att0 + "," + att1 + "," + star + "," + lenth + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 字符串分割
void send_cmd_spstr(int fd, std::string src, std::string dec, std::string key, std::string indec) {
    std::string cmd = "spstr " + src + "," + dec + "," + key + "," + indec + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 触摸校准
void send_cmd_touch_j(int fd) {
    std::string cmd = "touch_j\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 暂停屏幕刷新
void send_cmd_ref_stop(int fd) {
    std::string cmd = "ref_stop\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 恢复屏幕刷新
void send_cmd_ref_star(int fd) {
    std::string cmd = "ref_star\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 暂停串口指令执行
void send_cmd_com_stop(int fd) {
    std::string cmd = "com_stop\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 恢复串口指令执行
void send_cmd_com_star(int fd) {
    std::string cmd = "com_star\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 清空串口指令缓冲区
void send_cmd_code_c(int fd) {
    std::string cmd = "code_c\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 复位
void send_cmd_rest(int fd) {
    std::string cmd = "rest\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 写入一个变量到用户存储区
void send_cmd_wepo(int fd, std::string att, std::string add) {
    std::string cmd = "wepo " + att + "," + add + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 从用户存储区读数据到变量
void send_cmd_repo(int fd, std::string att, std::string add) {
    std::string cmd = "repo " + att + "," + add + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 透传数据写入用户存储区
void send_cmd_wept(int fd, std::string add, std::string lenth) {
    std::string cmd = "wept " + add + "," + lenth + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 从用户存储区透传数据到串口
void send_cmd_rept(int fd, std::string add, std::string lenth) {
    std::string cmd = "rept " + add + "," + lenth + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 扩展IO模式配置
void send_cmd_cfgpio(int fd, std::string id, std::string state, std::string obj) {
    std::string cmd = "cfgpio " + id + "," + state + "," + obj + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 复位crc初始值
void send_cmd_crcrest(int fd, std::string crctype, std::string initval) {
    std::string cmd = "crcrest " + crctype + "," + initval + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// crc校验一个变量/常量
void send_cmd_crcputs(int fd, std::string att, std::string length) {
    std::string cmd = "crcputs " + att + "," + length + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// crc校验一组Hex
void send_cmd_crcputh(int fd, std::string hex) {
    std::string cmd = "crcputh " + hex + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// crc校验一段串口缓冲区数据(recmod=1时有效)
void send_cmd_crcputu(int fd, std::string star, std::string length) {
    std::string cmd = "crcputu " + star + "," + length + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 运行中改变控件图层顺序(仅X3,X5系列支持)
void send_cmd_setlayer(int fd, std::string obj0, std::string obj1) {
    std::string cmd = "setplayer " + obj0 + "," + obj1 + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 控件移动 (仅X3,X5系列支持)
void send_cmd_move(int fd, std::string obj, std::string startx, std::string starty, std::string endx, std::string endy, std::string first, std::string time) {
    std::string cmd = "move " + obj + "," + startx + "," + starty + "," + endx + "," + endy + "," + first + "," + time + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 音频播放 (仅X3,X5系列支持)
void send_cmd_play(int fd, std::string ch, std::string audio, std::string loop) {
    std::string cmd = "play " + ch + "," + audio + "," + loop + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 串口透传文件 (仅X3,X5系列支持)
void send_cmd_twfile(int fd, std::string filepath, std::string filesize) {
    std::string cmd = "twfile \"" + filepath + "\"," + filesize + "\xff\xff\xff";
    MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 删除文件 (仅X3,X5系列支持)
void send_cmd_delfile(int fd, std::string filepath) {
    std::string cmd = "delfile \"" + filepath + "\"\xff\xff\xff";
    MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 重命名文件 (仅X3,X5系列支持)
void send_cmd_refile(int fd, std::string srcfilepath, std::string decfilepath) {
    std::string cmd = "refile " + srcfilepath + "," + decfilepath + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 	查找文件 (仅X3,X5系列支持)
void send_cmd_findfile(int fd, std::string filepath, std::string att) {
    std::string cmd = "findfile " + filepath + "," + att + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 透传读文件 (仅X3,X5系列支持)
void send_cmd_rdfile(int fd, std::string filepath, std::string addr, std::string size, std::string crc) {
    std::string cmd = "rdfile " + filepath + "," + addr + "," + size + "," + crc + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 	创建文件 (仅X3,X5系列支持)
void send_cmd_newfile(int fd, std::string filepath, std::string size) {
    std::string cmd = "newfile " + filepath + "," + size + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 创建文件夹 (仅X3,X5系列支持)
void send_cmd_newdir(int fd, std::string dir) {
    std::string cmd = "newdir " + dir + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 	删除文件夹 (仅X3,X5系列支持)
void send_cmd_deldir(int fd, std::string dir) {
    std::string cmd = "deldir " + dir + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 重命名文件夹 (仅X3,X5系列支持)
void send_cmd_redir(int fd, std::string srcdir, std::string decdir) {
    std::string cmd = "redir " + srcdir + "," + decdir + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 查找文件夹 (仅X3,X5系列支持)
void send_cmd_finddir(int fd, std::string dir, std::string att) {
    std::string cmd = "finddir " + dir + "," + att + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 蜂鸣器 (仅X2系列支持)
void send_cmd_beep(int fd, std::string time) {
    std::string cmd = "beep " + time + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 更改控件显示文本
void send_cmd_txt(int fd, std::string obj, std::string txt) {
    std::string cmd = obj + ".txt=" + "\"" + txt + "\"" + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 更改控件显示图片
void send_cmd_pic(int fd, std::string obj, std::string pic) {
    std::string cmd = obj + ".pic=" + pic + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

void send_cmd_picc(int fd, std::string obj, std::string picc) {
    std::string cmd = obj + ".picc=" + picc + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

void send_cmd_picc2(int fd, std::string obj, std::string picc) {
    std::string cmd = obj + ".picc2=" + picc + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 修改变量的值
void send_cmd_val(int fd, std::string obj, std::string val) {
    std::string cmd = obj + ".val=" + val + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 修改颜色
void send_cmd_pco(int fd, std::string obj, std::string poc) {
    std::string cmd = obj + ".pco=" + poc + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

void send_cmd_pco2(int fd, std::string obj, std::string poc2) {
    std::string cmd = obj + ".pco2=" + poc2 + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

// 修改背景图片
void send_cmd_bpic(int fd, std::string obj, std::string bpic) {
    std::string cmd = obj + ".bpic=" + bpic + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

void send_cmd_cp(int fd, std::string obj) {
    std::string cp0 = obj + ".write(\"";
    tcdrain(fd);
    write(fd, cp0.data(), cp0.length());
}

void send_cmd_vid(int fd, std::string obj, std::string vid) {
    std::string cmd = obj + ".vid=" + vid + "\xff\xff\xff";
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

void send_cmd_cp_write(int fd, std::string obj, std::string data) {

    // std::string cmd = obj + ".write(\"" + data + "\")\xff\xff\xff";
    // std::string cp0 = obj + ".write(\"";
    // std::string end = "\")\xff\xff\xff";
    // write(fd, cp0.data(), cp0.length());
    tcdrain(fd);
    write(fd, data.data(), data.length());
    // write(fd, end.data(), end.length());
    // write(fd, cmd.data(), cmd.length());
}

void send_cmd_cp_end(int fd) {
    std::string end = "\")\xff\xff\xff";
    tcdrain(fd);
    write(fd, end.data(), end.length());
}

void send_cmd_cp_close(int fd, std::string obj) {
    std::string cmd = obj + ".close()\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

void send_cmd_byte_data(int fd, char data) {
    char buff[1];
    buff[0] = data;
    tcdrain(fd);
    write(fd, buff, 1);
}

void send_cmd_write(int fd, std::string obj) {
    std::string cmd = obj + ".write(\"";
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

void send_cmd_write_end(int fd) {
    std::string cmd = "\")\xff\xff\xff";
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

void send_cmd_cp_image(int fd, std::string obj, std::string image) {
    send_cmd_write(fd, obj);
    tcdrain(fd);
    write(fd, image.data(), image.length());
    // MKSLOG_RED("写入图片");
    /*
    for (int i = 0; i < image.length(); i++) {
        send_cmd_byte_data(fd, static_cast<char>(image[i]));
    }
    */
    send_cmd_write_end(fd);
}

void send_cmd_txt_start(int fd, std::string obj) {
    std::string cmd = obj + ".txt=\"";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

void send_cmd_txt_data(int fd, std::string txt) {
    tcdrain(fd);
    write(fd, txt.data(), txt.length());
}

void send_cmd_txt_end(int fd) {
    std::string cmd = "\"\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

void send_cmd_txt_plus(int fd, std::string obj1, std::string obj2, std::string obj3) {
    std::string cmd = obj1 + ".txt=" + obj2 + ".txt+" + obj3 + ".txt\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

void send_cmd_download(int fd, int filesize) {
    std::string cmd = "whmi-wri " + std::to_string(filesize) + ",115200,0\xff\xff\xff";
    // std::string cmd = "whmi-wri " + std::to_string(filesize) + ",9600,0\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
    // set_option(fd, 230400, 8, 'N', 1);
    
}

void send_cmd_download_data(int fd, std::string data) {
    
    // write(fd, data.data(), data.length());
    // write(copy_fd, data.data(), data.length());
    
    // int num = 1024;
    int num = 512;
    int len = data.length();
    int end = num;
    std::string sub_data;
    for (int start = 0; start < len; ) {
        if (end > len) {
            sub_data = data.substr(start, len - start);
            write(fd, sub_data.data(), sub_data.length());
            // write(copy_fd, sub_data.data(), sub_data.length());
            // set_option(fd, 115200, 8, 'N', 1);
            break;
        }
        sub_data = data.substr(start, num);
        write(fd, sub_data.data(), sub_data.length());
        start = end;
        end = end + num;
        MKSLOG_BLUE("发送下载数据");
        // usleep(45000);
        // usleep(25000);
    }
    
    
    
    // write(fd, data.data(), data.length());
    // usleep(5000);
    /*
    std::string data_1 = data.substr(0, 512);
    std::string data_2 = data.substr(512, 1024);
    std::string data_3 = data.substr(1024, 1536);
    std::string data_4 = data.substr(1536, 2048);
    std::string data_5 = data.substr(2048, 2560);
    std::string data_6 = data.substr(2560, 3072);
    std::string data_7 = data.substr(3072, 3584);
    std::string data_8 = data.substr(3584, 4096);
    */
    // write(fd, data.data(), data.length());
    // usleep(500);
    // write(fd, data_1.data(), data.length());
    // usleep(50000);
    // write(fd, data_2.data(), data.length());
    // usleep(50000);
    // write(fd, data_3.data(), data.length());
    // usleep(50000);
    // write(fd, data_4.data(), data.length());
    // usleep(50000);
    // write(fd, data_5.data(), data.length());
    // usleep(50000);
    // write(fd, data_6.data(), data.length());
    // usleep(50000);
    // write(fd, data_7.data(), data.length());
    // usleep(50000);
    // write(fd, data_8.data(), data.length());
    // usleep(50000);

}


void send_cmd_vid_en(int fd, std::string obj, int value) {
    std::string cmd = obj + ".en=" + std::to_string(value) + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

void send_cmd_bauds(int fd, int bauds) {
    std::string cmd = "bauds=" + std::to_string(bauds) + "\xff\xff\xff";
    // MKSLOG_YELLOW("%s", cmd.data());
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

void send_cmd_baud(int fd, int baud) {
    std::string cmd = "baud=" + std::to_string(baud) + "\xff\xff\xff";
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}

void send_var_value(int fd, std::string var, int value) {
    std::string cmd = var + "=" + std::to_string(value) + "\xff\xff\xff";
    tcdrain(fd);
    write(fd, cmd.data(), cmd.length());
}