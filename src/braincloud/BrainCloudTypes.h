//
//  BrainCloudTypes.h
//  BrainCloudLib
//

#ifndef _BRAINCLOUDTYPES_H_
#define _BRAINCLOUDTYPES_H_

#if (defined(WIN32) && _MSC_VER > 1600) || defined(__linux__)
	#define __HAS_STDINT
#elif defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP || WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
	#define __HAS_CSTDINT
#endif

#if defined(__HAS_STDINT)
	#include <stdint.h>
#elif defined(__HAS_CSTDINT)
	#include <cstdint>
	#include <cstdlib>
#else
	typedef signed char			int8_t;
	typedef signed short		int16_t;
	typedef signed int			int32_t;
	typedef signed long long	int64_t;

	typedef unsigned char		uint8_t;
	typedef unsigned short		uint16_t;
	typedef unsigned int		uint32_t;
	typedef unsigned long long 	uint64_t;
#endif

#ifdef __GNUC__
#define DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED __declspec(deprecated)
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED
#endif

#endif  // _BRAINCLOUDTYPES_H_
