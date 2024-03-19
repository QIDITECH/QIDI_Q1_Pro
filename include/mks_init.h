#ifndef MKS_INIT_H
#define MKS_INIT_H

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <errno.h>

#include <iostream>
#include <fstream>

bool get_by_id();
std::string generate_by_id();
bool FileStringReplace(std::ifstream &instream, std::ofstream &outstream);
std::string get_cfg_by_id();

#endif
