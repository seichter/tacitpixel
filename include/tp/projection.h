
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

#ifndef TPPROJECTION_H
#define TPPROJECTION_H

#include <tp/node.h>
#include <tp/mat.h>


class TP_API tpProjection : public tpNode {
public:

	TP_TYPE_DECLARE

	tpProjection( const tpString& name = "" );

	void setProjection(const tpMat44r& m) { mProjection = m; }

	tpMat44r getProjection() { return mProjection; }

	const tpMat44r& getProjection() const { return mProjection; }

private:

	tpMat44r mProjection;

	virtual ~tpProjection();
};


#endif
