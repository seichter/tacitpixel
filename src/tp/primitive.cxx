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
#include "tp/utils.h"
#include "tp/map.h"

#include <cfloat>

#if 0

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
tpPrimitive::addVertex(const tpVec4<tpReal> &position,
					   const tpVec3<tpReal>& normal,
					   const tpVec2<tpReal>& tcoord,
					   const tpVec4<tpReal>& color)
{
	m_vertices.add(position[0]).add(position[1]).add(position[2]).add(1);
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
	return m_vertices.getSize() / 4;
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

		res->setLighting(false);
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

#endif

tpPrimitiveAttribute::tpPrimitiveAttribute( tpUByte stride /*= 4*/, tpUByte attr_type /*= 0*/ )
: mStride(stride)
, mAttrType(attr_type)
{
}

void tpPrimitiveAttribute::setAttributeType( tpUByte attr_type )
{
	mAttrType = attr_type;
}

bool tpPrimitiveAttribute::isAttributeType( tpUByte attr_type ) const
{
	return mAttrType == attr_type;
}

void tpPrimitiveAttribute::remove( tpSizeT idx )
{
	tpSizeT rIdx = idx * mStride;
	for (tpUByte i = 0; i < mStride;++i) mData.erase(rIdx);
}

tpPrimitiveAttribute& tpPrimitiveAttribute::add( const tpFloat *v, tpSizeT size )
{
	// we need to fill data according to OpenGL
	// including proper padding and alignment
	// v2 > v4 => (1,2) > (1,2,0,1)
	for (register tpSizeT i = 0; i < mStride; ++i) {
		if (i < size) {
			mData.add(v[i]);
		} else {
			// add zero for padding and 1 for marking the end
			(i < ((tpSizeT)mStride - 1)) ? mData.add(0) : mData.add(1);
		}
	}
	return *this;
}

tpPrimitiveAttribute& tpPrimitiveAttribute::add( const tpVec4f& v )
{
	return add(v.data(),4);
}

tpPrimitiveAttribute& tpPrimitiveAttribute::add( const tpVec3f& v )
{
	return add(v.data(),3);
}

tpPrimitiveAttribute& tpPrimitiveAttribute::add( const tpVec2f& v )
{
	return add(v.data(),2);
}

void tpPrimitiveAttribute::clear()
{
	mData.clear();
}

const tpFloat*
tpPrimitiveAttribute::getDataOffset(tpSizeT elements) const
{
    return mData.getData() + (this->getStride()*elements);
}

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

tpPrimitive::tpPrimitive( tpUByte primitivetype /*= kTriangles*/, const tpString& name /*= "primitive"*/ )
: tpRenderable(name)
, mPrimType(primitivetype)
{
}

tpPrimitive& tpPrimitive::operator =(const tpPrimitive &rhs)
{
	if (&rhs != this)
	{

	}

	return *this;
}

tpObject*
tpPrimitive::clone()
{
	return new tpPrimitive(*this);
}

bool
tpPrimitive::hasTextureCoordinates() const
{
	return (mTexCoords.isValid() && mTexCoords->getSize());
}

bool
tpPrimitive::hasColors() const
{
	return (mColors.isValid() && mColors->getSize());
}

bool
tpPrimitive::hasNormals() const
{
	return (mNormals.isValid() && mNormals->getSize());
}

bool
tpPrimitive::hasVertices() const
{
	return (mVertices.isValid() && mVertices->getSize());
}


const tpPrimitiveAttribute&
tpPrimitive::getVertices() const
{
	return *mVertices;
}

tpPrimitiveAttribute&
tpPrimitive::getVertices()
{
	if (!mVertices.isValid()) {
		mVertices = new tpPrimitiveAttribute(4,tpPrimitiveAttribute::kVertices);
	}
	return *mVertices;
}


const tpPrimitiveAttribute&
tpPrimitive::getNormals() const
{
	return *mNormals;
}

tpPrimitiveAttribute&
tpPrimitive::getNormals()
{
	if (!mNormals.isValid()) {
        mNormals = new tpPrimitiveAttribute(4,tpPrimitiveAttribute::kNormals);
	}
	return *mNormals;
}


const tpPrimitiveAttribute&
tpPrimitive::getTextureCoordinates() const
{
	return *mTexCoords;
}

tpPrimitiveAttribute&
tpPrimitive::getTextureCoordinates()
{
	if (!mTexCoords.isValid()) {
		mTexCoords = new tpPrimitiveAttribute(tpPrimitiveAttribute::kTexCoords);
	}
	return *mTexCoords;
}

const tpPrimitiveAttribute&
tpPrimitive::getColors() const
{
	return *mColors;
}

tpPrimitiveAttribute&
tpPrimitive::getColors()
{
	if (!mColors.isValid()) {
		mColors = new tpPrimitiveAttribute(4,tpPrimitiveAttribute::kColors);
	}
	return *mColors;
}

void
tpPrimitive::clear()
{
	mVertices = 0;
	mNormals = 0;
	mColors = 0;
	mTexCoords = 0;
}

tpPrimitive&
tpPrimitive::addVertexNormal( const tpVec3r& vertex,const tpVec3f& normal )
{
	getVertices().add(vertex);
	getNormals().add(normal);

	return *this;
}

tpPrimitive&
tpPrimitive::addVertexNormalTextureCoordinate( const tpVec3r& vertex,const tpVec3f& normal,const tpVec2f& tcoord )
{
	getVertices().add(vertex);
	getNormals().add(normal);
	getTextureCoordinates().add(tcoord);

	return *this;
}

tpPrimitive&
tpPrimitive::addVertexNormalColor( const tpVec3r& vertex,const tpVec3f& normal,const tpVec4f& color )
{
	getVertices().add(vertex);
	getNormals().add(normal);
	getColors().add(color);

	return *this;
}

tpPrimitive&
tpPrimitive::addVertexNormalTextureCoordinateColor( const tpVec3r& vertex,const tpVec3f& normal,const tpVec2f& texcoord,const tpVec4f& color )
{
	getVertices().add(vertex);
	getNormals().add(normal);
	getColors().add(color);
	getTextureCoordinates().add(texcoord);

	return *this;
}

void
tpPrimitive::setUniformColor(const tpVec4f& color)
{
	mColors = 0;
	for (tpUInt i = 0; i < getVertices().getSize();++i) getColors().add(color);
}

void
tpPrimitive::translate(const tpVec3f& vec)
{
	for (tpUInt i = 0; i < getVertices().getSize();++i)
	{
		getVertices().getData()[i*getVertices().getStride()+0] += vec(0);
		getVertices().getData()[i*getVertices().getStride()+1] += vec(1);
		getVertices().getData()[i*getVertices().getStride()+2] += vec(2);
	}
}

void
tpPrimitive::scale(const tpVec3f& vec)
{
	for (tpUInt i = 0; i < getVertices().getSize();++i)
	{
		getVertices().getData()[i*getVertices().getStride()+0] *= vec(0);
		getVertices().getData()[i*getVertices().getStride()+1] *= vec(1);
		getVertices().getData()[i*getVertices().getStride()+2] *= vec(2);
	}
}

void
tpPrimitive::flipNormals()
{
	for (tpUInt i = 0; i < getNormals().getSize();++i)
	{
		getNormals().getData()[i*getNormals().getStride()+0] *= -1;
		getNormals().getData()[i*getNormals().getStride()+1] *= -1;
		getNormals().getData()[i*getNormals().getStride()+2] *= -1;
	}

}

void
tpPrimitive::checkNormals() const
{
    for (tpUInt i = 0; i < getNormals().getSize();++i)
    {
        tpVec3f n(getNormals().getData()[i*getNormals().getStride()+0],
                  getNormals().getData()[i*getNormals().getStride()+1],
                  getNormals().getData()[i*getNormals().getStride()+2]
                  );

        //tpLogMessage("%3.6f l",n.getLength());

//        getNormals().getData()[i*getNormals().getStride()+0] *= -1;
//        getNormals().getData()[i*getNormals().getStride()+1] *= -1;
//        getNormals().getData()[i*getNormals().getStride()+2] *= -1;
    }
}

void
tpPrimitive::getAABB(tpVec3r& aabb_min,tpVec3r& aabb_max)
{

    aabb_max(0) = aabb_max(1) = aabb_max(2) = FLT_MIN;
    aabb_min(0) = aabb_min(1) = aabb_min(2) = FLT_MAX;

    for (tpUInt i = 0; i < getVertices().getSize();++i)
    {
        aabb_min(0) = tpMin(aabb_min(0),getVertices().getData()[i*getVertices().getStride()+0]);
        aabb_min(1) = tpMin(aabb_min(1),getVertices().getData()[i*getVertices().getStride()+1]);
        aabb_min(2) = tpMin(aabb_min(2),getVertices().getData()[i*getVertices().getStride()+2]);

        aabb_max(0) = tpMax(aabb_max(0),getVertices().getData()[i*getVertices().getStride()+0]);
        aabb_max(1) = tpMax(aabb_max(1),getVertices().getData()[i*getVertices().getStride()+1]);
        aabb_max(2) = tpMax(aabb_max(2),getVertices().getData()[i*getVertices().getStride()+2]);
    }
}

//////////////////////////////////////////////////////////////////////////

tpPrimitive*
tpPrimitiveFactory::create( tpUShort primitive_type )
{
	tpPrimitive *res(0);
	switch (primitive_type)
	{
	case kAxis:
		res = new tpPrimitive(tpPrimitive::kLines,"Axis");

		res->addVertexNormalColor(tpVec3r(0,0,0),tpVec3r(0,0,1),tpVec4r(1,0,0,1));
		res->addVertexNormalColor(tpVec3r(1,0,0),tpVec3r(0,0,1),tpVec4r(1,0,0,1));
		res->addVertexNormalColor(tpVec3r(0,0,0),tpVec3r(0,0,1),tpVec4r(0,1,0,1));
		res->addVertexNormalColor(tpVec3r(0,1,0),tpVec3r(0,0,1),tpVec4r(0,1,0,1));
		res->addVertexNormalColor(tpVec3r(0,0,0),tpVec3r(0,0,1),tpVec4r(0,0,1,1));
		res->addVertexNormalColor(tpVec3r(0,0,1),tpVec3r(0,0,1),tpVec4r(0,0,1,1));

        break;

	case kPlane:
		res = new tpPrimitive(tpPrimitive::kTriangleStrip);

		res->addVertexNormalTextureCoordinateColor(tpVec3r(0,0,0),tpVec3r(0,0,1),tpVec2r(0,1),tpVec4r(0,0,0,1)); // v0
		res->addVertexNormalTextureCoordinateColor(tpVec3r(1,0,0),tpVec3r(0,0,1),tpVec2r(1,1),tpVec4r(1,0,0,1)); // v1
		res->addVertexNormalTextureCoordinateColor(tpVec3r(0,1,0),tpVec3r(0,0,1),tpVec2r(0,0),tpVec4r(0,1,0,1)); // v3
		res->addVertexNormalTextureCoordinateColor(tpVec3r(1,1,0),tpVec3r(0,0,1),tpVec2r(1,0),tpVec4r(1,1,1,1)); // v2

        res->addRenderFlag(tpRenderFlag::kLighting);

		break;


	case kSphere:
		{

			// moved from old tpSphere class ... need to parameterize this
			const tpReal radius = 1;
			const tpUInt divisions = 20;
			tpReal _division = tpReal(360.0)/divisions;

			res = new tpPrimitive(tpPrimitive::kTriangleStrip);

			//res->setPrimitiveType(tpPrimitive::kLineStrip);

			tpReal   _latitude, _longitude;
			tpReal   dToR;
			tpReal x, y, z;

			dToR = TP_PI / tpReal(180.0);

			for (_latitude = -90.0; _latitude <= 90.0; _latitude += _division) {

				// loop the other way around it
				for (_longitude = 0; _longitude <= 360; _longitude += _division) {

					x = tpReal( sin ( _longitude * dToR) * cos ((_latitude + _division) * dToR) );
					y = tpReal( sin ( (_latitude + _division) * dToR) );
					z = tpReal( cos ( _longitude * dToR) * cos ((_latitude + _division) * dToR) );

					// assign the second normal and vertex
					res->addVertexNormal(tpVec3r(x * radius,y * radius,z * radius),
										 tpVec3r(x,y,z)
										 );

					// calculate the coordinates
					x = sin ( _longitude * dToR) * cos (_latitude * dToR);
					y = sin ( _latitude * dToR);
					z = cos ( _longitude * dToR) * cos (_latitude * dToR);

					// assign a normal and a vertex
					res->addVertexNormal(
								tpVec3r(x * radius,y * radius,z * radius),
								tpVec3r(x,y,z)
								);

				}
			}

            res->addRenderFlag(tpRenderFlag::kLighting);

		}

	}


	return res;
}

