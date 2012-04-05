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

#include "tp/light.h"
#include "tp/log.h"


tpArray<tpUInt> tpLight::msLightID = tpArray<tpUInt>();

tpLight::tpLight()
	: tpNode()
	, mAmbientColor(tpVec4f(0.0f,0.0f,0.0f,1.0f))
	, mDiffuseColor(tpVec4f(0.0f,0.0f,0.0f,1.0f))
	, mSpecularColor(tpVec4f(0.0f,0.0f,0.0f,1.0f))
	, mExponent(0)
	, mCutOff(128)
	, mConstantAttenuation(1)
	, mLinearAttenuation(0)
	, mQuadraticAttenuation(0)
	, mPositionH(tpVec4f(0.f,0.f,1.f,0.f))
	, mId(tpLight::getNewId())
{
	if (mId == 0) {
		// use OpenGL defaults for ID0
		mDiffuseColor = mSpecularColor = tpVec4f(1.0f,1.0f,1.0f,0.0f);
		mSpecularColor(3) = 1.f;
	}
}

tpUInt
tpLight::getNewId()
{
	tpUInt _id = msLightID.getSize();
	while (tpArray<tpUInt>::kNotFound != msLightID.find(_id) ) {
		++_id;
	}
	msLightID.add(_id);
	return _id;
}

tpLight::tpLight(const tpLight &rhs)
	: tpNode(rhs)
{
}

tpLight &tpLight::operator =(const tpLight &rhs)
{
	if (this != &rhs)
	{
		// copy everything except the id

		//*this = tpNode::operator =(rhs);

		mAmbientColor = rhs.mAmbientColor;
		mDiffuseColor = rhs.mDiffuseColor;
		mSpecularColor = rhs.mSpecularColor;
		mPositionH = rhs.mPositionH;

		mExponent = rhs.mExponent;
		mCutOff = rhs.mCutOff;
		mConstantAttenuation = rhs.mConstantAttenuation;
		mLinearAttenuation = rhs.mLinearAttenuation;
		mQuadraticAttenuation = rhs.mQuadraticAttenuation;

	}

	return *this;
}

bool tpLight::isValid() const
{
	// a quick check for correct homogenous coordinates
	return (isSpot()) ? true : (mPositionH.getSquaredNorm() > 0);
}

tpLight::~tpLight()
{
	msLightID.erase(mId);
}

TP_TYPE_REGISTER(tpLight,tpNode,Light);

