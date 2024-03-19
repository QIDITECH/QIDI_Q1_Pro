#ifndef MKS_LOG_H
#define MKS_LOG_H

#include <iostream>
#include <stdio.h>

#define LOG_RED             "\033[31;1m"
#define LOG_YELLOW          "\033[0;33m"
#define LOG_GREEN           "\033[0;32m"
#define LOG_BLUE            "\033[0;34m"
#define LOG_PURPLE          "\033[0;35m"
#define LOG_SKYBLUE         "\033[0;36m"
#define LOG_HIGHLIGHT       "\033[7m\033[5m"
#define LOG_END             "\033[0m"

#if 0
#define MKSLOG(fmt, ...) printf(("[%s][%s] %s: %d\n" fmt "\n"), __DATE__, __TIME__, __FILE__, __LINE__, ##__VA_ARGS__)
#define MKSLOG_RED(fmt, ...) printf(("%s[%s][%s] %s: %d\n" fmt "\n%s"), LOG_RED, __DATE__, __TIME__, __FILE__, __LINE__, ##__VA_ARGS__, LOG_END)
#define MKSLOG_YELLOW(fmt, ...) printf(("%s[%s][%s] %s: %d\n" fmt "\n%s"), LOG_YELLOW, __DATE__, __TIME__, __FILE__, __LINE__, ##__VA_ARGS__, LOG_END)
#define MKSLOG_BLUE(fmt, ...) printf(("%s[%s][%s] %s: %d\n" fmt "\n%s"), LOG_SKYBLUE, __DATE__, __TIME__, __FILE__, __LINE__, ##__VA_ARGS__, LOG_END)
#define MKSLOG_GREEN(fmt, ...) printf(("%s[%s][%s] %s: %d\n" fmt "\n%s"), LOG_GREEN, __DATE__, __TIME__, __FILE__, __LINE__, ##__VA_ARGS__, LOG_END)
#else
#define MKSLOG(fmt, ...) printf((fmt "\n"), ##__VA_ARGS__)
#define MKSLOG_RED(fmt, ...) printf(("%s" fmt "\n%s"), LOG_RED, ##__VA_ARGS__, LOG_END)
#define MKSLOG_YELLOW(fmt, ...) printf(("%s" fmt "\n%s"), LOG_YELLOW, ##__VA_ARGS__, LOG_END)
#define MKSLOG_BLUE(fmt, ...) printf(("%s" fmt "\n%s"), LOG_SKYBLUE, ##__VA_ARGS__, LOG_END)
#define MKSLOG_GREEN(fmt, ...) printf(("%s" fmt "\n%s"), LOG_GREEN, ##__VA_ARGS__, LOG_END)
#define MKSLOG_HIGHLIGHT(fmt, ...) printf(("%s" fmt "\n%s"), LOG_HIGHLIGHT, ##__VA_ARGS__, LOG_END)
#endif
#endif
