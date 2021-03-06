/*
 * Copyright (C) 1999-2013 Hartmut Seichter
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
#include "tp/config.h"

#if defined(_WIN32) || defined(_WIN64) || defined(_WIN32_WCE)
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <tchar.h>
	#include <direct.h>
#elif defined(__APPLE__)
	#include <CoreFoundation/CoreFoundation.h>
	#include <unistd.h>
	#if defined(__MACH__)
		#include <mach/mach.h>
	#endif
#elif defined(__unix) || defined(__HAIKU__)
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


tpUByte
tpSystem::getEndian() const
{
	tpInt32 num = 1;
	if(*(char *)&num == 1) return kSysLittleEndian;
	return kSysBigEndian;
}




tpString
tpSystem::getCWD() const
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
tpSystem::getEnv(const tpString& name) const
{
	tpString ret;
	ret = getenv(name.c_str());
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

tpString
tpSystem::findFile( const tpString& filename )
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

tpULong
tpSystem::getPID(bool parent /*=false*/) const {
#if defined(__unix)
	return (parent) ? getppid() : getpid();
#endif
	return 0L;
}



tpStringArray
tpSystem::getFontPaths()
{
	tpStringArray res;

#if defined(_WIN32)
	res.add(this->getEnv("SYSTEMROOT") + tpString("/Fonts"));
#endif

	return res;
}

tpFloat
tpSystem::getIdleTime() const
{
	float tot_cpu = 0;


	// shamelessly plugged from StackOverflow

#if defined(__MACH__)

	kern_return_t kr;
	task_info_data_t tinfo;
	mach_msg_type_number_t task_info_count;

	task_info_count = TASK_INFO_MAX;
	kr = task_info(mach_task_self(), TASK_BASIC_INFO, (task_info_t)tinfo, &task_info_count);
	if (kr != KERN_SUCCESS) {
		return -1;
	}

	task_basic_info_t      basic_info;
	thread_array_t         thread_list;
	mach_msg_type_number_t thread_count;

	thread_info_data_t     thinfo;
	mach_msg_type_number_t thread_info_count;

	thread_basic_info_t basic_info_th;
	uint32_t stat_thread = 0; // Mach threads

	basic_info = (task_basic_info_t)tinfo;

	// get threads in the task
	kr = task_threads(mach_task_self(), &thread_list, &thread_count);
	if (kr != KERN_SUCCESS) {
		return -1;
	}
	if (thread_count > 0)
		stat_thread += thread_count;

	long tot_sec = 0;
	long tot_usec = 0;
	int j;

	for (j = 0; j < thread_count; j++)
	{
		thread_info_count = THREAD_INFO_MAX;
		kr = thread_info(thread_list[j], THREAD_BASIC_INFO,
						 (thread_info_t)thinfo, &thread_info_count);
		if (kr != KERN_SUCCESS) {
			return -1;
		}

		basic_info_th = (thread_basic_info_t)thinfo;

		if (!(basic_info_th->flags & TH_FLAGS_IDLE)) {
			tot_sec = tot_sec + basic_info_th->user_time.seconds + basic_info_th->system_time.seconds;
			tot_usec = tot_usec + basic_info_th->system_time.microseconds + basic_info_th->system_time.microseconds;
			tot_cpu = tot_cpu + basic_info_th->cpu_usage / (float)TH_USAGE_SCALE;
		}

	} // for each thread

	tot_cpu = 1.f - tot_cpu;

#endif

	return tot_cpu;

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
