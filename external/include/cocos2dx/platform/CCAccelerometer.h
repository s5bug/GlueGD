#include "./CCPlatformConfig.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #include "./ios/CCAccelerometer.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include "./android/CCAccelerometer.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#if (CC_TARGET_PLATFORM == CC_PLATFORM_BLACKBERRY)
    #include "./blackberry/CCAccelerometer.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_BLACKBERRY

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	#include "./win32/CCAccelerometer.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_WIN32

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	#include "./winrt/CCAccelerometer.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_WINRT

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	#include "./winrt/CCAccelerometer.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_WP8

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	#include "./mac/CCAccelerometer.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_MAC

#if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
	#include "./linux/CCAccelerometer.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_LINUX

// MARMALADE CHANGE
// Added for Marmalade support
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MARMALADE)
	#include "./Marmalade/CCAccelerometer.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_LINUX

#if (CC_TARGET_PLATFORM == CC_PLATFORM_NACL)
    #include "./nacl/CCAccelerometer.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#if (CC_TARGET_PLATFORM == CC_PLATFORM_EMSCRIPTEN)
    #include "./emscripten/CCAccelerometer.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_EMSCRIPTEN

#if (CC_TARGET_PLATFORM == CC_PLATFORM_TIZEN)
    #include "./tizen/CCAccelerometer.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_TIZEN
