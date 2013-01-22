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

#include <tp/stringtokenizer.h>


tpStringTokenizer::tpStringTokenizer()
{
}


tpStringTokenizer::tpStringTokenizer(const tpString& str, const tpString& delim)
	: mCopy(str)
	, mDelim(delim)
{
}

tpString
tpStringTokenizer::next()
{
	tpString res;
	tpInt pos = mCopy.find(*mDelim.c_str(),false);

	if (pos != tpString::kNotFound)
	{
		res = mCopy.substr(0,pos);
		mCopy = mCopy.substr( pos + mDelim.getLength(), mCopy.getLength() - pos - mDelim.getLength() );
	} else {
		res = mCopy;
		mCopy.empty();
	}

	return res;
}

bool
tpStringTokenizer::finished() const
{
	return (0 == mCopy.getLength());
}


/*static */
tpStringArray
tpStringTokenizer::split(const tpString& str, const tpString& delim)
{
    tpStringTokenizer tkz(str,delim);
    tpStringArray res;
    while (!tkz.finished())
    {
        res.add(tkz.next());
    }
    return res;
}
