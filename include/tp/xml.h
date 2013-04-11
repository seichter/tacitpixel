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

#ifndef TPXML_H
#define TPXML_H

#include <tp/object.h>
#include <tp/string.h>
#include <tp/map.h>
#include <tp/stack.h>
#include <tp/refptr.h>


class tpXML;

struct TP_API tpXMLNodeCallback {
	virtual void operator()(const tpXML& xml) = 0;
};

/**
 * @brief The tpXML class wraps around RapidXML to provide a
 * minimal XML parsing capability. Please note that tpXML is not
 * threadsafe.
 */
class TP_API tpXML : public tpObject {
	tpString mDoc;
public:

	tpXML();

	typedef tpMap<tpString,tpString> Attributes;
	typedef tpArray<tpString> Path;

	void parse(const tpString &doc);

	const Path& getCurrentPath() const { return mPath; }
	Path& getCurrentPath() { return mPath; }

	Attributes& getCurrentAttributes() { return mAttributes; }
	const Attributes& getCurrentAttributes() const { return mAttributes; }

	tpString& getCurrentDataNode() { return mData; }
	const tpString& getCurrentDataNode() const { return mData; }

	void setCallback(tpXMLNodeCallback* cb) { mCallback = cb; }
	tpXMLNodeCallback* getCallback() { return mCallback; }

	void callback() const;

protected:

	tpXMLNodeCallback* mCallback;

	Path mPath;
	Attributes mAttributes;
	tpString mData;

	virtual ~tpXML() {}

};

#endif
