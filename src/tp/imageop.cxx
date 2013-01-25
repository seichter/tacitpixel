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

#include <tp/imageop.h>
#include <tp/log.h>

struct tpImageOperatorNull {
	
	void impl1(tpImage& img) { tpLogMessage(__FUNCTION__); }
	void impl8(tpImage& img) { tpLogMessage(__FUNCTION__); }
	
};


struct tpImageOpRGB2BGR {
	
	void impl1(tpImage& img) 
	{
		char* p = static_cast<char*>(img.getData());
		for (tpSizeT i = 0; i < img.getDataSize(); i+= 3)
		{
			tpSwap(p[i],p[i+2]);
		}
	}
	
	void impl8(tpImage& img) 
	{
		char* p = static_cast<char*>(img.getData());
		for (tpSizeT i = 0; i < img.getDataSize(); i+= 24)
		{
			tpSwap(p[i+ 0],p[i+ 2]);
			tpSwap(p[i+ 3],p[i+ 5]);
			tpSwap(p[i+ 6],p[i+ 8]);
			tpSwap(p[i+ 9],p[i+11]); 
			tpSwap(p[i+12],p[i+14]);
			tpSwap(p[i+15],p[i+17]);
			tpSwap(p[i+18],p[i+20]);
			tpSwap(p[i+21],p[i+23]);
		}
	}	
};



template <typename T> struct tpImageOperatorByteAligned : tpImageOperator {
	
	T* m;
	
	void (T::*_impl1)(tpImage& img);
	void (T::*_impl8)(tpImage& img);
	
	tpImageOperatorByteAligned(T* pObj, void (T::*impl1)(tpImage& img), void (T::*impl8)(tpImage& img) ) 
	: m(pObj), _impl1(impl1), _impl8(impl8)
	{
		if (0 == _impl8) _impl8 = _impl1;
	}
	
	tpImageOperator& operator()(const tpImage* in, tpImage* out) 
	{
		if (!out) out = const_cast<tpImage*>(in);
		(in->getDataSize() % 8 == 0) ? (*m.*_impl8)(*out) : (*m.*_impl1)(*out);
		return *this;
	}
};



#if 0
// faster computation, (2*R + 5*G + B) / 8
    tmp = in[i] << 1;                   // 2 * red
    tmp += in[i+1] << 2 + in[i+1];      // 5 * green
    tmp += in[i+2];                     // 1 * blue
    out[i] = (unsigned char)(tmp >> 3); // divide by 8
#endif

#define RGB8882GRAY8_SHIFT(R,G,B) \
	(int)((R+R+G+G+G+G+G+B)>>3)
	 

struct tpToGrayOp : tpImageOperator {

	inline void _rgb2grey(const tpImage* in, tpImage* out)
	{
		short tmp;
		const char* src_ptr = static_cast<const char*>(in->getData());
		char* dst_ptr = static_cast<char*>(out->getData());
		for (tpSizeT i = out->getWidth() * out->getHeight(); i > 1; --i)
		{
			tmp  = src_ptr[0] << 1;
			tmp += (src_ptr[1] << 2) + src_ptr[1];
			tmp += src_ptr[2];
			*dst_ptr = (char)(tmp >> 3);
			dst_ptr++; src_ptr += 3;
		}
	}

	
	tpImageOperator& operator()(const tpImage* in, tpImage* out)
	{
		if (in && out && out->getPixelFormat() == tpPixelFormat::kGray_8)
		{
			switch(in->getPixelFormat())
			{
				case tpPixelFormat::kRGB_888:
					_rgb2grey(in,out);
					break;
			}
		}
		
		return *this;
	}
};

tpImageOperator& 
tpImageOperator::o(tpUByte op)
{
	static tpToGrayOp gs_togray;
	static tpImageOperator gs_null;

	switch (op) {
//		case tpImageOperatorManager::kNull:
//			ret = new tpImageOperatorByteAligned<tpImageOperatorNull>(&nullop,&tpImageOperatorNull::impl1,&tpImageOperatorNull::impl8);
//			break;
//		case tpImageOperatorManager::kSwapRedBlue:
//			ret = new tpImageOperatorByteAligned<tpImageOpRGB2BGR>(&swapredblue,&tpImageOpRGB2BGR::impl1,&tpImageOpRGB2BGR::impl8);
//			break;
		case tpImageOperator::kGrey : 
			return gs_togray;
			break;
		default:
			break;
	}
	
	return gs_null;
}
