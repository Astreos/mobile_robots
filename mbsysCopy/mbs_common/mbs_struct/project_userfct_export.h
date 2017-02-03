
#ifndef PROJECT_USERFCT_EXPORT_H
#define PROJECT_USERFCT_EXPORT_H

#ifdef _WIN32
    #ifdef PROJECT_USERFCT_STATIC_DEFINE
    #  define PROJECT_USERFCT_EXPORT
    #  define PROJECT_USERFCT_NO_EXPORT
    #else
    #  ifndef PROJECT_USERFCT_EXPORT
    #    ifdef Project_userfct_EXPORTS
            /* We are building this library */
    #      define PROJECT_USERFCT_EXPORT __declspec(dllexport)
    #    else
            /* We are using this library */
    #      define PROJECT_USERFCT_EXPORT __declspec(dllimport)
    #    endif
    #  endif
    
    #  ifndef PROJECT_USERFCT_NO_EXPORT
    #    define PROJECT_USERFCT_NO_EXPORT 
    #  endif
    #endif
    
    #ifndef PROJECT_USERFCT_DEPRECATED
    #  define PROJECT_USERFCT_DEPRECATED __declspec(deprecated)
    #  define PROJECT_USERFCT_DEPRECATED_EXPORT PROJECT_USERFCT_EXPORT __declspec(deprecated)
    #  define PROJECT_USERFCT_DEPRECATED_NO_EXPORT PROJECT_USERFCT_NO_EXPORT __declspec(deprecated)
    #endif
    
    #define DEFINE_NO_DEPRECATED 0
    #if DEFINE_NO_DEPRECATED
    # define PROJECT_USERFCT_NO_DEPRECATED
    #endif

#else  // Unix
#  define PROJECT_USERFCT_EXPORT
#endif

#endif
