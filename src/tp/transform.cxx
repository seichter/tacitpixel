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

#include <tp/transform.h>


tpTransform::tpTransform(const tpString& name) : tpNode(name)
{
	m_fields.add(new tpRefField< tpMat44r > (m_matrix,"matrix"));
}

tpTransform::tpTransform(const tpTransform& transform) : tpNode(transform)
{
	m_matrix = transform.m_matrix;
	m_fields.add(new tpRefField< tpMat44r > (m_matrix,"matrix"));
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
