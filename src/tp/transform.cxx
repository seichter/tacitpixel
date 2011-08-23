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

#include <tp/transform.h>


tpTransform::tpTransform(const tpString& name) : tpNode(name), m_scope(kRelative)
{
	m_fields.add(new tpRefField< tpMat44r >(m_matrix,"matrix"));
	m_fields.add(new tpRefField< tpUByte >(m_scope,"scope"));
}

tpTransform::tpTransform(const tpTransform& transform) : tpNode(transform), m_scope(kRelative)
{
	m_matrix = transform.m_matrix;
	m_fields.add(new tpRefField< tpMat44r > (m_matrix,"matrix"));
	m_fields.add(new tpRefField< tpUByte >(m_scope,"scope"));
}



#if 0

tpObject* tpTransform::clone()
{
	return new tpTransform(*this);
}


const tpVec3r& tpTransform::getTranslation()
{
	return m_matrix.getTranslation();
}

const tpVec3r& tpTransform::getScale()
{
	return m_scale.getValue();
}

const tpVec4r& tpTransform::getRotation()
{
	return m_rotation.getValue();
}

void tpTransform::setTranslation(tpReal x, tpReal y, tpReal z)
{
	m_matrix.setTranslation(x,y,z);
	m_translation.setValue(tpVec3r(x,y,z));
}

void tpTransform::setTranslation(const tpVec3r& vec)
{
	this->setTranslation(vec.vec[0],vec.vec[1],vec.vec[2]);
};

void tpTransform::setScale(const tpVec3r& vec)
{
	m_matrix.setScale(vec);
	m_scale.setValue(vec);
};

void tpTransform::setRotation(const tpVec4r& vec)
{
	m_matrix.rotate(tpVec3r(vec.vec[1],vec.vec[2],vec.vec[3]),vec.vec[0]);
	m_rotation.setValue(vec);
};


void tpTransform::scale(const tpVec3<tpReal>& vec)
{
	m_matrix.scale(vec);
};

void tpTransform::translate(tpReal v1,tpReal v2,tpReal v3)
{
	m_matrix.translate(v1,v2,v3);
};


void tpTransform::translate(const tpVec3r& vec)
{
	m_matrix.translate(vec);
};


void tpTransform::rotate(const tpReal& x,const tpReal& y, const tpReal& z,
						 const tpReal& degree)
{
	m_matrix.rotate(x,y,z,degree);
}




tpMatrix<tpReal> tpTransform::getMatrix() const
{
	return m_matrix;
};

void tpTransform::dump() 
{

	//tpNode::dump();

	//tpLogMessage("%d Children",m_children.getSize());	

	//for (tpULong i = 0; i < m_children.getSize(); ++i)
	//	if (m_children[i]) m_children[i]->dump();
	
	
	
}


void tpTransform::setTransform(const tpMatrix<tpReal>& mat)
{
	m_matrix = mat;
}

#endif

TP_TYPE_REGISTER(tpTransform,tpNode,Transform);
