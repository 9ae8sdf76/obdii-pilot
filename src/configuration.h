

#ifndef BASIC_ICS_CONFIGURATION_H
#define BASIC_ICS_CONFIGURATION_H

#include <stdlib.h>

struct configuration;

typedef struct configuration configuration;

struct configuration {
    size_t time_format24;
    size_t time_meridiem;
    size_t time_flash;
    size_t time_screensaver;
};

configuration * p_config;

#endif //BASIC_ICS_CONFIGURATION_H
