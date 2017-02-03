
#ifndef MBSYSC_MODULE_EXPORT_H
#define MBSYSC_MODULE_EXPORT_H

#ifdef _WIN32
	#ifdef MBSYSC_MODULE_STATIC_DEFINE
	#  define MBSYSC_MODULE_EXPORT
	#  define MBSYSC_MODULE_NO_EXPORT
	#else
	#  ifndef MBSYSC_MODULE_EXPORT
	#    ifdef MBsysC_module_EXPORTS
	        /* We are building this library */
	#      define MBSYSC_MODULE_EXPORT __declspec(dllexport)
	#    else
	        /* We are using this library */
	#      define MBSYSC_MODULE_EXPORT __declspec(dllimport)
	#    endif
	#  endif
	
	#  ifndef MBSYSC_MODULE_NO_EXPORT
	#    define MBSYSC_MODULE_NO_EXPORT 
	#  endif
	#endif
	
	#ifndef MBSYSC_MODULE_DEPRECATED
	#  define MBSYSC_MODULE_DEPRECATED __declspec(deprecated)
	#  define MBSYSC_MODULE_DEPRECATED_EXPORT MBSYSC_MODULE_EXPORT __declspec(deprecated)
	#  define MBSYSC_MODULE_DEPRECATED_NO_EXPORT MBSYSC_MODULE_NO_EXPORT __declspec(deprecated)
	#endif
#else  
#  define MBSYSC_MODULE_EXPORT
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define MBSYSC_MODULE_NO_DEPRECATED
#endif

#endif
