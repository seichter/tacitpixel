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
#include "tp/mesh.h"           
#include "tp/math.h"
#include "tp/log.h"
#include "tp/node.h"


tpMesh::tpMesh(const tpString& name,tpUInt meshtype)
	: tpNode(name) ,
	m_meshtype(meshtype)
{
}

tpMesh::tpMesh(const tpMesh& geo)
	: tpNode(geo),
	m_meshtype(geo.m_meshtype)
{
	m_vertices = geo.m_vertices;
	m_normals = geo.m_normals;
	m_texcoords = geo.m_texcoords;
};

tpObject* tpMesh::clone()
{
	return new tpMesh(*this);
};

tpUInt tpMesh::getMeshType() const
{
	return this->m_meshtype;
}

tpVoid tpMesh::setMeshType(tpUInt meshtype)
{
	this->m_meshtype = (tpMeshType)meshtype;
}

tpVoid tpMesh::addVertex( tpReal x, tpReal y, tpReal z ) {
	m_vertices.add(x);
	m_vertices.add(y);
	m_vertices.add(z);
}

tpVoid tpMesh::addVertex(const tpVec3r& vec)
{
	return addVertex(vec[0],vec[1],vec[2]);
}

tpVoid tpMesh::addNormal(tpReal x,tpReal y,tpReal z) {
	m_normals.add(x);m_normals.add(y);m_normals.add(z);
}

tpVoid tpMesh::addNormal(const tpVec3r& vec) {
	addNormal(vec[0],vec[1],vec[2]);
}

tpVoid tpMesh::addTexCoord(tpReal u, tpReal v) {
	m_texcoords.add(u); m_texcoords.add(v);
}

void tpMesh::removeVertex( tpUInt id ) {
	m_vertices.erase(id);
}

void tpMesh::scale(const tpVec3r& scale)
{
	tpReal* v_ptr = m_vertices.getData();
	for (tpSizeT v_len = getVertexCount(); v_len; v_len--)
	{
		*v_ptr++ *= scale[0];
		*v_ptr++ *= scale[1];
		*v_ptr++ *= scale[2];
	}
}

void tpMesh::getAABB(tpVec3r& aabb_min,tpVec3r& aabb_max)
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

const tpArray<tpReal>& tpMesh::getVertices() const {
	return m_vertices;
}

const tpArray<tpReal>& tpMesh::getNormals() const {
	return m_normals;
}

const tpArray<tpReal>& tpMesh::getTexCoords() const {
	return m_texcoords;
}


tpVec3r tpGetNormal(const tpArray<tpReal>& vertices)
{
	tpVec3r normal(0,1,0);
	
	/* minimum of 3x3 vertices needed */
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


tpVoid tpMesh::flipNormals() 
{
	tpReal *n_ptr = m_normals.getData();

	for (tpULong i = getNormalsCount(); i; --i) 
	{
		*n_ptr++ *= -1;
		*n_ptr++ *= -1;
		*n_ptr++ *= -1;	}
}


tpUInt tpMesh::getVertexCount() const {
	return m_vertices.getSize() / 3;
}

tpUInt tpMesh::getTexCoordsCount() const {
	return m_texcoords.getSize() / 2;
}

tpUInt tpMesh::getNormalsCount() const {
	return m_normals.getSize() / 3;
}

tpMesh::~tpMesh()
{
}

TP_TYPE_REGISTER(tpMesh,tpNode,Mesh);

