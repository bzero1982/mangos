#ifndef MC_CONFIG_H
#define MC_CONFIG_H

#include "../../framework/Platform/CompilerDefs.h"

// Format is YYYYMMDDRR where RR is the change in the conf file
// for that day.
#define MANGCHAT_CONF_VERSION    2008011901

#if PLATFORM == PLATFORM_WINDOWS
  #define _MANGCHAT_CONFIG  "mangchat.conf"
#else
  #define _MANGCHAT_CONFIG  "/home/mangos/code/compiled/etc/mangchat.conf"
#endif

#endif
