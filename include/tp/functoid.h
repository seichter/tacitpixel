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

#ifndef TP_FUNCTOID_H
#define TP_FUNCTOID_H

#include <tp/globals.h>
#include <tp/map.h>
#include <tp/string.h>

class tpFunctoid;

typedef tpMap<tpFunctoid*,tpString> tpFunctoidArray;


struct TP_API tpGetProcAddress {
	virtual void* operator()(const char* name) = 0;
};


class TP_API tpFunctoid {
public:

	virtual void assign(tpGetProcAddress& gpa,const tpString& name) = 0;

	static void load(tpGetProcAddress& gpa,tpFunctoidArray& farray);
};

template <typename T>
struct tpFunctoidImpl : tpFunctoid
{
	T f;

	bool isValid() const { return (f != 0L); }

	void assign(tpGetProcAddress& gpa, const tpString& name)
	{
		f = reinterpret_cast<T>(gpa(name.c_str()));
	}
};



#endif
