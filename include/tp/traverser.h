#ifndef TP_TRAVERSER_H
#define TP_TRAVERSER_H

#include <tp/globals.h>
#include <tp/referenced.h>
#include <tp/types.h>

class tpNode;

class tpTraverser : public tpReferenced {
public:
	
	enum {
		E_PUSH,
		E_POP
	};

	tpTraverser();

	virtual void push(tpNode* node) = 0;
	virtual void pop(tpNode* node) = 0;
	
	tpUInt getFrameCount() const { return m_framecount; }
	
protected:
	
	virtual ~tpTraverser();
	
	tpUInt m_framecount;
	
};


#endif