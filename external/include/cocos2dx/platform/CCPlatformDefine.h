#include "./CCPlatformConfig.h"

#ifndef __SELECT_CCPLATFORMDEFINE_H__
#define __SELECT_CCPLATFORMDEFINE_H__

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "./win32/CCPlatformDefine.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
#include "./mac/CCPlatformDefine.h"
#endif

#endif
