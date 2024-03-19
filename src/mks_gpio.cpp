#include "../include/mks_gpio.h"
#include "../include/mks_log.h"
#include "../include/event.h"

#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>

// static char gpio_path[64];

static int gpio_config(const char *attr, const char *val, const char *gpio_path) {
    char file_path[64];
    int len;
    int fd;

    sprintf(file_path, "%s/%s", gpio_path, attr);
    if (0 > (fd = open(file_path, O_WRONLY))) {
        perror("open error");
        return fd;
    }

    len = strlen(val);
    if (len != write(fd, val, len)) {
        perror("write error");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}

// 32+2*8+5=53
int set_GPIO1_C5_low() {
    if (access("/sys/class/gpio/gpio53", F_OK)) {
        int fd;
        int len;
        char arg[] = "53";

        if (0 > (fd = open("/sys/class/gpio/export", O_WRONLY))) {
            perror("open error");
            return -1;
        }
        
        len = strlen(arg);
        if (len != write(fd, arg, len)) {
            perror("write error");
            close(fd);
            return -1;
        }
        close(fd);      // 关闭文件
    }

    /* 配置为输出模式 */
    if (gpio_config("direction", "out", "/sys/class/gpio/gpio53")) {
        printf("配置输出模式出错\n");
        return -1;
    }

    /* 极性设置 */
    if (gpio_config("active_low", "0", "/sys/class/gpio/gpio53")) {
        printf("配置极性设置出错\n");
        return -1;
    }

    /* 控制GPIO输出低电平 */
    if (gpio_config("value", "0", "/sys/class/gpio/gpio53")) {
        printf("配置输出低电平出错\n");
        return -1;
    }

    return 0;
}

int set_GPIO1_C5_high() {
    if (access("/sys/class/gpio/gpio53", F_OK)) {
        int fd;
        int len;
        char arg[] = "53";

        if (0 > (fd = open("/sys/class/gpio/export", O_WRONLY))) {
            perror("open error");
            return -1;
        }
        
        len = strlen(arg);
        if (len != write(fd, arg, len)) {
            perror("write error");
            close(fd);
            return -1;
        }
        close(fd);      // 关闭文件
    }

    /* 配置为输出模式 */
    if (gpio_config("direction", "out", "/sys/class/gpio/gpio53")) {
        printf("配置输出模式出错\n");
        return -1;
    }

    /* 极性设置 */
    if (gpio_config("active_low", "0", "/sys/class/gpio/gpio53")) {
        printf("配置极性设置出错\n");
        return -1;
    }

    /* 控制GPIO输出高电平 */
    if (gpio_config("value", "1", "/sys/class/gpio/gpio53")) {
        printf("配置输出低电平出错\n");
        return -1;
    }

    return 0;
}

// 32+1*8+2=42
int init_GPIO1_B2() {
    if (access("/sys/class/gpio/gpio42", F_OK)) {
        int fd;
        int len;
        char arg[] = "42";

        if (0 > (fd = open("/sys/class/gpio/export", O_WRONLY))) {
            perror("open error");
            return -1;
        }

        len = strlen(arg);
        if (len != write(fd, arg, len)) {
            perror("write error");
            close(fd);
            exit(-1);
        }

        close(fd);
    }

    /* 配置为输入模式 */
    if (gpio_config("direction", "in", "/sys/class/gpio/gpio42")) {
        return -1;
    }

    /* 极性设置 */
    if (gpio_config("active_low", "0", "/sys/class/gpio/gpio42")) {
        return -1;
    }

    /* 配置为非中断方式 */
    if (gpio_config("edge", "falling", "/sys/class/gpio/gpio42")) {
        return -1;
    }

    return 0;

}

void *monitor_GPIO1_B2(void *arg) {
    struct pollfd pfd;
    int ret;
    char val;

    /* 打开 value 属性文件 */
    if (0 > (pfd.fd = open("/sys/class/gpio/gpio42/value", O_RDONLY))) {
        perror("打开value出错");
        // return ;
    }

    /* 调用 poll */
    pfd.events = POLLPRI;       // 只关心高优先级数据可读（中断）

    read(pfd.fd, &val, 1);      // 先读取一次清除状态

    for (;;) {
        ret = poll(&pfd, 1, -1);
        if (0 > ret) {
            perror("poll error");
            // return ;
        } else if (0 == ret) {
            fprintf(stderr, "poll timeout.\n");
            continue;
        }

        /* 检验高优先级数据是否可读 */
        if (pfd.revents & POLLPRI) {
            if (0 > lseek(pfd.fd, 0, SEEK_SET)) {       // 将读位置移动到头部
                perror("lseek error");
                // return ;
            }

            if (0 > read(pfd.fd, &val, 1)) {
                perror("read error");
                // return ;
            }
            if ((val - '0') == 0) {
                // 检测到低电平之后要执行的
                // set_GPIO1_C5_low();
                //4.4.1 CLL 屏蔽关机页面
                system("echo TEST > /root/TESTGPIO; sync");
                //go_to_page_power_off();
                system("sync; shutdown -h now;");
            }
        }
        usleep(110000);      // 检测电平减少一点
    }
}

int init_GPIO1_C3() {
    if (access("/sys/class/gpio/gpio51", F_OK)) {
        int fd;
        int len;
        char arg[] = "51";

        if (0 > (fd = open("/sys/class/gpio/export", O_WRONLY))) {
            perror("open error");
            return -1;
        }

        len = strlen(arg);
        if (len != write(fd, arg, len)) {
            perror("write error");
            close(fd);
            exit(-1);
        }

        close(fd);
    }

    /* 配置为输入模式 */
    if (gpio_config("direction", "in", "/sys/class/gpio/gpio51")) {
        return -1;
    }

    /* 极性设置 */
    if (gpio_config("active_low", "0", "/sys/class/gpio/gpio51")) {
        return -1;
    }

    /* 配置为非中断方式 */
    if (gpio_config("edge", "both", "/sys/class/gpio/gpio51")) {
    // if (gpio_config("edge", "rising", "/sys/class/gpio/gpio51")) {
        return -1;
    }

    return 0;
}

void *monitor_GPIO1_C3(void *arg) {
    struct pollfd pfd;
    int ret;
    char val;
    int cnt = 0;
    const int debounce_limit = 5;               // 防抖计数器限制
    const int debounce_interval = 30;           // 防抖时间间隔（毫秒）

    /* 打开 value 属性文件 */
    if (0 > (pfd.fd = open("/sys/class/gpio/gpio51/value", O_RDONLY))) {
        perror("打开value出错");
        // return ;
    }

    /* 调用 poll */
    pfd.events = POLLPRI;       // 只关心高优先级数据可读（中断）

    read(pfd.fd, &val, 1);      // 先读取一次清除状态

    for (;;) {
        ret = poll(&pfd, 1, -1);
        if (0 > ret) {
            perror("poll error");
            // return ;
        } else if (0 == ret) {
            fprintf(stderr, "poll timeout.\n");
            continue;
        }

        /* 检验高优先级数据是否可读 */
        if (pfd.revents & POLLPRI) {
            if (0 > lseek(pfd.fd, 0, SEEK_SET)) {       // 将读位置移动到头部
                perror("lseek error");
                // return ;
            }

            if (0 > read(pfd.fd, &val, 1)) {
                perror("read error");
                // return ;
            }
            if ((val - '0') == 1) {
                if (cnt >= debounce_limit) {
                // 检测到低电平之后要执行的
                // set_GPIO1_C5_low();
                // system("echo TEST > /root/TESTGPIO; sync");
                go_to_page_power_off();
                shutdown_mcu();
                set_GPIO1_B3_low();
                system("sync");
                // system("sync; shutdown -h now;");
                cnt = 0;
                } else {
                    cnt++;
                    usleep(debounce_interval * 1000);
                }
            }else {
                cnt = 0;
            }
            MKSLOG_BLUE("cnt=%d",cnt);
        }
        usleep(110000);
        // 检测电平减少一点
    }
}

int set_GPIO1_B3_low() {
    if (access("/sys/class/gpio/gpio43", F_OK)) {
        int fd;
        int len;
        char arg[] = "43";

        if (0 > (fd = open("/sys/class/gpio/export", O_WRONLY))) {
            perror("open error");
            return -1;
        }
        
        len = strlen(arg);
        if (len != write(fd, arg, len)) {
            perror("write error");
            close(fd);
            return -1;
        }
        close(fd);      // 关闭文件
    }

    /* 配置为输出模式 */
    if (gpio_config("direction", "out", "/sys/class/gpio/gpio43")) {
        printf("配置输出模式出错\n");
        return -1;
    }

    /* 极性设置 */
    if (gpio_config("active_low", "0", "/sys/class/gpio/gpio43")) {
        printf("配置极性设置出错\n");
        return -1;
    }

    /* 控制GPIO输出低电平 */
    if (gpio_config("value", "0", "/sys/class/gpio/gpio43")) {
        printf("配置输出低电平出错\n");
        return -1;
    }

    return 0;
}

