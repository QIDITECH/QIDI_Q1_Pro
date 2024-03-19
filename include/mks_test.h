#ifndef MKS_TEST_H
#define MKS_TEST_H

#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream> 
#include <exception> 
#include <string.h>

void *mks_test(void *arg);
bool testUSB();
bool moko_test_func();
bool network_test_func();

#endif
