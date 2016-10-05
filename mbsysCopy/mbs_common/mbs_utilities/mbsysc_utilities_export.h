
#ifndef MBSYSC_UTILITIES_EXPORT_H
#define MBSYSC_UTILITIES_EXPORT_H

#ifdef _WIN32
	#ifdef MBSYSC_UTILITIES_STATIC_DEFINE
	#  define MBSYSC_UTILITIES_EXPORT
	#  define MBSYSC_UTILITIES_NO_EXPORT
	#else
	#  ifndef MBSYSC_UTILITIES_EXPORT
	#    ifdef MBsysC_utilities_EXPORTS
	        /* We are building this library */
	#      define MBSYSC_UTILITIES_EXPORT __declspec(dllexport)
	#    else
	        /* We are using this library */
	#      define MBSYSC_UTILITIES_EXPORT __declspec(dllimport)
	#    endif
	#  endif
	
	#  ifndef MBSYSC_UTILITIES_NO_EXPORT
	#    define MBSYSC_UTILITIES_NO_EXPORT 
	#  endif
	#endif
	
	#ifndef MBSYSC_UTILITIES_DEPRECATED
	#  define MBSYSC_UTILITIES_DEPRECATED __declspec(deprecated)
	#  define MBSYSC_UTILITIES_DEPRECATED_EXPORT MBSYSC_UTILITIES_EXPORT __declspec(deprecated)
	#  define MBSYSC_UTILITIES_DEPRECATED_NO_EXPORT MBSYSC_UTILITIES_NO_EXPORT __declspec(deprecated)
	#endif
#else  
#  define MBSYSC_UTILITIES_EXPORT
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define MBSYSC_UTILITIES_NO_DEPRECATED
#endif

#endif
