#ifndef TPIMAGEOP_H
#define TPIMAGEOP_H

#include <tp/image.h>


struct TP_API tpImageOperator {
	
	enum {
		kNull = 0x0,
		kSwapRedBlue,
		kGrey
	};

	virtual tpImageOperator& operator()(const tpImage* in, tpImage* out) { return *this; };
	
	static tpImageOperator& o(tpUByte op);

};


#endif