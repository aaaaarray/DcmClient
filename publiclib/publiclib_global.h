#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(PUBLICLIB_LIB)
#  define PUBLICLIB_EXPORT Q_DECL_EXPORT
# else
#  define PUBLICLIB_EXPORT Q_DECL_IMPORT
# endif
#else
# define PUBLICLIB_EXPORT
#endif
