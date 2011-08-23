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

#ifndef TPTRANSFORM_H
#define TPTRANSFORM_H


#include <tp/node.h>
#include <tp/vec.h>
#include <tp/mat.h>


class TP_API tpTransform : public tpNode {
public:

	TP_TYPE_DECLARE;

	enum {
		kRelative = 0x0,
		kAbsolute
	};

	tpTransform(const tpString& name = "transform");
	tpTransform(const tpTransform& transform);

	const tpMat44r& getMatrix() const { return m_matrix; }
	tpMat44r& getMatrix() { return m_matrix; }

	tpUByte getScope() const { return m_scope; }
	void setScope(tpUByte val) { m_scope = val; }


	/*
	const tpVec3r& getTranslation();
	const tpVec3r& getScale();
	const tpVec4r& getRotation();
	
	void setTranslation(tpReal x, tpReal y, tpReal z);


	void setScale(const tpVec3r&);
	void setRotation(const tpVec4r&);
	void rotate(const tpReal& x, 
		const tpReal& y,
		const tpReal& z,
		const tpReal& degree);


	void translate(tpReal x, tpReal y, tpReal z);
	void translate(const tpVec3r&);
	void scale(const tpVec3r&);

	virtual tpObject* clone();

	*/

protected:
	tpUByte m_scope;
	tpMat44r m_matrix;

};


#endif
