#include <../include/MakerbaseSerial.h>
#include <time.h>

int set_option(int fd, int baudrate, int bits, unsigned char parity, unsigned char stopbit) {

    // time_t start_time = 0, end_time = 0;

    // time(&start_time);
    struct termios newtio, oldtio;

    if (tcgetattr(fd, &oldtio) != 0) {
        // printf("读取串口参数发生错误\n");
        return -1;
    }

    memset(&newtio, 0, sizeof(struct termios));
    cfmakeraw(&newtio);                         // 设置为原始模式

    /* 使能接收 */
    newtio.c_cflag |= CREAD;

    speed_t speed;

    // 数据位相关的比特位清零
    newtio.c_cflag &= ~CSIZE;

    switch (bits)
    {
    case 7:
        newtio.c_cflag |= CS7;
        // printf("7数据位\n");
        break;

    case 8:
        newtio.c_cflag |= CS8;
        // printf("8数据位\n");
        break;
    
    default:
        newtio.c_cflag |= CS8;
        // printf("8数据位\n");
        break;
    }

    switch (parity)
    {
    case 'O':
    case 'o':
        newtio.c_cflag |= (PARENB | PARODD);
        newtio.c_cflag |= INPCK;
        // printf("奇校验\n");
        break;

    case 'E':
    case 'e':
        newtio.c_cflag |= PARENB;
        newtio.c_cflag &= ~PARODD;
        newtio.c_iflag |= INPCK;
        // printf("偶校验\n");
        break;

    case 'N':
    default:
        newtio.c_cflag &= ~PARENB;
        newtio.c_iflag &= ~INPCK;
        // printf("设置为无校验");
        break;
    }

    // 设置波特率
    switch (baudrate)
    {
    case 1200:
        speed = B1200;
        // printf("波特率为1200\n");
        break;
    
    case 1800:
        speed = B1800;
        // printf("波特率为1800\n");
        break;

    case 2400:
        speed = B2400;
        // printf("波特率为2400\n");
        break;

    case 4800:
        speed = B4800;
        // printf("波特率为4800\n");
        break;

    case 9600:
        speed = B9600;
        printf("波特率为9600\n");
        break;

    case 19200:
        speed = B19200;
        // printf("波特率为19200\n");
        break;

    case 38400:
        speed = B38400;
        // printf("波特率为38400\n");
        break;

    case 57600:
        speed = B57600;
        // printf("波特率为57600\n");
        break;

    case 115200:
        speed = B115200;
        // printf("波特率为115200\n");
        break;
    
    case 230400:
        speed = B230400;
        // printf("波特率为230400\n");
        break;

    case 460800:
        speed = B460800;
        // printf("波特率为460800\n");
        break;

    case 500000:
        speed = B500000;
        // printf("波特率为500000\n");
        break;

    case 921600:
        speed = B921600;
        printf("波特率为921600\n");
        break;
    
    default:
        speed = B115200;
        // printf("波特率为115200\n");
        break;
    }

    if (0 > cfsetspeed(&newtio, speed)) {
        // printf("设置波特率失败\n");
        return -1;
    }

    // 设置停止位
    switch (stopbit) {
    case 1:
        newtio.c_cflag &= ~CSTOPB;
        // printf("设置1个停止位\n");
        break;

    case 2:
        newtio.c_cflag |= CSTOPB;
        // printf("设置2个停止位\n");
        break;

    default:
        newtio.c_cflag &= ~CSTOPB;
        // printf("默认设置1个停止位\n");
        break;
    }

    // 将MIN和TIME设置为0
    newtio.c_cc[VTIME] = 0;
    newtio.c_cc[VMIN] = 0;

    // 清空缓冲区
    if (0 > tcflush(fd, TCIOFLUSH)) {
        // printf("清空缓冲区失败\n");
        return -1;
    }

    // 写入配置、使配置生效
    if (0 > tcsetattr(fd, TCSANOW, &newtio)) {
        // printf("配置串口失败");
        return -1;
    }

    // time(&end_time);
    // printf("设置波特率使用的时间: %f\n", difftime(end_time, start_time));

    return 0;
}

char *create_command(char *cmd) {
    char end[4] = {(char)0xff, (char)0xff, (char)0xff, 0};
    return strcat(cmd, end);
}
