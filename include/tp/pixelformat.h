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

#ifndef TP_PIXELFORMAT_H
#define TP_PIXELFORMAT_H 1


struct tpPixelFormat {

	enum {
		kRGB_888,
		kBGR_888,
		kRGBA_8888,
		kBGRA_8888,
		kRGB_565,
		kBGR_565,
		kRGB_555,
		kBGR_555,
		kGray_8,
		kYUV_420
	};

	inline static
	tpUShort getBitsPerPixel(const tpUByte& format)
	{
		switch (format)
		{
			case kRGB_888:
			case kBGR_888:
				return 24;
			case kRGBA_8888:
			case kBGRA_8888:
				return 32;
			case kYUV_420:
				return 12;
			case kRGB_565:
			case kRGB_555:
				return 16;
			case kGray_8:
				return 8;
			default:
				break;
		};
		return 0;
	}

};

#endif
