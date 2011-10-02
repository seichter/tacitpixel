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

#ifndef TPNODE_H
#define TPNODE_H


#include <tp/globals.h>
#include <tp/string.h>
#include <tp/referenced.h>
#include <tp/object.h>
#include <tp/refptr.h>
#include <tp/traverser.h>
#include <tp/matrix.h>


typedef tpArray<tpNode*> tpNodeArray;
typedef tpArray<tpRefPtr<tpNode> > tpRefNodeArray;
typedef tpArray<tpNodeArray> tpNodeArrayList;

typedef tpArray<tpMat44r> tpMatrixArray;

/*!
	\class tpNode
	\brief abstract traversable Node for the scenegraph
*/

class TP_API tpNode : public tpObject {
public:

	TP_TYPE_DECLARE;

	//! standard c'tor
	tpNode(const tpString& name = "");

	//! copy c'tor
	tpNode(const tpNode& node);

	//! cloning interface
	virtual tpObject* clone();

	//! traversing interface
	void traverse(tpTraverser& traverser);

	//! add a child node
	tpNode* addChild(tpNode* node);

	//! remove child node
	tpNode* removeChild(tpNode* node);

	//! set child node
	bool setChild(tpSizeT idx,tpNode* node);

	//! get nested children
	tpSizeT getChildCount() const { return m_children.getSize(); }

	//! get child at index
	tpNode* getChild(tpSizeT idx) { return m_children[idx].get(); }

	//! get child at index (const)
	const tpNode* getChild(tpSizeT idx) const { return m_children[idx].get(); }

	//! get index of child
	tpSizeT getIndexOfChild(const tpNode* c) const {
		for (tpSizeT i = 0; i < m_children.getSize(); i++)
		{
			if (c == m_children[i].get()) return i;
		}
		return static_cast<tpSizeT>(-1);
	}

	//! get parent list
	const tpArray<tpNode*>& getParents() const { return m_parents; }

	//! get copy of parent list
	tpArray<tpNode*> getParents() { return m_parents; }

	//! get count of parent nodes
	tpSizeT getParentsCount() const { return m_parents.getSize(); }

	//! check if this node has any parent > root
	bool hasParents() const { return (getParentsCount() > 0); }

	//! check if this node has any child nodes > leaf
	bool hasChildren() const { return (getChildCount() > 0); }

	//!
	const tpNodeArrayList& getNodePaths() const { return m_nodepath; }

	//! read a node from a file
	static tpNode* read(const tpString& file);

protected:

	virtual ~tpNode();

	tpRefNodeArray m_children;
	tpNodeArray m_parents;

	tpNodeArrayList m_nodepath;
	tpMatrixArray m_matrices;

};


class TP_API tpNodeHandler : public tpReferenced {
public:

	TP_TYPE_DECLARE;

	enum {
		kNone  = 0,
		kRead  = (1 << 1),
		kWrite = (1 << 2)
	};

	tpNodeHandler();

	virtual tpUByte getCapabilities(const tpString& name) = 0;

	virtual tpNode* read(const tpString& name) { return 0; }
	virtual bool write(const tpNode* node, const tpString& name) { return false; }

protected:

	~tpNodeHandler();
};


#endif
