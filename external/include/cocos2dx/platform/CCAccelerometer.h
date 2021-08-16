#include "./CCPlatformConfig.h"

#ifndef __SELECT_CCACCELEROMETER_H__
#define __SELECT_CCACCELEROMETER_H__

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "./win32/CCAccelerometer.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
#include "./mac/CCAccelerometer.h"
#endif

#endif
