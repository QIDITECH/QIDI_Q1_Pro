#ifndef MAKERBASE_COMMAND_H
#define MAKERBASE_COMMAND_H

#include <map>

/**
 * @brief 接收的执行与API映射定义
 * 
 */
static std::map<int, std::string> method2command = {

    {0x01, "server.connection.identify"},
    {0x02, "server.websocket.id"},
    {0x03, "printer.info"},
    {0x04, "printer.emergency_stop"},
    {0x05, "printer.restart"},
    {0x06, "printer.firmware_restart"},

    {0x11, "printer.objects.list"},
    {0x12, "printer.objects.query"},
    {0x13, "printer.objects.subscribe"},
    {0x14, "printer.query_endstops.status"},
    {0x15, "server.info"},
    {0x16, "server.config"},
    {0x17, "server.temperature_store"},
    {0x18, "server.gcode_store"},
    {0x19, "server.restart"},

    {0x21, "printer.gcode.script"},
    {0x22, "printer.gcode.help"},

    {0x31, "printer.print.start"},
    {0x32, "printer.print.pause"},
    {0x33, "printer.print.resume"},
    {0x34, "printer.print.cancel"},

    {0x41, "machine.system_info"},
    {0x42, "machine.shutdown"},
    {0x43, "machine.reboot"},
    {0x44, "machine.services.restart"},
    {0x45, "machine.services.stop"},
    {0x46, "machine.services.start"},
    {0x47, "machine.proc_stats"},

    {0x51, "server.files.list"},
    {0x52, "server.files.metadata"},
    {0x53, "server.files.get_directory"},
    {0x54, "server.files.post_directory"},
    {0x55, "server.files.delete_directory"},
    {0x56, "server.files.move"},
    {0x57, "server.files.copy"},
    // 0x58     留给 文件下载 功能，只有http请求可以用
    // 0x59     留给 上传文件 功能，只有http请求可以用
    {0x5a, "server.files.delete_file"},
    // 0x5b     留给 下载klippy.log 功能，只有http请求可以用
    // 0x5c     留给 下载moonraker.log 功能，只有http请求可以用

    // 0x61     登录用户
    // 0x62     注销当前用户
    // 0x63     获取当前用户
    // 0x64     创建用户
    // 0x65     删除用户
    // 0x66     列出可用用户
    // 0x67     重置用户密码
    // 0x68     刷新 JSON Web 令牌
    // 0x69     生成 Oneshot 令牌
    // 0x6a     检索有关授权端点的信息
    // 0x6b     获取当前 API 密钥
    // 0x6c     生成新的 API 密钥

    {0x71, "server.database.list"},
    {0x72, "server.database.get_item"},
    {0x73, "server.database.post_item"},
    {0x74, "server.database.delete_item"},

    {0x81, "server.job_queue.status"},
    {0x82, "server.job_queue.post_job"},
    {0x83, "server.job_queue.delete_job"},
    {0x84, "server.job_queue.pause"},
    {0x85, "server.job_queue.start"},

    {0x86, "server.announcements.list"},
    {0x87, "server.announcements.update"},
    {0x88, "server.announcements.dismiss"},
    {0x89, "server.announcements.feeds"},
    {0x8a, "server.announcements.post_feed"},
    {0x8b, "server.announcements.delete_feed"},

    {0x8c, "machine.update.status"},

    {0x90, "machine.update.full"},
    {0x91, "machine.update.status"},
    {0x92, "machine.update.moonraker"},
    {0x93, "machine.update.klipper"},
    {0x94, "machine.update.client"},
    {0x95, "machine.update.system"},
    {0x96, "machine.update.recover"},

    {0xa1, "machine.device_power.devices"},
    {0xa2, "machine.device_power.get_device"},
    {0xa3, "machine.device_power.post_device"},
    {0xa4, "machine.device_power.status"},
    {0xa5, "machine.device_power.on"},
    {0xa6, "machine.device_power.off"},

    {0xb1, "machine.wled.strips"},
    {0xb2, "machine.wled.status"},
    {0xb3, "machine.wled.on"},
    {0xb4, "machine.wled.off"},
    {0xb5, "machine.wled.toggle"},
    {0xb6, "machine.wled.get_strip"},

    // OctoPrint API 模拟 暂不添加 0xc 字段保留

    {0xd1, "server.history.list"},
    {0xd2, "server.history.totals"},
    {0xd3, "server.history.reset_totals"},
    {0xd4, "server.history.get_job"},
    {0xd5, "server.history.delete_job"},

    {0xe1, "server.mqtt.publish"},
    {0xe2, "server.mqtt.subscribe"},

    {0x101, "server.webcams.list"},
    {0x102, "server.webcams.get_item"},
    {0x103, "server.webcams.post_item"},
    {0x104, "server.webcams.delete_item"},
    {0x105, "server.webcams.test"},

    {0xf1, "server.extensions.list"},
    {0xf2, "server.extensions.request"},
    {0xf3, "connection.send_event"}

};

static std::map<int, int> method2id = {
    {0x01, 4656},
    {0x02, 4656},
    {0x03, 5445},
    {0x04, 4564},
    {0x05, 4894},
    {0x06, 8463},

    {0x11, 1454},
    {0x12, 4654},
    {0x13, 5434},
    {0x14, 3456},
    {0x15, 9546},
    {0x16, 5616},
    {0x17, 2313},
    {0x18, 7643},
    {0x19, 4656},
    
    {0x21, 7466},
    {0x22, 4645},

    {0x31, 4654},
    {0x32, 4564},
    {0x33, 1465},
    {0x34, 2578},

    {0x41, 4665},
    {0x42, 4665},
    {0x43, 4665},
    {0x44, 4656},
    {0x45, 4645},
    {0x46, 4645},
    {0x47, 7896},

    {0x51, 4644},
    {0x52, 3545},
    {0x53, 5644},
    {0x54, 6548},
    {0x55, 6545},
    {0x56, 5664},
    {0x57, 5623},
    // 0x58
    // 0x59
    {0x5a, 1323},
    // 0x5b
    // 0x5c

    // 0x61     登录用户
    // 0x62     注销当前用户
    // 0x63     获取当前用户
    // 0x64     创建用户
    // 0x65     删除用户
    // 0x66     列出可用用户
    // 0x67     重置用户密码
    // 0x68     刷新 JSON Web 令牌
    // 0x69     生成 Oneshot 令牌
    // 0x6a     检索有关授权端点的信息
    // 0x6b     获取当前 API 密钥
    // 0x6c     生成新的 API 密钥

    {0x71, 8694},
    {0x72, 5644},
    {0x73, 4654},
    {0x74, 4654},

    {0x81, 4654},
    {0x82, 4654},
    {0x83, 4654},
    {0x84, 4654},
    {0x85, 4654},

    {0x86, 4654},
    {0x87, 4654},
    {0x88, 4654},
    {0x89, 4654},
    {0x8a, 4654},
    {0x8b, 4654},

    {0x8c, 4644},

    {0x90, 4645},
    {0x91, 4644},
    {0x92, 4645},
    {0x93, 5745},
    {0x94, 8546},
    {0x95, 4564},
    {0x96, 4564},

    {0xa1, 5646},
    {0xa2, 4564},
    {0xa3, 4564},
    {0xa4, 4564},
    {0xa5, 4564},
    {0xa6, 4564},

    {0xb1, 7123},
    {0xb2, 7124},
    {0xb3, 7125},
    {0xb4, 7126},
    {0xb5, 7127},
    {0xb6, 7128},

    // OctoPrint API 模拟 暂不添加 0xc 字段保留

    {0xd1, 5656},
    {0xd2, 5656},
    {0xd3, 5534},
    {0xd4, 4564},
    {0xd5, 5534},

    {0xe1, 4564},
    {0xe2, 4564},

    {0xf1, 4564},
    {0xf2, 4564},
    // 0xf2 without id

    {0x101, 4654},
    {0x102, 4654},
    {0x103, 4654},
    {0x104, 4654},
    {0x105, 4654}

};

#endif
