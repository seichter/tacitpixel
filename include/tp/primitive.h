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
	
	enum {
		kAttributeVertex 			= 0,
		kAttributeNormal 			= (1 << 1),
		kAttributeTextureCoordinate = (1 << 2),
		kAttributeColorPerVertex	= (1 << 3)
	};
	
	/*! c'tor
		\param name name of the name
	 */
	tpPrimitive(tpUByte meshtype = kTriangles, 
		tpUShort attributes = kAttributeVertex | kAttributeNormal);

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
	
	void setAttributes(tpUShort attrib) { m_attributes = attrib; }
	tpUShort getAttributes() const { return m_attributes; }
	
	bool hasAttribute(tpUByte attrib) const { return (0 != (m_attributes & attrib)); }


protected:

	virtual ~tpPrimitive();

	tpUByte	m_primitivetype;
	tpUShort m_attributes;

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
