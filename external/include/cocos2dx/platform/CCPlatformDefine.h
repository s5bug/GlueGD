#include "./CCPlatformConfig.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #include "./ios/CCPlatformDefine.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include "./android/CCPlatformDefine.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#if (CC_TARGET_PLATFORM == CC_PLATFORM_BLACKBERRY)
    #include "./blackberry/CCPlatformDefine.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_BLACKBERRY

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	#include "./win32/CCPlatformDefine.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_WIN32

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	#include "./winrt/CCPlatformDefine.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_WINRT

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	#include "./winrt/CCPlatformDefine.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_WP8

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	#include "./mac/CCPlatformDefine.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_MAC

#if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
	#include "./linux/CCPlatformDefine.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_LINUX

// MARMALADE CHANGE
// Added for Marmalade support
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MARMALADE)
	#include "./Marmalade/CCPlatformDefine.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_LINUX

#if (CC_TARGET_PLATFORM == CC_PLATFORM_NACL)
    #include "./nacl/CCPlatformDefine.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#if (CC_TARGET_PLATFORM == CC_PLATFORM_EMSCRIPTEN)
    #include "./emscripten/CCPlatformDefine.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_EMSCRIPTEN

#if (CC_TARGET_PLATFORM == CC_PLATFORM_TIZEN)
    #include "./tizen/CCPlatformDefine.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_TIZEN
