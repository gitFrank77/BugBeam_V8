#ifndef __UTILS_CONFIG_H
#define __UTILS_CONFIG_H

#define UTILS_V1    1
#define UTILS_V2    0

#include <SPI.h>
#include <Ethernet.h>
#include <Modbus.h>
#include <ModbusIP.h>

#if UTILS_V1
#include "utils_v1.h"
#elif UTILS_V2  
#include "utils_v2.h"
#else
#error "No source selected"
#endif



#endif /* utils_config.h */
