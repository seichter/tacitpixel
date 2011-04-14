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

#ifndef TPCAMERA_H
#define TPCAMERA_H 1


#include <tp/referenced.h>
#include <tp/thread.h>
#include <tp/mat.h>
#include <tp/vec.h>

/*! \brief camera in a scene
	
	To render a scene you can either use your own 
	matrix before rendering the scene. Or you 
	pass a pointer to a camera (tpCamera) to the
	respective renderengine (tpRenderEngine)
 */
class TP_API tpCamera : public tpReferenced {
public:
	
	TP_TYPE_DECLARE;
	
	enum {
		kOrtho,			//!< Orthographic Projection.
		kPerspective	//!< Perspective camera.
	};

	/*! Standard constructor for a camera.
		\param type type of the camera
	 */
	tpCamera(tpUByte type = kPerspective);
	
	// Destructor
	virtual ~tpCamera();

	/*! similar to gluLookAt. But this will create the respective 
		transformation matrix
		\param pos position of the camera in the scene.
		\param target target point you are looking at
		\param up the vector up from your head
	 */
	void lookAt(const tpVec3r& pos,
		const tpVec3r& target,
		const tpVec3r& up = tpVec3r(0,1,0));
	
	/*! Set the position of the camera.
		\param pos positon of the camera in the scene
	*/
	void setPosition(const tpVec3r& pos);

	/*! Set the position of the camera.
		\param x X-coordinate
		\param y Y-coordinate
		\param z Z-coordinate
	*/
	void setPosition(tpReal x,
		tpReal y,
		tpReal z);

	/*! move the camera.
		\param x X-coordinate
		\param y Y-coordinate
		\param z Z-coordinate
	*/
	void translate(tpReal x,
		tpReal y,
		tpReal z);


	/*! Get the camera position
		\return 3-fold vector with coordinates for camera position.
	 */
	tpVec3r getPosition() const;

	/*! Set absolute axis based orientation of the camera
		\param x Position parameter X 
		\param y Position parameter Y 
		\param z Position parameter Z 
		\param angle Angle around above axis
	 */
	void setRotation(tpReal x, tpReal y, tpReal z,tpReal angle);


	/*! Rotate the camera around an axis
		\param x Position parameter X 
		\param y Position parameter Y 
		\param z Position parameter Z 
		\param angle Angle around above axis

	 */

	void rotate(tpReal x, tpReal y, tpReal z,tpReal angle);


	/*! Set the near clipping plane for this camera.
		\param plane distance to clip in front of rendering volume
	 */
	void setNearClipping(const tpReal& plane);


	/*! Get the near clipping plane.
		\return distance for near clipping.
	 */
	const tpReal& getNearClipping() const;


	/*! Set the far clipping plane for this camera.
		\param plane distance to clip after the rendering volume
	 */
	void setFarClipping(const tpReal& plane);


	/*! Get the far clipping plane.
		\return distance for far clipping plane
	 */
	const tpReal& getFarClipping() const;


	/*! Set the field-of-view for the camera.
		\param fov field of view in degree.
	 */
	void setFOV(const tpReal& fov);


	/*! Return the field of view in degree.
		\return field of view.
	 */
	const tpReal& getFOV() const;


	/*! Set the raw transformation of the camera.
		\param transform non-inverse transformation matrix of the 
		 camera.
	*/
	void setTransform(const tpMat44r& transform);
	

	/*! Returns the projection matrix
		\return matrix of the inverse camera transformation
	*/
	const tpMat44r& getProjection() const;


	/*! Returns the camera transformation
		\return matrix of the camera transformation
	*/
	const tpMat44r& getTransform() const;


	const tpMat44r& getInverseTransform() const;


	void setOrtho() { m_type = kOrtho; apply(); }
	void setPerspective() { m_type = kPerspective; apply(); }

protected:

	void apply();

	void setTransformationMatrix(const tpMat44r& trans, bool isInverted );
	
	void applyInverse();

	tpVec4f m_clearcolor;
	
	tpReal m_aspectratio;

	tpUByte		m_type;

	tpMat44r	m_transform;
	tpMat44r	m_transform_inverse;

	tpMat44r	m_projection;

	tpReal m_nearplane;
	tpReal m_farplane;

	tpReal m_fov;
	
};



#endif
