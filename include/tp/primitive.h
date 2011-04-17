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


#include <tp/renderable.h>

/**
 * Mesh types are mapping to the enums in OpenGL
 *	
 */
enum tpMeshType {
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
class TP_API tpPrimitive : public tpRenderable {
public:

	TP_TYPE_DECLARE;
	
	enum {
		kPoints = 0x0000,
		kLines,
		kLineStrip,
		kLineLoop,
		kTriangles,
		kTriangleStrip,
		kTriangleFan,
		kQuads,
		kQuadStrip,
		kPolygon
	};
	
	/*! c'tor
		\param name name of the name
	 */
	tpPrimitive(tpUByte meshtype = kTriangles);

	/*! copy c'tor
		\param mesh mesh to copy
		\param mode mode for copying
	 */ 
	tpPrimitive(const tpPrimitive& mesh);

	/*! cloning interface
		\param mode mode to clone the mesh
	 */
	virtual tpObject* clone();

	void addVertex(const tpVec3<tpReal>& pos, 
		const tpVec3<tpReal>& normal = tpVec3r(0,0,1), 
		const tpVec2<tpReal>& tcoord = tpVec2r(0,1),
		const tpVec4<tpReal>& color = tpVec4r(1,1,1,1));
	    
	//! remove a vertex
	void removeVertex(tpUInt id);
	
	//! returns the vertices of that mesh
	const tpArray<tpReal>& getVertices() const;
	//! returns the normals of that mesh
	const tpArray<tpReal>& getNormals() const;
	//! returns the tex corods of that mesh
	const tpArray<tpReal>& getTexCoords() const;
	//! returns color array
	const tpArray<tpReal>& getColors() const { return m_colors; }

	tpUInt getVertexCount() const;
	tpUInt getNormalsCount() const;
	tpUInt getTexCoordsCount() const;

	tpUInt getPrimitiveType() const;
	void setPrimitiveType(tpUInt meshtype);

	void scale(const tpVec3r& scale);

	void getAABB(tpVec3r& aabb_min,tpVec3r& aabb_max);

	void flipNormals();


protected:

	virtual ~tpPrimitive();

	tpUByte	m_primitivetype;

	void calcNormals();
	
	tpArray<tpReal> m_vertices;
	tpArray<tpReal> m_normals;
	tpArray<tpReal> m_texcoords;
	tpArray<tpReal> m_colors;
};


class TP_API tpPrimitiveFactory : public tpReferenced {
public:

	enum {
		kPlane,
		kSphere,
		kAxis,
		kTeapot
	};

	static tpPrimitiveFactory* get(bool destroy = false);

	tpPrimitive* create(tpUShort primitive_type);
};


#endif
