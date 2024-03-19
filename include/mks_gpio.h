#ifndef MKS_GPIO_H
#define MKS_GPIO_H

static int gpio_config(const char *attr, const char *val);
int set_GPIO1_C5_low();
int set_GPIO1_C5_high();
int init_GPIO1_B2();
void *monitor_GPIO1_B2(void *arg);
int init_GPIO1_C3();
void *monitor_GPIO1_C3(void *arg);
int set_GPIO1_B3_low();

#endif
