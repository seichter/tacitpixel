/*
 * Copyright (C) 1999-2013 Hartmut Seichter
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

#ifndef TP_TRAVERSER_H
#define TP_TRAVERSER_H

#include <tp/globals.h>
#include <tp/referenced.h>
#include <tp/types.h>

class tpNode;

/**
  * @brief the traverser implements the hierarchical visitor pattern
  */
struct TP_API tpTraverser {

	enum {
		kDownward,	/** traverses down the children */
		kUpward		/** traverses up into the parents */
	};

	/**
	  * @brief returns traversal direction
	  * @return get direction of traversal
	  */
	virtual unsigned char getDirection() const { return kDownward; }

	/**
	  * @brief enters a node
	  * @param node a node is currently comming in
	  */
	virtual void enter(tpNode* node) {}

	/**
	  * @brief leaves a node
	  * @param node a node that we had visited
	  */
	virtual void leave(tpNode* node) {}

};


#endif
