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

#ifndef TPNODE_H
#define TPNODE_H


#include <tp/globals.h>
#include <tp/string.h>
#include <tp/referenced.h>
#include <tp/object.h>
#include <tp/refptr.h>
#include <tp/traverser.h>
#include <tp/matrix.h>


class TP_API tpRenderObject : public tpReferenced {
public:

	tpRenderObject() {};

	/*
	virtual void onEvent(tpContext& context) {};
	virtual void onCull(tpContext& context) {};
	virtual void onUpdate(tpContext& context) {};
	virtual void onDraw(tpContext& context) {};
	*/

	tpVoid setNestedRenderObject( tpRenderObject* robj ) { m_nested = robj; }
	tpRenderObject* getNestedRenderObject() const { return m_nested.get(); }

protected:

	tpRefPtr<tpRenderObject> m_nested;

	virtual ~tpRenderObject() {};
};


/*

enum tpTraverseFlags
{
	TP_TRAVERSE_OFF	 = 0,
	TP_TRAVERSE_DRAW = 0 << 1,
	TP_TRAVERSE_STAT = 1 << 1,
	TP_TRAVERSE_DISTRIBUTE = 2 << 1
};

//////////////////////////////////////////////////////////////////////////

enum tpNodeFactoryCapabilities {
	TP_NODE_CAN_READ = 0x0,
	TP_NODE_CAN_WRITE
};


class TP_API tpNodeFactory : public tpReferenced {
public:

	TP_TYPE_DECLARE;

	tpNodeFactory();

	virtual tpBool getCapabilities(tpUInt capability, const tpString& name) { return false; }

	virtual tpNode* read(const tpString& name) { return 0; }
	virtual tpBool write(tpNode* node, const tpString& name) { return false; }

protected:

	~tpNodeFactory();
};

*/
 
 
//class tpGroup;


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
	
	
protected:

    virtual ~tpNode();

	tpArray< tpRefPtr<tpNode> > m_children;

	tpArray<tpNode*> m_parents;

	tpArray<tpNode*> m_nodepath;
	tpArray<tpMat44r*> m_matrices;
};


#endif
