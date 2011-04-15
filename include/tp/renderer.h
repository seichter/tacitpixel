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

#ifndef TPRENDERER_H
#define TPRENDERER_H


#include <tp/node.h>

class tpCamera;



/**
 * \class tpRenderer 
 *
 * The scenegraph rendering layer.
 * 
 */
class TP_API tpRenderer : public tpReferenced {
public:

	enum {
		kNull = 0,
		kOpenGL,
		kDirectX
	};

	TP_TYPE_DECLARE;

	tpRenderer();

	tpRenderer(const tpRenderer& r);

	virtual void operator()( tpNode* node, tpCamera* camera ) = 0;

	virtual tpUInt implementsBackend() const = 0;

	static tpRenderer* create(tpUInt backend);	

protected:

	virtual ~tpRenderer();
};




#endif