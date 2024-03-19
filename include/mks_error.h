#ifndef MKS_ERROR_H
#define MKS_ERROR_H

#include "nlohmann/json.hpp"
#include "./mks_log.h"

void parse_error(nlohmann::json error);

#endif
