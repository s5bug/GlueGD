#include "./CCPlatformConfig.h"

#ifndef __SELECT_CCGL_H__
#define __SELECT_CCGL_H__

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "./win32/CCGL.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
#include "./mac/CCGL.h"
#endif

#endif
