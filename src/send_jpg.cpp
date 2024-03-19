#include "../include/mks_log.h"
#include "../include/send_msg.h"
#include "../include/MakerbaseSerial.h"
#include "../include/send_jpg.h"
#include "../include/mks_file.h"

// #define BLOCK_SIZE 2048
// #define HEADER_SIZE 12

extern int tty_fd;


bool get_0xfe = false;
bool get_0x06 = false;
bool get_0x05 = false;
bool get_0xfd = false;
bool get_0x04 = false;
bool get_0x24 = false;

bool have_64_jpg[6];
std::string have_64_png_path[6];
bool begin_show_64_jpg;
bool begin_show_160_jpg;
bool begin_show_192_jpg;
bool show_192_jpg_complete = true;
std::string jpg_160_path;



int time_differ(int duration, int start_time) {
    int tmp;
    tmp = time(NULL);
    tmp = tmp - start_time;
    if (tmp > duration) {
        return 1;
    } else {
        return 0;
    }
}

int time_differ_ms(long duration, long start_time) {
    struct timeval tv1;
    gettimeofday(&tv1, NULL);
    long tmp;
    tmp = tv1.tv_sec * 1000 + tv1.tv_usec / 1000;
    tmp = tmp - start_time;
    if (tmp > duration) {
        return 1;
    } else {
        return 0;
    }
}

extern std::string input_path;
extern int input_size;
extern bool start_path;
// 刷预览图线程
void *sent_jpg_thread_handle(void *arg)
{
    std::string png_path = "";
    std::string ram_path = "";
    std::string jpg_path = "";

    while (1)
    {
        // 刷新小预览图
        if (begin_show_64_jpg)
        {
            begin_show_64_jpg = false;
            for (uint i = 0; i < 6; i++)
            {
                if (have_64_jpg[i] == true)
                {
                    send_cmd_tsw(tty_fd, "255", "0"); // 禁止屏幕触摸
                    MKSLOG_BLUE("不能触摸屏幕");
                    usleep(50500 + i * 500);
                    ram_path = std::string("ram/") + std::string("file") + std::to_string(i) + std::string(".jpg");
                    // jpg_path = page_files_root_path + std::string("/") + page_files_path + std::string("/.thumbs/") + page_files_list_show_name[i].substr(0, page_files_list_show_name[i].find(".gcode")) + std::string("-64x64.jpg");
                    // output_jpg(have_64_png_path[i], input_size);
                    // std::cout << extractFileName(have_64_png_path[i]) << std::endl;
                    // if (i < 4) {
                    //     output_jpg(have_64_png_path[i], 112);
                    // } else {
                    //     output_jpg(have_64_png_path[i], 160);
                    // }
                    // jpg_path = "/home/mks/tjc.jpg";
                    jpg_path = "/home/mks/gcode_files/" + have_64_png_path[i] ;
                    // send_cmd_baud(tty_fd, 230400);
                    std::cout << jpg_path << std::endl;
                    // usleep(50000);
                    // set_option(tty_fd, 230400, 8, 'N', 1);
                    // if (sent_jpg_to_tjc(ram_path, jpg_path) != true)
                    // {
                    sent_jpg_to_tjc(ram_path, jpg_path);
                    // }
                    // send_cmd_baud(tty_fd, 115200);
                    // usleep(50000);
                    // set_option(tty_fd, 115200, 8, 'N', 1);
                    have_64_jpg[i] = false;
                    // system("rm /home/mks/tjc.jpg");
                    send_cmd_tsw(tty_fd, "255", "1"); // 使能屏幕触摸
                    MKSLOG_BLUE("可以触摸屏幕");
                }
            }
        }

        // 刷新预览界面预览图
        // if (begin_show_192_jpg)
        // {
        //     // jpg_path = page_files_root_path + std::string("/") + page_files_path + std::string("/.thumbs/") + current_select_file.substr(0, current_select_file.find(".gcode")) + std::string("-192x192.jpg");
        //     jpg_path = "/root/tjc.jpg";
        //     while (1)
        //     {
        //         if (sent_jpg_to_tjc("ram/file0.jpg", jpg_path) == true)
        //         {
        //             break;
        //         }
        //         delet_pic("ram/file0.jpg");
        //         usleep(500000);
        //     }
        //     begin_show_192_jpg = false;
        //     show_192_jpg_complete = true;
        // }

        // 刷新打印界面预览图
        // if (begin_show_160_jpg)
        // {           
        //     while (1)
        //     {
        //         if (sent_jpg_to_tjc("ram/160.jpg", jpg_160_path) == true)
        //         {
        //             break;
        //         }
        //         delet_pic("ram/160.jpg");
        //         usleep(500000);
        //     }
        //     begin_show_160_jpg = false;
        // }

        // 提前删除预览界面的预览图，防止进入预览页面时闪一下
        // if (current_page_id != TJC_PAGE_PREVIEW)
        // {
        //     if (show_192_jpg_complete)
        //     {
        //         delet_pic("ram/192.jpg");
        //         show_192_jpg_complete = false;
        //     }
        // }

        usleep(60000);
    }
}


// 删除图片
void delet_pic(std::string ram_path)
{
    send_cmd_delfile(tty_fd, ram_path);
}

// 获取文件大小
long getFileSize(FILE *file)
{
    long fileSize = -1;
    if (file != NULL)
    {
        if (fseek(file, 0L, SEEK_END) == 0)
        {
            fileSize = ftell(file);
        }
        rewind(file);
    }
    return fileSize;
}


// 计算单个数据的校验码
unsigned int calccrc(unsigned char crcbuf, unsigned int crc)
{
    unsigned char i;

    crc = crc ^ crcbuf;

    for (i = 0; i < 8; i++)
    {
        unsigned char chk;

        chk = crc & 1;

        crc = crc >> 1;

        crc = crc & 0x7fff;

        if (chk == 1)

            crc = crc ^ 0xa001;

        crc = crc & 0xffff;
    }

    return crc;
}

// 计算一串数据的校验码
unsigned int check_crc(unsigned char *buf, unsigned int len)
{
    unsigned char hi, lo;

    unsigned int i;

    unsigned int crc;

    crc = 0xFFFF;

    for (i = 0; i < len; i++)
    {
        crc = calccrc(*buf, crc);

        buf++;
    }

    hi = crc % 256;

    lo = crc / 256;

    crc = (hi << 8) | lo;

    return crc;
}

// 删除所有小预览图
void delete_small_jpg()
{
    delet_pic("ram/file0.jpg");
    usleep(56000);
    delet_pic("ram/file1.jpg");
    usleep(56000);
    delet_pic("ram/file2.jpg");
    usleep(56000);
    delet_pic("ram/file3.jpg");
    usleep(56000);

}

// 发送图片
bool sent_jpg_to_tjc(std::string ram_path, std::string jpg_path)
{
    FILE *file;
    long filesize;
    int file_res = 0;
    static int start_time;
    struct timeval tv1;
    long resent_time;

    uint16_t head_id = 0;
    uint8_t head_buf[12] = {0x3A, 0xA1, 0xBB, 0x44, 0x7F, 0xFF, 0xFE, 0x01, 0x00, 0x00, 0xDC, 0x07};
    uint8_t exit_buf[12] = {0x3A, 0xA1, 0xBB, 0x44, 0x7F, 0xFF, 0xFE, 0x00, 0xFF, 0xFF, 0x00, 0x00};
    uint8_t read_buf[BLOCK_SIZE - sizeof(head_buf)] = {0}; // 最大4096 - 帧头

    // MKSLOG_BLUE("图片路径为：%s", jpg_path.c_str());

    file = fopen(jpg_path.c_str(), "r");
    if (file == NULL)
    {
        MKSLOG_BLUE("文件打开失败");
        return true;
    }

    filesize = getFileSize(file);
    // MKSLOG_BLUE("文件大小为: %ld字节", filesize);

    // 发送透传指令
    send_cmd_twfile(tty_fd, ram_path, std::to_string(filesize));
    // 延时，等待收到0xfe+结束符
    usleep(105000);
    // 循环发送包头+4096个数据，直到文件结尾
    start_time = time(NULL);
    while (1)
    {
        memset(read_buf, 0x00, sizeof(read_buf));

        file_res = fread(read_buf, 1, (sizeof(read_buf) - 2), file); // 留两字节存放校验码
        // MKSLOG_BLUE("读取大小为: %ld字节", file_res);
        if (file_res <= 0)
        {
            break;
        }

        // 发送包头
        head_buf[8] = head_id & 0xff;
        head_buf[9] = head_id >> 8;
        head_buf[10] = (file_res+2) & 0xff;
        head_buf[11] = (file_res+2) >> 8;
        write(tty_fd, head_buf, sizeof(head_buf));
        // 打印头数据
        // for (int i = 0; i < sizeof(head_buf); i++)
        // {
        //     printf("%02X", head_buf[i]);
        // }
        // printf("\n");

        // 获取校验码，存放在最后两字节
        uint16_t crc_val = check_crc(read_buf, file_res);
        read_buf[file_res] = crc_val >> 8;
        read_buf[file_res + 1] = crc_val & 0xff;
        // MKSLOG_BLUE("校验码为: %02X, %02X", crc_val >> 8, crc_val & 0xff);
        // tcdrain(tty_fd);
        // 发送数据
        write(tty_fd, read_buf, file_res+2);
        // tcdrain(tty_fd);

        // 打印数据
        // for (int i = 0; i < file_res+2; i++)
        // {
        //     printf("%02X", read_buf[i]);
        // }
        // printf("\n");

        // 返回0x05表示这一帧写入成功
        start_time = time(NULL);
        gettimeofday(&tv1, NULL);
        resent_time = tv1.tv_sec * 1000 + tv1.tv_usec / 1000;
        get_0x05 = false;
        get_0xfd = false;
        get_0x04 = false;
        while (!get_0x05 && !get_0xfd)
        {
            usleep(80000);
            // 返回0x04表示这一帧写入失败
            if (get_0x04)
            {
                // 打印退出透传模式
                // tcdrain(tty_fd);
                write(tty_fd, exit_buf, sizeof(exit_buf));
                // tcdrain(tty_fd);
                for (int i = 0; i < sizeof(exit_buf); i++)
                {
                    printf("%02X", exit_buf[i]);
                }
                printf("\n");
                write(tty_fd, exit_buf, sizeof(exit_buf));
                // tcdrain(tty_fd);
                MKSLOG_GREEN("返回0x04，失败");
                fclose(file);
                return false;
            }
            // 超时重发机制
            gettimeofday(&tv1, NULL);
            if (time_differ_ms(800, resent_time) && get_0x24 == false)
            {
                gettimeofday(&tv1, NULL);
                resent_time = tv1.tv_sec * 1000 + tv1.tv_usec / 1000;
                // 重发数据帧
                write(tty_fd, head_buf, sizeof(head_buf));
                // tcdrain(tty_fd);
                write(tty_fd, read_buf, file_res+2);
                MKSLOG_GREEN("返回超时，重发数据包");
            }

            // 超时机制
            if (time_differ(4, start_time))
            {
                write(tty_fd, exit_buf, sizeof(exit_buf));
                for (int i = 0; i < sizeof(exit_buf); i++)
                {
                    printf("%02X", exit_buf[i]);
                }
                printf("\n");
                write(tty_fd, exit_buf, sizeof(exit_buf));
                // tcdrain(tty_fd);
                write(tty_fd, exit_buf, sizeof(exit_buf));
                // tcdrain(tty_fd);
                MKSLOG_GREEN("写数据帧超时，失败");
                fclose(file);
                return false;
            }
            
            // CLL 缓冲区溢出时停止发送
            if (get_0x24 == true) {
                sleep(4);
                get_0x24 = false;
                return false;
            }
        }

        

        // 修改包头ID
        head_id++;
        // 超时机制
        if (time_differ(6, start_time))
        {
            write(tty_fd, exit_buf, sizeof(exit_buf));
            for (int i = 0; i < sizeof(exit_buf); i++)
            {
                printf("%02X", exit_buf[i]);
            }
            printf("\n");
            write(tty_fd, exit_buf, sizeof(exit_buf));
            write(tty_fd, exit_buf, sizeof(exit_buf));
            MKSLOG_GREEN("写图片超时，失败");
            fclose(file);
            break;
        }
    }

    MKSLOG_GREEN("成功写入图片到内存");
    fclose(file);
    return true;
}



void printHex(const uint8_t *data, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        printf("%02X", data[i]);
    }
    // printf("\n");
}





// bool mks_send_jpg(char *path, int fd, int index) {
//     // FILE *input_file, *output_file;
//     FILE *input_file;
//     uint8_t head_buf[HEADER_SIZE] = {0x3A, 0xA1, 0xBB, 0x44, 0x7F, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0xDC, 0x07};
//     uint8_t exit_buf[12] = {0x3A, 0xA1, 0xBB, 0x44, 0x7F, 0xFF, 0xFE, 0x00, 0xFF, 0xFF, 0x00, 0x00};
//     uint8_t buffer[BLOCK_SIZE - HEADER_SIZE]; // 数据块的大小减去帧头的大小
//     size_t bytes_read;
//     long file_size;

//     uint16_t head_id = 0;

//     input_file = fopen(path, "rb");
//     if (input_file == NULL) {
//         perror("Error opening input file");
//         return false;
//     }

//     // 获取文件大小
//     fseek(input_file, 0, SEEK_END);
//     file_size = ftell(input_file);
//     fseek(input_file, 0, SEEK_SET);

//     // char cmd[32];
//     // sprintf(cmd, "twfile \"ram/file%d.jpg\",%ld\xff\xff\xff", index, file_size);
//     // write(fd, cmd, sizeof(cmd));
//     // usleep(100000);

//     char ram_path[32];
//     sprintf(ram_path, "ram/file%d.jpg", index);
//     std::string to_out = ram_path;

//     send_cmd_twfile(fd, to_out, std::to_string(file_size));

    /*
    sleep(5);
  
    // 逐块读取输入文件，动态拼接帧头和数据，然后写入输出文件
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), input_file)) > 0) {
        head_buf[8] = head_id & 0xff;
        head_buf[9] = head_id >> 8;
        head_buf[10] = bytes_read & 0xff;
        head_buf[11] = bytes_read >> 8;
        // 写入帧头
        write(fd, head_buf, sizeof(head_buf));
        tcdrain(fd);
        // 写入读取的数据块
        write(fd, buffer, bytes_read);
        // 写入帧头
        // fwrite(head_buf, 1, sizeof(head_buf), output_file);
        // 写入读取的数据块
        // fwrite(buffer, 1, bytes_read, output_file);
        printf("Hexadecimal content written:\n");
        printHex(head_buf, sizeof(head_buf));
        printHex(buffer, bytes_read);
        printf("\n");
        head_id++;
        usleep(2000000);
    }
    */

    
//     static int start_time;
//     struct timeval tv1;
//     long resent_time;
//     start_time = time(NULL);
//     while (1) {
//         bytes_read = fread(buffer, 1, sizeof(buffer), input_file);
//         head_buf[8] = head_id & 0xff;
//         head_buf[9] = head_id >> 8;
//         head_buf[10] = bytes_read & 0xff;
//         head_buf[11] = bytes_read >> 8;
//         write(fd, head_buf, sizeof(head_buf));
//         tcdrain(fd);
//         write(fd, buffer, bytes_read);
//         printf("Hexadecimal content written:\n");
//         printHex(head_buf, sizeof(head_buf));
//         printHex(buffer, bytes_read);
//         printf("\n");

//         // 返回0x05表示这一帧写入成功
//         start_time = time(NULL);
//         gettimeofday(&tv1, NULL);
//         resent_time = tv1.tv_sec * 1000 + tv1.tv_usec / 1000;
//         get_0x05 = false;
//         get_0xfd = false;
//         get_0x04 = false;
//         while (!get_0x05 && !get_0xfd)
//         {
//             usleep(100000);
//             // 返回0x04表示这一帧写入失败
//             if (get_0x04)
//             {
//                 // 打印退出透传模式
//                 write(fd, exit_buf, sizeof(exit_buf));
//                 for (int i = 0; i < sizeof(exit_buf); i++)
//                 {
//                     printf("%02X", exit_buf[i]);
//                 }
//                 printf("\n");
//                 write(fd, exit_buf, sizeof(exit_buf));
//                 MKSLOG_YELLOW("返回0x04，失败");
//                 fclose(input_file);
//                 return false;
//             }
//             // 超时重发机制
//             gettimeofday(&tv1, NULL);
//             if (time_differ_ms(6000, resent_time))
//             {
//                 gettimeofday(&tv1, NULL);
//                 resent_time = tv1.tv_sec * 1000 + tv1.tv_usec / 1000;
//                 // 重发数据帧
//                 write(fd, head_buf, sizeof(head_buf));
//                 write(fd, buffer, bytes_read);
//                 MKSLOG_YELLOW("返回超时，重发数据包");
//             }
//             // 超时机制
//             if (time_differ(3, start_time))
//             {
//                 write(fd, exit_buf, sizeof(exit_buf));
//                 for (int i = 0; i < sizeof(exit_buf); i++)
//                 {
//                     printf("%02X", exit_buf[i]);
//                 }
//                 printf("\n");
//                 write(fd, exit_buf, sizeof(exit_buf));
//                 write(fd, exit_buf, sizeof(exit_buf));
//                 MKSLOG_YELLOW("写数据帧超时，失败");
//                 fclose(input_file);
//                 return false;
//             }
//         }

//         head_id++;
//         // 超时机制
//         if (time_differ(10, start_time))
//         {
//             write(fd, exit_buf, sizeof(exit_buf));
//             for (int i = 0; i < sizeof(exit_buf); i++)
//             {
//                 printf("%02X", exit_buf[i]);
//             }
//             printf("\n");
//             write(fd, exit_buf, sizeof(exit_buf));
//             write(fd, exit_buf, sizeof(exit_buf));
//             MKSLOG_YELLOW("写图片超时，失败");
//             fclose(input_file);
//             break;
//         }
//     }
    

//     // 关闭文件
//     fclose(input_file);
//     MKSLOG_YELLOW("成功写入图片到内存");
//     printf("File processing complete.\n");
//     return true;
// }

// bool begin_show_64_jpg;
// bool begin_show_160_jpg;
// bool begin_show_192_jpg;
// bool show_192_jpg_complete = true;
// bool have_64_jpg[4];

// void *sent_jpg_thread_handle(void *arg) {
//     std::string png_path = "";
//     std::string ram_path = "";
//     std::string jpg_path = "";
//     while (1) {
//         // 刷新小预览图
//         if (begin_show_64_jpg) {
//             begin_show_64_jpg = false;
//             for (uint i = 0; i < 4; i++) {
//                 if (have_64_jpg[i] == true) {
//                     /*
//                     if (mks_send_jpg(jpg_path.c_str(), tty_fd, i) != false) {

//                     } 
//                     */
//                 }
//             }
//         }
//         usleep(50000);
//     }
// }

