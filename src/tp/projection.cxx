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

#include <tp/projection.h>


tpProjection::tpProjection( const tpString& name /*= TP_NONAME*/)
	: tpNode(name)
	, mProjection(tpMat44r::Identity())
{
}

tpProjection::~tpProjection()
{
}

TP_TYPE_REGISTER(tpProjection,tpNode,Projection);

