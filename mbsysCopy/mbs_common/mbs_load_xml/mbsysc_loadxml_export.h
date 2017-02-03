
#ifndef MBSYSC_LOADXML_EXPORT_H
#define MBSYSC_LOADXML_EXPORT_H

#ifdef _WIN32
	#ifdef MBSYSC_LOADXML_STATIC_DEFINE
	#  define MBSYSC_LOADXML_EXPORT
	#  define MBSYSC_LOADXML_NO_EXPORT
	#else
	#  ifndef MBSYSC_LOADXML_EXPORT
	#    ifdef MBsysC_loadXML_EXPORTS
		    /* We are building this library */
	#      define MBSYSC_LOADXML_EXPORT __declspec(dllexport)
	#    else
	       /* We are using this library */
	#      define MBSYSC_LOADXML_EXPORT __declspec(dllimport)
	#    endif
	#  endif

	#  ifndef MBSYSC_LOADXML_NO_EXPORT
	#    define MBSYSC_LOADXML_NO_EXPORT 
	#  endif
	#endif

	#ifndef MBSYSC_LOADXML_DEPRECATED
	#  define MBSYSC_LOADXML_DEPRECATED __declspec(deprecated)
	#  define MBSYSC_LOADXML_DEPRECATED_EXPORT MBSYSC_LOADXML_EXPORT __declspec(deprecated)
	#  define MBSYSC_LOADXML_DEPRECATED_NO_EXPORT MBSYSC_LOADXML_NO_EXPORT __declspec(deprecated)
	#endif	
#else  
	#  define MBSYSC_LOADXML_EXPORT
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define MBSYSC_LOADXML_NO_DEPRECATED
#endif

#endif
