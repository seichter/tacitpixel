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

#ifndef TPPRIMITIVE_H
#define TPPRIMITIVE_H

#include <tp/renderable.h>


class TP_API tpPrimitiveAttribute : public tpReferenced {
protected:
	tpArray<tpFloat> mData;
	tpUByte mStride;
	tpUByte mAttrType;
public:

	enum {
		kVertices = 0,
		kNormals,
		kColors,
		kTexCoords,
		kUser = 128
	};

	tpPrimitiveAttribute(tpUByte stride = 4, tpUByte attr_type = kVertices);

	void setAttributeType(tpUByte attr_type);
	tpUByte getAttributeType() const { return mAttrType; }
	bool isAttributeType( tpUByte attr_type ) const;


	tpSizeT getSize() const { return mData.getSize() / mStride; }

	void remove(tpSizeT idx);

	void clear();

	tpUByte getStride() const { return mStride; }

	const tpFloat* getData() const { return mData.getData(); }
	tpFloat* getData() { return mData.getData(); }

	tpPrimitiveAttribute& add(const tpVec4f& v);
	tpPrimitiveAttribute& add(const tpVec3f& v);
	tpPrimitiveAttribute& add( const tpVec2f& v );
    tpPrimitiveAttribute& add( const tpFloat *v, tpSizeT size );

    const tpFloat *getDataOffset(tpSizeT elements) const;
};


class TP_API tpPrimitive : public tpRenderable {
protected:
	tpRefPtr<tpPrimitiveAttribute> mVertices;
	tpRefPtr<tpPrimitiveAttribute> mNormals;
	tpRefPtr<tpPrimitiveAttribute> mTexCoords;
	tpRefPtr<tpPrimitiveAttribute> mColors;

    tpArray<tpUInt16> mIndices;

	typedef tpArray<tpRefPtr<tpPrimitiveAttribute> > tpRefPrimitiveAttributeArray;

	tpRefPrimitiveAttributeArray mUserAttributes;

	tpUByte mPrimType;
public:

	enum PrimitiveType {
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

	tpPrimitive(tpUByte primitivetype = kTriangles, const tpString& name = "primitive");

	tpPrimitive &operator =(const tpPrimitive &rhs);


	tpObject *clone(const tpUByte& m) const;


	bool hasVertices() const;

	bool hasNormals() const;

	bool hasColors() const;

	bool hasTextureCoordinates() const;

	tpPrimitiveAttribute& getVertices();

	const tpPrimitiveAttribute& getVertices() const;

	tpPrimitiveAttribute& getNormals();

	const tpPrimitiveAttribute& getNormals() const;

	tpPrimitiveAttribute& getColors();

	const tpPrimitiveAttribute& getColors() const;

	tpPrimitiveAttribute& getTextureCoordinates();

	const tpPrimitiveAttribute& getTextureCoordinates() const;

	tpPrimitive& addVertexNormal( const tpVec3r& vertex,const tpVec3f& normal );

	tpPrimitive& addVertexNormalTextureCoordinate( const tpVec3r& vertex,const tpVec3f& normal,const tpVec2f& tcoord );

	tpPrimitive& addVertexNormalColor(const tpVec3r& vertex,const tpVec3f& normal,const tpVec4f& color);

	tpPrimitive& addVertexNormalTextureCoordinateColor( const tpVec3r& vertex,const tpVec3f& normal,const tpVec2f& texcoord,const tpVec4f& color );

    void clearIndices() { mIndices.clear(); }

    void addIndex(tpUInt16 idx) { mIndices.add(idx); }

    template <typename T>
    void copyIndices(T* idx, tpSizeT size)
    {
        for (tpSizeT i = 0; i < size; ++i) mIndices.add(idx[i]);
    }

    const tpUInt16* getIndices() const { return mIndices.getData(); }

	void clear();

	tpUByte getPrimitiveType() const { return mPrimType; }

	void setPrimitiveType(tpUByte val) { mPrimType = val; }

	void setUniformColor(const tpVec4f &color);

	void translate(const tpVec3f &vec);

	void scale(const tpVec3f &vec);

    void flipNormals();
    void checkNormals() const;
};



struct TP_API tpPrimitiveFactory {

	enum {
		kPlane,
		kSphere,
		kAxis,
		kTeapot
	};

	static tpPrimitive* create(tpUShort primitive_type);
};


#endif
