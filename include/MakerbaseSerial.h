#ifndef MAKERBASE_SERIAL_H
#define MAKERBASE_SERIAL_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <termios.h>

int set_option(int fd, int baudrate, int bits, unsigned char parity, unsigned char stopbit);

#endif
