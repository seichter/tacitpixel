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

#ifndef TP_SYSTEM_H
#define TP_SYSTEM_H

#include <tp/globals.h>
#include <tp/referenced.h>
#include <tp/array.h>
#include <tp/string.h>

class TP_API tpSystem : public tpReferenced {

	tpSystem();
	tpSystem(const tpSystem& o) {}

public:

    enum {
        kSysLittleEndian = 0x0,
        kSysBigEndian
    };

	static tpSystem* get();

	tpString getResourcePath();

	tpString getPluginPath();

	tpString getExecutablePath(bool removeExecName = false);

	tpString getCWD() const;

	tpUByte getEndian() const;

	void getDisplaySize(tpInt* w, tpInt* h);

	tpString findFile(const tpString& filename);

	tpString getTime() const;

	tpString getEnv(const tpString &name) const;

};

TP_API tpString tpSlash();


#endif
