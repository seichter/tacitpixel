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

class tpContext;
class tpTraverser;


/*
class TP_API tpRenderObject : public tpReferenced {
public:

	tpRenderObject() {};

	virtual tpVoid onEvent(tpContext& context) {};
	virtual tpVoid onCull(tpContext& context) {};
	virtual tpVoid onUpdate(tpContext& context) {};
	virtual tpVoid onDraw(tpContext& context) {};


	tpVoid setNestedRenderObject( tpRenderObject* robj ) { m_nested = robj; }
	tpRenderObject* getNestedRenderObject() const { return m_nested.get(); }

protected:

	tpRefPtr<tpRenderObject> m_nested;

	virtual ~tpRenderObject() {};
};

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
 
 
class tpGroup;


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
	virtual void traverse(tpTraverser* traverse, tpContext& context);
	
	//! add a child node
	void addChild(tpNode* node);
	
	//! remove child node
	void removeChild(tpNode* node);
	
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
	
	
protected:

    virtual ~tpNode();

	tpArray<tpNode*> m_parents;
	tpArray< tpRefPtr<tpNode> > m_children;

};


#endif
