#include "./CCPlatformConfig.h"

#ifndef __SELECT_CCSTDC_H__
#define __SELECT_CCSTDC_H__

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "./platform/win32/CCStdC.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
#include "./platform/mac/CCStdC.h"
#endif

#endif
