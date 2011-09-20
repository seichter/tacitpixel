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

#ifndef TPIO_H
#define TPIO_H

#include <tp/globals.h>
#include <tp/types.h>

class TP_API tpIO {
public:

	enum {
		kSeekSet = 0,
		kSeekCur = 1,
		kSeekEnd = 2
	};

	enum {
		kGood = 0,
		kEOF =	1 << 1,
		kFail = 1 << 2,
		kBad =	1 << 3
	};

	tpIO() : m_state(kGood) {}
	virtual ~tpIO() {}

	virtual tpIO& read(char* buffer,tpSizeT buffer_size) = 0;
	virtual tpIO& write(const char* buffer, tpSizeT buffer_size) = 0;
	
	

	virtual tpIO& seek(tpSizeT pos,tpUByte rel = kSeekSet) = 0;
	virtual tpSizeT tell() const = 0;

	virtual tpSizeT getCount() const = 0;

	virtual void sync() = 0;

	bool isGood() const { return  (kGood == m_state); }
	bool isFail() const { return (0 == m_state && kFail); }
	bool isBad() const { return  (0 == m_state && kBad); }
	bool isEOF() const { return  (0 == m_state && kEOF); }

	void clear(tpUByte state = kGood) { m_state = state; }

	tpUByte getState() const { return m_state; }

	void rewind() { seek(0,kSeekSet); clear(kGood); }

protected:

	tpUByte m_state;

};

#endif
