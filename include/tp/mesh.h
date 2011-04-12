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

#ifndef TPMESH_H
#define TPMESH_H


#include <tp/node.h>

/**
 * Mesh types are mapping to the enums in OpenGL
 *	
 */
enum tpMeshType {
	TP_MESH_TRIANGLES		= 0x0004,
	TP_MESH_TRIANGLE_STRIP,
	TP_MESH_TRIANGLE_FAN,
	TP_MESH_QUADS,
	TP_MESH_QUAD_STRIP,
	TP_MESH_POLYGON
};

typedef tpArray<tpDouble> tpArrayd;
typedef tpArray<tpDouble> tpArrayf;
typedef tpArray<tpReal> tpArrayr;

/*!
	\class tpMesh
	\brief an expanded triangle mesh

	This node can hold primitive triangle geometries and
	furthermore, can generate normals and color vertices
*/
class TP_API tpMesh : public tpNode
{
public:

	TP_TYPE_DECLARE;

	/*! c'tor
		\param name name of the name
	 */
	tpMesh(const tpString& name, tpUInt meshtype = TP_MESH_TRIANGLES);

	/*! copy c'tor
		\param mesh mesh to copy
		\param mode mode for copying
	 */ 
	tpMesh(const tpMesh& mesh);

	/*! cloning interface
		\param mode mode to clone the mesh
	 */
	virtual tpObject* clone();

	/*! add a vertex with a vector
	 */
	tpVoid addVertex(const tpVec3r&);
	//! add a vertex with single components
	tpVoid addVertex(tpReal x,tpReal y,tpReal z);

	//! add a normal
	tpVoid addNormal(const tpVec3r&);
	//! add a normal with single components
	tpVoid addNormal(tpReal x,tpReal y,tpReal z);
	    

	//! add a texture coordinate
	tpVoid addTexCoord(tpReal u, tpReal v);

	//! remove a vertex
	void removeVertex(tpUInt id);
	
	//! returns the vertices of that mesh
	const tpArray<tpReal>& getVertices() const;
	//! returns the normals of that mesh
	const tpArray<tpReal>& getNormals() const;
	//! returns the tex corods of that mesh
	const tpArray<tpReal>& getTexCoords() const;

	tpUInt getVertexCount() const;
	tpUInt getNormalsCount() const;
	tpUInt getTexCoordsCount() const;

	tpUInt getMeshType() const;

	tpVoid setMeshType(tpUInt meshtype);

	void scale(const tpVec3r& scale);

	void getAABB(tpVec3r& aabb_min,tpVec3r& aabb_max);

	tpVoid flipNormals();


  
protected:

	virtual ~tpMesh();

	tpUInt	m_meshtype;

	void calcNormals();
	tpArray<tpReal> m_vertices;
	tpArray<tpReal> m_normals;
	tpArray<tpReal> m_texcoords;
};



#endif
