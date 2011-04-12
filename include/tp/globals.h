#ifndef _TACITPIXEL_GLOBALS_H_
#define _TACITPIXEL_GLOBALS_H_


#if defined(_MSC_VER)
	#if defined(TP_BUILD_LIBRARY)
		#define TP_API __declspec(dllexport)
	#else
		#define TP_API __declspec(dllimport)
	#endif
#else // non MSC
	#define TP_API
#endif


#endif
