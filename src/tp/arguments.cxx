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

#include "tp/arguments.h"
#include "tp/stringtokenizer.h"

tpArguments::tpArguments(int *argc, char* argv[])
{
    setArguments(argc,argv);
}

bool
tpArguments::get(const tpString& param,tpString& value) const
{
	for (tpStringArray::const_iterator i = mArguments.begin();
		 i != mArguments.end();
		 ++i)
	{
		if (param == (*i))
		{
			if (i != &mArguments.back()) { i++; value = (*i); }
			return true;
		}
	}
    return false;
}

bool 
tpArguments::get(const tpString &param, tpUInt &value) const
{
    tpString num;
    bool ok = this->get(param,num);

    value = atoi(num.c_str());
    return ok;
}

bool
tpArguments::get(const tpString& param) const
{
	for (tpStringArray::const_iterator i = mArguments.begin();
		i != mArguments.end();
		++i)
	{
		if (param == (*i))
		{
			return true;
		}
	}
	return false;
}


//////////////////////////////////////////////////////////////////////////

void tpArguments::setArguments(const tpString &args)
{
    mArguments = tpStringTokenizer::split(args," ");
}

void tpArguments::setArguments(int *argc, char *argv[])
{
    if (argc)
    {
        for (tpUInt i = 0;
             i < *argc;
             ++i)
        {
            this->mArguments.add(argv[i]);
        }
    }
}




