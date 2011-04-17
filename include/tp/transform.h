/*
 * Twisted Pair Visualization Engine
 *
 * Copyright (c) 1999-2009 Hartmut Seichter 
 * 
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the Twisted Pair License (TPL) version 1.0 or (at your option) 
 * any later version. The full license text is available in the LICENSE file 
 * included with this distribution, and on the technotecture.com website.
 *
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
