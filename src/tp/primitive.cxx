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
#include "tp/primitive.h"           
#include "tp/math.h"
#include "tp/log.h"
#include "tp/node.h"


tpPrimitive::tpPrimitive(tpUByte meshtype)
	: tpRenderable() ,
	m_primitivetype(meshtype)
{
}

tpPrimitive::tpPrimitive(const tpPrimitive& geo)
	: tpRenderable(geo),
	m_primitivetype(geo.m_primitivetype)
{
	m_vertices = geo.m_vertices;
	m_normals = geo.m_normals;
	m_texcoords = geo.m_texcoords;
	m_colors = geo.m_colors;
}

tpObject* tpPrimitive::clone()
{
	return new tpPrimitive(*this);
}

tpUInt tpPrimitive::getPrimitiveType() const
{
	return this->m_primitivetype;
}

void tpPrimitive::setPrimitiveType(tpUInt meshtype)
{
	this->m_primitivetype = (tpMeshType)meshtype;
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

};


void tpPrimitive::flipNormals() 
{
	tpReal *n_ptr = m_normals.getData();

	for (tpULong i = getNormalsCount(); i; --i) 
	{
		*n_ptr++ *= -1;
		*n_ptr++ *= -1;
		*n_ptr++ *= -1;	}
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

tpPrimitive::~tpPrimitive()
{
}


TP_TYPE_REGISTER(tpPrimitive,tpNode,Mesh);

