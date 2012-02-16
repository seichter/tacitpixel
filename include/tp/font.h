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

#ifndef TPFONT_H
#define TPFONT_H

#include <tp/referenced.h>
#include <tp/refptr.h>
#include <tp/scopeptr.h>
#include <tp/image.h>
#include <tp/file.h>
#include <tp/primitive.h>
#include <tp/string.h>

class tpFontRasterizer {
public:

    virtual bool onLoad(const tpString& font) = 0;

    virtual void onText(const tpString& text,tpPrimitive& prim) = 0;

    virtual tpImage* getImage() = 0;

    virtual ~tpFontRasterizer() {}
};


class TP_API tpFont : public tpReferenced {
protected:

    tpScopePtr<tpFontRasterizer> mRasterizer;

public:

	tpFont();

	tpImage* getImage();

	bool load(const tpString &name);

	void text(const tpString &text, tpPrimitive &p);

};

class TP_API tpText : public tpPrimitive {
public:

    tpText();

    void set(const tpString &s);

    void setFont(const tpString& name);

protected:

    tpRefPtr<tpFont> mFont;
    tpString mText;

};

#endif
