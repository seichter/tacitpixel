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

#ifndef TPHASH_H
#define TPHASH_H

#include <tp/globals.h>
#include <tp/types.h>


/*! Hash function. Implements the Adler-32 hash algorithm.
 *  \param buffer buffer to be hashed
 *  \param bufsize size of the buffer
 *  \return the hashcode
 */
TP_API tpULong tpAdler32(const tpUChar* buffer,tpULong bufsize);

#endif
