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