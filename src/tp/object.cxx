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


#include <tp/object.h>
#include <tp/log.h>

tpObject::tpObject(const tpString& name)
	: tpReferenced()
    , tpEventHandler()
    , mName(name)
{
    mFields.add(new tpRefField<tpString>(mName,"name"));
}

tpObject::tpObject(const tpObject& obj)
	: tpReferenced()
{
    mFields.add(new tpRefField<tpString>(mName,"name"));
    mName = obj.mName;
}

tpObject::~tpObject()
{
}

tpObject* 
tpObject::clone(const tpUByte& clonemode) const
{
    tpObject* ret = new tpObject(mName + "_copy");
	return ret;
}

bool 
tpObject::isOfType(const tpRTTI* type) const
{
	return getType()->isOfType(type);
}

bool 
tpObject::isExactType(const tpRTTI* type) const
{
	return getType() == type;
}

TP_TYPE_REGISTER(tpObject,tpReferenced,Object);
