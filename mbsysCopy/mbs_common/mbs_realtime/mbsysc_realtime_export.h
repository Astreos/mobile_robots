
#ifndef MBSYSC_REALTIME_EXPORT_H
#define MBSYSC_REALTIME_EXPORT_H

#ifdef _WIN32

	#ifdef MBSYSC_REALTIME_STATIC_DEFINE
	#  define MBSYSC_REALTIME_EXPORT
	#  define MBSYSC_REALTIME_NO_EXPORT
	#else
	#  ifndef MBSYSC_REALTIME_EXPORT
	#    ifdef MBsysC_realtime_EXPORTS
	        /* We are building this library */
	#      define MBSYSC_REALTIME_EXPORT __declspec(dllexport)
	#    else
	        /* We are using this library */
	#      define MBSYSC_REALTIME_EXPORT __declspec(dllimport)
	#    endif
	#  endif
	
	#  ifndef MBSYSC_REALTIME_NO_EXPORT
	#    define MBSYSC_REALTIME_NO_EXPORT 
	#  endif
	#endif
	
	#ifndef MBSYSC_REALTIME_DEPRECATED
	#  define MBSYSC_REALTIME_DEPRECATED __declspec(deprecated)
	#  define MBSYSC_REALTIME_DEPRECATED_EXPORT MBSYSC_REALTIME_EXPORT __declspec(deprecated)
	#  define MBSYSC_REALTIME_DEPRECATED_NO_EXPORT MBSYSC_REALTIME_NO_EXPORT __declspec(deprecated)
	#endif
	
	#define DEFINE_NO_DEPRECATED 0
	#if DEFINE_NO_DEPRECATED
	# define MBSYSC_REALTIME_NO_DEPRECATED
	#endif

#else  // Unix
#  define MBSYSC_REALTIME_EXPORT
#endif

#endif
