#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef _WIN32
	#include "Windows/platform_win32.h"
#elif defined(__linux__)
	#include "Linux/platform_linux.h"
#else
	#error "Unsupported platform"
#endif

#endif // PLATFORM_H
