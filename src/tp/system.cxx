/* 
 * Copyright (C) 1999-2011 Hartmut Seichter
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "tp/system.h"

#include "tp/log.h"
#include "tp/refptr.h"

#if defined(_WIN32) || defined(_WIN64) || defined(_WIN32_WCE)
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <tchar.h>
	#include <direct.h>
#elif defined(__APPLE__)
	#include <CoreFoundation/CoreFoundation.h>
	#include <unistd.h>
#elif defined(__unix)
	#include <unistd.h>
#else
	// Unknown platform
#endif

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#if defined(HAVE_TIME_H)
#include <time.h>
#endif


tpUByte tpSystem::getEndian() const
{
	int i = 1;
	char *p = (char *)&i;

	if (p[0] == 1) return TP_SYS_LITTLEENDIAN;

	return TP_SYS_LITTLEENDIAN;
}

tpString tpSystem::getCWD() const 
{
#if defined(_WIN32)
	char* wd = _getcwd(0,0);
#else
	char* wd = getcwd(0, 0);
#endif
	
	tpString ret(wd);
	
	free(wd);
	
	return ret;
}

tpString
tpSystem::getResourcePath()
{
	tpString result;
	const unsigned int MAXPATHLEN = 2048;
	static char path[MAXPATHLEN];
	memset(&path[0],0,MAXPATHLEN);
	
	
#if defined(__APPLE__)
	CFURLRef bundleURL;
	CFStringRef pathStr;
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	
	bundleURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
	pathStr = CFURLCopyFileSystemPath(bundleURL, kCFURLPOSIXPathStyle);
	CFStringGetCString(pathStr, path, MAXPATHLEN, kCFStringEncodingASCII);
	
	result.set(&path[0]);
	
	CFRelease(pathStr);
	CFRelease(bundleURL);
	
#elif defined(_WIN32) || defined(_WIN64) || defined(_WIN32_WCE)
	
	result = getExecutablePath(true);
	
#endif
	
	return result;
}


tpString
tpSystem::getPluginPath()
{
	tpString result;
	const unsigned int MAXPATHLEN = 2048;
	static char path[MAXPATHLEN];
	memset(&path[0],0,MAXPATHLEN);
	
	
#if defined(__APPLE__)

	result = getExecutablePath(true);
	
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	CFURLRef bundleURL = CFBundleCopyBuiltInPlugInsURL(mainBundle);
	CFStringRef pathStr = CFURLCopyFileSystemPath(bundleURL, kCFURLPOSIXPathStyle);
	
	CFStringGetCString(pathStr, path, MAXPATHLEN, kCFStringEncodingASCII);
	
	printf("--- %s --- '%s'\n",path,result.c_str());
	
	result.set(&path[0]);
	
	CFRelease(pathStr);
	CFRelease(bundleURL);
	
	
	
#elif defined(_WIN32) || defined(_WIN64) || defined(_WIN32_WCE)
	
	result = getExecutablePath(true);

#else

	result = getExecutablePath(true) + tpString("/../") + tpString("lib");
	
#endif
	
	return result;
}

tpString tpSystem::getExecutablePath(bool removeExecName)
{
	tpString result;

	const unsigned int MAXPATHLEN = 2048;
	static char path[MAXPATHLEN];
	memset(&path[0],0,MAXPATHLEN);

#if defined(__APPLE__)
	CFURLRef bundleURL;
	CFStringRef pathStr;
	CFBundleRef mainBundle = CFBundleGetMainBundle();

	bundleURL = CFBundleCopyExecutableURL(mainBundle);
	pathStr = CFURLCopyFileSystemPath(bundleURL, kCFURLPOSIXPathStyle);
 	CFStringGetCString(pathStr, path, MAXPATHLEN, kCFStringEncodingASCII);

	result.set(&path[0]);
	
	CFRelease(pathStr);
	CFRelease(bundleURL);
	
#elif defined(_WIN32) || defined(_WIN64) || defined(_WIN32_WCE)
	static TCHAR lpFname[MAXPATHLEN];
	DWORD ret = GetModuleFileName( NULL, &lpFname[0], MAXPATHLEN );

	result.set(&lpFname[0]);

	if (removeExecName)
	{
		int offset = result.find('\\',true);
		result.truncate(offset);
	}

#else
	
    readlink("/proc/self/exe", path, sizeof(path));
	result.set(path);

	if (removeExecName)
	{
		int offset = result.find('/',true);
		result.c_str()[offset] = '\0';
	}

#endif

	return result;
}

tpSystem* tpSystem::get()
{
	static tpRefPtr<tpSystem> gs_system = new tpSystem();
	return gs_system.get();
}

void tpSystem::getDisplaySize( tpInt* w, tpInt* h )
{
#if defined(_WIN32_WCE)
	*w = GetSystemMetrics(SM_CXSCREEN);
	*h = GetSystemMetrics(SM_CYSCREEN);
#endif
}

static tpArray<tpString> gs_filesearchpaths;

tpString tpSystem::findFile( const tpString& filename )
{
	//tpLogNotify("%s - %d",__FUNCTION__,m_filesearchpaths.getSize());
	//getchar();

	tpString res;

	for (tpSizeT i = 0; i < gs_filesearchpaths.getSize(); i++)
	{
		res = gs_filesearchpaths[i];
		res.append( tpSlash() );
		res.append( filename );
		//tpLogNotify("%s - file %s",__FUNCTION__,path.c_str());
		//getchar();
		FILE* f = fopen(res.c_str(),"rb");
		if (f) {
			fclose(f); 
			break;
		} else res = "";
	}

	//tpLogNotify("%s - found '%s'",__FUNCTION__,res.c_str());

	return res;
}

tpSystem::tpSystem()
{
	gs_filesearchpaths.add(getExecutablePath(true))
		.add(getCWD())
		.add("");
}

tpString
tpSystem::getTime() const
{
	time_t rawtime;
	struct tm * timeinfo;

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	
	char buffer[80];
	
	strftime (buffer,80,"%X %x",timeinfo);

	tpString res(buffer);
	
	return res;
	
}


tpString
tpSlash()
{
#if defined(_WIN32)
	return tpString("\\");
#else
	return tpString("/");
#endif
}
