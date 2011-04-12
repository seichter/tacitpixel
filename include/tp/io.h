#ifndef TPIO_H
#define TPIO_H


class tpIO {
public:
	virtual tpIO& read(char* buffer,tpSizeT buffer_size) = 0;
	virtual tpIO& write(const char* buffer, tpSizeT buffer_size) = 0;

	virtual tpIO& seek(tpSizeT pos) = 0;
	virtual tpSizeT tell() const = 0;

	virtual tpSizeT getCount() const = 0;

	virtual void sync() = 0;
};

#endif