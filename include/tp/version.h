#ifndef TP_VERSION_H
#define TP_VERSION_H

#include <tp/globals.h>

TP_API int tpGetMajorVersion();
TP_API int tpGetMinorVersion();
TP_API int tpGetBuildVersion();

TP_API const char* tpGetVersionString();

#endif
