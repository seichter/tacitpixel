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

#include "tp/primitive.h"
#include "tp/math.h"
#include "tp/log.h"
#include "tp/node.h"


//struct tpAttributeIterator {
//	tpVec4f
//};


//struct tpAttributeStorage {

//	enum {
//		kVertex,
//		kMap,
//		kColor,
//		kNormal
//	};

//	tpUInt getStride(tpUChar attribute) const { return 4; }
//	tpUInt getOffset(tpUChar attribute) const { return 0; }

//	tpAttributeIterator& addVertex();


//	tpArray<tpFloat> mStorage;
//};



struct tpPrimitiveAttribute {

};


tpPrimitive::tpPrimitive(tpUByte meshtype,tpUShort attributes)
	: tpRenderable() ,
	m_primitivetype(meshtype),
	m_attributes(attributes)
{
}

tpPrimitive::tpPrimitive(const tpPrimitive& geo)
	: tpRenderable(geo),
	m_primitivetype(geo.m_primitivetype),
	m_vertices(geo.m_vertices)
{
	m_normals = geo.m_normals;
	m_texcoords = geo.m_texcoords;
	m_colors = geo.m_colors;
}


tpPrimitive::~tpPrimitive()
{
}


tpObject* tpPrimitive::clone()
{
	return new tpPrimitive(*this);
}


//////////////////////////////////////////////////////////////////////////

tpUInt tpPrimitive::getPrimitiveType() const
{
	return this->m_primitivetype;
}

void tpPrimitive::setPrimitiveType(tpUInt meshtype)
{
	this->m_primitivetype = meshtype;
}

void
tpPrimitive::addVertex(const tpVec3<tpReal>& pos,
					   const tpVec3<tpReal>& normal,
					   const tpVec2<tpReal>& tcoord,
					   const tpVec4<tpReal>& color)
{
	m_vertices.add(pos[0]).add(pos[1]).add(pos[2]);
	m_normals.add(normal[0]).add(normal[1]).add(normal[2]);
	m_texcoords.add(tcoord[0]).add(tcoord[1]);
	m_colors.add(color[0]).add(color[1]).add(color[2]).add(color[3]);
}


void
tpPrimitive::removeVertex( tpUInt id ) {
	m_vertices.erase(id);
}

void
tpPrimitive::scale(const tpVec3r& scale)
{
	tpReal* v_ptr = m_vertices.getData();
	for (tpSizeT v_len = getVertexCount(); v_len; v_len--)
	{
		*v_ptr++ *= scale[0];
		*v_ptr++ *= scale[1];
		*v_ptr++ *= scale[2];
	}
}

void
tpPrimitive::getAABB(tpVec3r& aabb_min,tpVec3r& aabb_max)
{

	tpReal* v_ptr = m_vertices.getData();
	for (tpSizeT v_len = getVertexCount(); v_len; v_len--)
	{
		aabb_max[0] = tpMax(*v_ptr,aabb_max[0]);
		aabb_min[0] = tpMin(*v_ptr,aabb_min[0]);
		v_ptr++;

		aabb_max[1] = tpMax(*v_ptr,aabb_max[1]);
		aabb_min[1] = tpMin(*v_ptr,aabb_min[1]);
		v_ptr++;

		aabb_max[2] = tpMax(*v_ptr,aabb_max[2]);
		aabb_min[2] = tpMin(*v_ptr,aabb_min[2]);
		v_ptr++;

	}
}

const tpArray<tpReal>& tpPrimitive::getVertices() const {
	return m_vertices;
}

const tpArray<tpReal>& tpPrimitive::getNormals() const {
	return m_normals;
}

const tpArray<tpReal>& tpPrimitive::getTexCoords() const {
	return m_texcoords;
}


tpVec3r tpGetNormal(const tpArray<tpReal>& vertices)
{
	tpVec3r normal(0,1,0);

	/* minimum of 3 vertices needed */
	if (vertices.getSize() > 8) {

		tpVec3r a(vertices[0],vertices[1],vertices[2]);
		tpVec3r b(vertices[3],vertices[4],vertices[5]);
		tpVec3r c(vertices[6],vertices[7],vertices[8]);

		a -= b;
		b -= c;

		if (a == b) return normal;

		normal = a.cross(b);
		normal.normalize();
	}

	return normal;

}


void tpPrimitive::flipNormals()
{
	tpReal *n_ptr = m_normals.getData();

	for (tpULong i = getNormalsCount(); i; --i)
	{
		*n_ptr++ *= -1;
		*n_ptr++ *= -1;
		*n_ptr++ *= -1;
	}
}

void tpPrimitive::clearAll()
{
	m_vertices.clear();
	m_texcoords.clear();
	m_normals.clear();
	m_colors.clear();
}


tpUInt tpPrimitive::getVertexCount() const {
	return m_vertices.getSize() / 3;
}

tpUInt tpPrimitive::getTexCoordsCount() const {
	return m_texcoords.getSize() / 2;
}

tpUInt tpPrimitive::getNormalsCount() const {
	return m_normals.getSize() / 3;
}

void
tpPrimitive::toString(tpString& s) const {
	for (int i = 0; i < getVertexCount();++i) {
		tpLogNotify("v (%d) %3.3f %3.3f %3.3f",i,m_vertices[i*3],m_vertices[i*3+1],m_vertices[i*3+2]);
//		s.append(tpString::format("v (%d) %3.3f %3.3f %3.3f",i*3,m_vertices[i*3],m_vertices[i*3+1],m_vertices[i*3+2]));
	}
}


TP_TYPE_REGISTER(tpPrimitive,tpNode,Primitive);


//////////////////////////////////////////////////////////////////////////

tpPrimitiveFactory* tpPrimitiveFactory::get( bool destroy )
{
	static tpRefPtr<tpPrimitiveFactory> gs_primfactory( (destroy) ? 0L : new tpPrimitiveFactory() );
	return gs_primfactory.get();
}

tpPrimitive* tpPrimitiveFactory::create( tpUShort primitive_type )
{
	tpPrimitive *res(0);
	switch (primitive_type)
	{
	case kAxis:
		res = new tpPrimitive(tpPrimitive::kLines,
			tpPrimitive::kAttributeVertex | tpPrimitive::kAttributeColors
			);

		res->addVertex(tpVec3r(0,0,0),tpVec3r(0,0,1),tpVec2r(0,0),tpVec4r(1,0,0,1));
		res->addVertex(tpVec3r(1,0,0),tpVec3r(0,0,1),tpVec2r(0,0),tpVec4r(1,0,0,1));
		res->addVertex(tpVec3r(0,0,0),tpVec3r(0,0,1),tpVec2r(0,0),tpVec4r(0,1,0,1));
		res->addVertex(tpVec3r(0,1,0),tpVec3r(0,0,1),tpVec2r(0,0),tpVec4r(0,1,0,1));
		res->addVertex(tpVec3r(0,0,0),tpVec3r(0,0,1),tpVec2r(0,0),tpVec4r(0,0,1,1));
		res->addVertex(tpVec3r(0,0,1),tpVec3r(0,0,1),tpVec2r(0,0),tpVec4r(0,0,1,1));
		break;

	case kPlane:
		res = new tpPrimitive(tpPrimitive::kTriangleStrip,
			tpPrimitive::kAttributeVertex | tpPrimitive::kAttributeColors
			);

		res->addVertex(tpVec3r(0,0,0),tpVec3r(0,0,1),tpVec2r(0,1),tpVec4r(0,0,0,1)); // v0
		res->addVertex(tpVec3r(1,0,0),tpVec3r(0,0,1),tpVec2r(1,1),tpVec4r(1,0,0,1)); // v1
		res->addVertex(tpVec3r(0,1,0),tpVec3r(0,0,1),tpVec2r(0,0),tpVec4r(0,1,0,1)); // v3
		res->addVertex(tpVec3r(1,1,0),tpVec3r(0,0,1),tpVec2r(1,0),tpVec4r(1,1,1,1)); // v2

		break;
	}


	return res;
}
