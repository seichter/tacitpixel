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

#ifndef TPLIGHT_H
#define TPLIGHT_H

#include <tp/node.h>
#include <tp/array.h>
#include <tp/vec.h>


class TP_API tpLight : public tpNode {
public:

	TP_TYPE_DECLARE

	tpLight();

	tpLight(const tpLight& rhs);

	tpLight& operator = (const tpLight& rhs);

	tpUInt getID() const { return mId; }

	void setAmbientColor(const tpVec4f& color) { mAmbientColor = color; }
	void setDiffuseColor(const tpVec4f& color) { mDiffuseColor = color; }
	void setSpecularColor(const tpVec4f& color) { mSpecularColor = color; }
	void setPosition(const tpVec4f& pos) { mPositionH = pos; }


	const tpVec4f& getAmbientColor() const { return mAmbientColor; }
	const tpVec4f& getDiffuseColor() const { return mDiffuseColor; }
	const tpVec4f& getSpecularColor() const { return mSpecularColor; }
	const tpVec4f& getPosition() const { return mPositionH; }

	//! checks if we have a spotlight
	bool isSpot() const { return (mPositionH[3] > 0); }

	//! checks if the coordinates are correct
	bool isValid() const;

protected:

	static tpUInt getNewId();

	virtual ~tpLight();

	/* colors */
	tpVec4f mAmbientColor;
	tpVec4f mDiffuseColor;
	tpVec4f mSpecularColor;

	/* light model */
	tpFloat mExponent;
	tpFloat mCutOff;
	tpFloat mConstantAttenuation;
	tpFloat mLinearAttenuation;
	tpFloat mQuadraticAttenuation;

	/* position in homogenous coordinates */
	tpVec4f mPositionH;

	/* ID management */
	tpUInt mId;
	static tpArray<tpUInt> msLightID;

};

#endif
