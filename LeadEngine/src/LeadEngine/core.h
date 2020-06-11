#pragma once

#ifdef LE_PLATFORM_WINDOWS
	#ifdef LE_BUILD_DLL
		#define LE_API __declspec(dllexport)
	#else
		#define LE_API __declspec(dllimport)
	#endif
#else
	#error Lead Engine only supports Windows.
#endif

#ifdef LE_ENABLE_ASSERTS
	#define LE_ASSERT(x, ...) { if(!(x)) { LE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define LE_CORE_ASSERT(x, ...) { if(!(x)) { LE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define LE_ASSERT(x, ...)
	#define LE_CORE_ASSERT(x, ...)
#endif