// The pre-processed contents are eval'd by qmake.
#if defined( __x86_64__ )
QMAKE_TARGET.arch = x64
#elif defined( _M_X64 )
QMAKE_TARGET.arch = x86_64
#elif defined( __i386 ) || defined( _M_IX86 )
QMAKE_TARGET.arch = x86
#endif
#ifdef _MSC_VER
QMAKE_MSC_VER = _MSC_VER
#endif
#ifdef __GNUC__
QMAKE_GCC_MAJOR_VERSION = __GNUC__
QMAKE_GCC_MINOR_VERSION = __GNUC_MINOR__
QMAKE_GCC_PATCH_VERSION = __GNUC_PATCHLEVEL__
#endif
