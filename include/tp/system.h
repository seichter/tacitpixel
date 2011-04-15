#ifndef TP_SYSTEM_H
#define TP_SYSTEM_H


#include <tp/globals.h>
#include <tp/referenced.h>
#include <tp/array.h>
#include <tp/string.h>

#if defined(_WIN32) || defined(_WIN32_WCE)
const tpString tpPathSep("\\");
#else
const tpString tpPathSep("/");
#endif

enum {
	TP_SYS_LITTLEENDIAN = 0x0,
	TP_SYS_BIGENDIAN
};

class TP_API tpSystem : public tpReferenced {

	tpSystem();

public:
	
	static tpSystem* get();
	
	tpString getResourcePath();

	tpString getPluginPath();
	
	tpString getExecutablePath(bool removeExecName = false);
	
	tpString getCWD() const;

	tpUByte getEndian() const;

	void getDisplaySize(tpInt* w, tpInt* h);

	tpString findFile(const tpString& filename);
	
	tpString getTime() const;

};

#endif