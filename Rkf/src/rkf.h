#pragma once

// For myself
#ifdef RKF_BUILD_DLL
	#define RKF_API __declspec(dllexport)
#else
	#define RKF_API __declspec(dllimport)
#endif

// For user
//#define RKF_API __declspec(dllimport)

namespace rkf {
	RKF_API void func();
}
