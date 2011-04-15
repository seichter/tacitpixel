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

#ifndef TPGLRENDERER
#define TPGLRENDERER 1

#include <tp/renderer.h>
#include <tp/matrix.h>
//#include "tpRefCache.h"

#include "tpGLShader.h"

typedef tpMatrix< tpReal > tpGLMatrix;

/*

class tpGLContext : public tpContext
{
	tpStack< tpGLMatrix > m_modelviewstack;
	tpStack< tpGLMatrix > m_projectionstack;

public:

	tpGLContext() : tpContext()
	{
		m_modelviewstack.push(tpMatrix<tpReal>());
	}
	
	const tpGLMatrix& getCurrentModelView() const { return m_modelviewstack.getTop(); }

	void pushAndMulModelView(const tpMatrix<tpReal>& mat) {
		m_modelviewstack.push( getCurrentModelView() * mat );
	}

	void pushModelView(const tpMatrix<tpReal>& mat) {
		m_modelviewstack.push( mat );
	}

	void popModelView() { 
		m_modelviewstack.pop(); 
	}
	
	void pushProjection(const tpMatrix<tpReal>& mat) {
		m_projectionstack.push( mat );
	}

	void popProjection() { 
		m_projectionstack.pop(); 
	}

	const tpGLMatrix& getCurrentProjection() const {
		return m_projectionstack.getTop();
	}

	//void pushProgram(tpUInt prog) { m_programstack.push( prog ); }
	//void popProgram() { m_programstack.pop(); }
	

};

*/

class tpGLTraverser : public tpTraverser {
public:
	virtual void operator()(tpNode*,tpCamera*) = 0;
};



class tpGLRenderer : public tpRenderer {
public:

	TP_TYPE_DECLARE;

	tpGLRenderer();

	void operator()( tpNode* node, tpCamera* camera );

	tpUInt implementsBackend() const;

protected:

	virtual ~tpGLRenderer();

	tpRefPtr<tpGLTraverser> m_traverser;

};

#endif
